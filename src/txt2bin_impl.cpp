#include "txt2bin_impl.h"
#if 0
#include "utils/verify.h"
#include "io/writer.h"
#include "Utils.h" // VERIFY
#include "utils/scope_exit.h" // CswMakeScopeExit
#include "io/line_reader.h"
#include "io/windows_file_byte_reader.h"
#include "io/windows_file_byte_writer.h"
#include "io/buffered_writer.h"
#include "utils/convert.h"

#include <cstdio> // std::FILE
#include <algorithm> // std::copy
#include <cstdlib> // EXIT_SUCCESS

#include <Windows.h>
#include <stdio.h> // _wfopen
#include <string>
#include <iostream>
#include <cstdlib> // std::strtod
#include <cmath> // HUGE_VAL
#include <clocale> // std::setlocale LC_ALL
#include <cerrno>


static const std::uint8_t s_control_byte = 0xFE;
static const std::uint8_t s_jump_byte = 0xFD;
static const std::uint8_t s_rates[]  = {0b0000'0000, 0b0100'0000, 0b1000'0000, 0b1100'0000}; // hex: {0x00, 0x40, 0x80, 0xC0} dec: {0, 64, 128, 192}
static const std::uint8_t s_ranges[] = {0b0000'0000, 0b0001'0000, 0b0010'0000}; // hex: {0x00, 0x10, 0x20} dec: {0, 16, 32} value 0b0011'0000 == 0x30 == 48 is missing
static const std::uint8_t s_rate_mask        = 0b1100'0000;
static const std::uint8_t s_range_mask       = 0b0011'0000;
static const std::uint8_t s_digital_out_mask = 0b0000'1111;
static const erange s_default_range = erange::range10;
static const erate s_default_rate = erate::rate10;
static const digital_io s_default_dio = digital_io{0x0F};
static const std::uint8_t s_default_byte = 0xFF;
static const double s_volts_10_nominal = 10.0;
static const double s_volts_1_nominal = 1.414;
static const double s_volts_01_nominal = 0.157;
static const double s_volts_conversion_table[] = {s_volts_10_nominal, s_volts_1_nominal, s_volts_01_nominal};
static const double s_20bits_as_double = static_cast<double>(0x000F'FFFF);
static const double s_max10volts = 10.357;
static const std::uint32_t s_minus_bit = 0x0010'0000u; // only bit 21 is set
static const std::uint32_t s_21bits_mask = 0x001F'FFFFu;
static const std::uint32_t s_block1start = 0x0000'0000;
static const std::uint32_t s_block1end   = 0x0002'0000; // 0x0002'000 bytes, 131'072 bytes, 43'690.666 samples, 2 bytes wasted
static const std::uint32_t s_block2start = 0x0002'0000;
static const std::uint32_t s_block2end   = 0x0004'0000; // 0x0002'000 bytes, 131'072 bytes, 43'690.666 samples, 2 bytes wasted
static const std::uint32_t s_block3start = 0x0004'0000;
static const std::uint32_t s_block3end   = 0x0006'0000; // 0x0002'000 bytes, 131'072 bytes, 43'690.666 samples, 2 bytes wasted
static const std::uint32_t s_block4start = 0x0006'0000;
static const std::uint32_t s_block4end   = 0x0007'FFD3; // 0x0001'FFD3 bytes, 131'027 bytes, 43'675.666 samples, 2 bytes wasted
static const std::uint32_t s_calib_start = 0x0007'FFD3;
static const std::uint32_t s_calib_end   = 0x0007'FFFD; // 0x2A bytes, 42 bytes, 14 samples
static const std::uint32_t s_checksum_s  = 0x0007'FFFD;
static const std::uint32_t s_checksum_e  = 0x0007'FFFE; // 1 byte
static const std::uint32_t s_flashend    = 0x0008'0000; // 2 bytes wasted
static const int s_block1samples = (s_block1end - s_block1start) / 3;
static const int s_block2samples = (s_block2end - s_block2start) / 3;
static const int s_block3samples = (s_block3end - s_block3start) / 3;
static const int s_block4samples = (s_block4end - s_block4start) / 3;
static const int s_calibsamples = (s_calib_end - s_calib_start) / 3;


int txt2bin_impl(wchar_t const* const& block1_file_name, wchar_t const* const& block2_file_name, wchar_t const* const& block3_file_name, wchar_t const* const& block4_file_name, wchar_t const* const& cal_file_name, wchar_t const* const& eeprom_file_name)
{
	HANDLE const block1_file_handle = CreateFileW(block1_file_name, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	VERIFY(block1_file_handle != INVALID_HANDLE_VALUE);
	auto block1_file_close_fn = mk::make_scope_exit([&]()
	{
		BOOL const ret = CloseHandle(block1_file_handle);
		VERIFY(ret != 0);
	});
	HANDLE const block2_file_handle = CreateFileW(block2_file_name, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	VERIFY(block2_file_handle != INVALID_HANDLE_VALUE);
	auto block2_file_close_fn = mk::make_scope_exit([&]()
	{
		BOOL const ret = CloseHandle(block2_file_handle);
		VERIFY(ret != 0);
	});
	HANDLE const block3_file_handle = CreateFileW(block3_file_name, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	VERIFY(block3_file_handle != INVALID_HANDLE_VALUE);
	auto block3_file_close_fn = mk::make_scope_exit([&]()
	{
		BOOL const ret = CloseHandle(block3_file_handle);
		VERIFY(ret != 0);
	});
	HANDLE const block4_file_handle = CreateFileW(block4_file_name, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	VERIFY(block4_file_handle != INVALID_HANDLE_VALUE);
	auto block4_file_close_fn = mk::make_scope_exit([&]()
	{
		BOOL const ret = CloseHandle(block4_file_handle);
		VERIFY(ret != 0);
	});
	HANDLE const cal_file_handle = CreateFileW(cal_file_name, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	VERIFY(cal_file_handle != INVALID_HANDLE_VALUE);
	auto cal_file_close_fn = mk::make_scope_exit([&]()
	{
		BOOL const ret = CloseHandle(cal_file_handle);
		VERIFY(ret != 0);
	});
	HANDLE const eeprom_file_handle = CreateFileW(eeprom_file_name, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	VERIFY(eeprom_file_handle != INVALID_HANDLE_VALUE);
	auto eeprom_file_close_fn = mk::make_scope_exit([&]()
	{
		BOOL const ret = CloseHandle(eeprom_file_handle);
		VERIFY(ret != 0);
	});

	auto const loc = std::setlocale(LC_ALL, "C");
	VERIFY(loc);

	struct block_handle_start_end
	{
		HANDLE m_handle;
		std::uint32_t start_addr;
		std::uint32_t end_addr;
	};

	block_handle_start_end const blocks[] =
	{
		{block1_file_handle, s_block1start, s_block1end},
		{block2_file_handle, s_block2start, s_block2end},
		{block3_file_handle, s_block3start, s_block3end},
		{block4_file_handle, s_block4start, s_block4end},
	};

	windows_file_byte_writer byte_writer(eeprom_file_handle);
	auto wrtr = buffered_writer(writer(std::ref(byte_writer)));
	std::uint32_t address = 0;
	for(auto const& block : blocks)
	{
		VERIFY(address == block.start_addr);
		windows_file_byte_reader file_byte_reader(block.m_handle);
		auto file_line_reader = make_line_reader(file_byte_reader);
		line_view file_line;
		erange current_range = s_default_range;
		erate current_rate = s_default_rate;
		digital_io current_dio = s_default_dio;
		std::array<std::uint8_t, 3> sample;
		while(file_line_reader.read_line(file_line))
		{
			if(!create_sample(file_line, sample, current_range, current_rate, current_dio))
			{
				continue;
			}
			wrtr.write_bytes(sample.data(), static_cast<int>(sample.size()));
			address += static_cast<std::uint32_t>(sample.size());
			VERIFY(address <= block.end_addr);
		}
		// here is -1 probably because somebody screw up and made off-by-one error in validator3.exe
		for(int i = address; i != block.end_addr - 1; ++i)
		{
			std::uint8_t const byte = s_default_byte;
			wrtr.write_bytes(&byte, 1);
			++address;
			VERIFY(address <= block.end_addr);
		}
		// here is 0x00 instead of 0xFF because screwed validator3.exe
		if(address <= block.end_addr)
		{
			// but surprise, surprise, end of block 4 is not screwed
			std::uint8_t const byte = block.end_addr == s_block4end ? s_default_byte : 0x00;
			wrtr.write_bytes(&byte, 1);
			++address;
			VERIFY(address <= block.end_addr);
		}
		VERIFY(address == block.end_addr);
	}

	windows_file_byte_reader cal_file_byte_reader(cal_file_handle);
	auto cal_file_line_reader = make_line_reader(cal_file_byte_reader);
	line_view file_line;
	int cal_lines = 0;
	std::uint8_t cal_checksum = 0;
	while(cal_file_line_reader.read_line(file_line))
	{
		if(file_line.size() == 0)
		{
			continue;
		}
		long const val = string_to_long(file_line);
		VERIFY(val >= 0x0000'0000 && val <= 0x00FF'FFFF);
		std::array<std::uint8_t, 3> sample
		{
			(val >> 0) & 0xFFu,
			(val >> 8) & 0xFFu,
			(val >> 16) & 0xFFu
		};
		cal_checksum += sample[0];
		cal_checksum += sample[1];
		cal_checksum += sample[2];
		wrtr.write_bytes(sample.data(), static_cast<int>(sample.size()));
		address += static_cast<std::uint32_t>(sample.size());
		++cal_lines;
		VERIFY(address <= s_calib_end);
		VERIFY(cal_lines <= s_calibsamples);
	}
	VERIFY(address == s_calib_end);
	VERIFY(cal_lines == s_calibsamples);
	VERIFY(address == s_checksum_s);
	cal_checksum = ~cal_checksum + 2; // Constant +2 is empirically proved to be correct.
	wrtr.write_bytes(&cal_checksum, 1);
	++address;
	VERIFY(address == s_checksum_e);
	// here is -1 probably because somebody screw up and made off-by-one error in validator3.exe
	for(std::uint32_t i = address; i != s_flashend - 1; ++i)
	{
		std::uint8_t const byte = s_default_byte;
		wrtr.write_bytes(&byte, 1);
		++address;
		VERIFY(address <= s_flashend);
	}
	// here is 0x00 instead of 0xFF because screwed validator3.exe
	if(address <= s_flashend)
	{
		std::uint8_t const byte = 0x00;
		wrtr.write_bytes(&byte, 1);
		++address;
		VERIFY(address <= s_flashend);
	}
	VERIFY(address == s_flashend);

	return EXIT_SUCCESS;
}

bool create_sample(line_view const& file_line, std::array<std::uint8_t, 3>& sample, erange& current_range, erate& current_rate, digital_io& current_dio)
{
	if(file_line.size() == 0)
	{
		return false;
	}
	if(cbegin(file_line)[0] == '*')
	{
		return false;
	}
	
	if(cbegin(file_line)[0] == 'R')
	{
		long const val = string_to_long(line_view{cbegin(file_line) + 1, file_line.size() - 1});
		VERIFY(val == 1 || val == 10 || val == 100);
		switch(val)
		{
		case 10: current_range = erange::range10; break;
		case 1: current_range = erange::range1; break;
		case 100: current_range = erange::range01; break;
		}
		std::array<std::uint8_t, 3> ret;
		ret[0] = control_byte(current_range, current_rate, current_dio);
		ret[1] = s_control_byte;
		ret[2] = s_default_byte;
		sample = ret;
		return true;
	}

	if(cbegin(file_line)[0] == 'F')
	{
		long const val = string_to_long(line_view{cbegin(file_line) + 1, file_line.size() - 1});
		VERIFY(val == 10 || val == 25 || val == 50 || val == 100);
		switch(val)
		{
		case 10: current_rate = erate::rate10; break;
		case 25: current_rate = erate::rate25; break;
		case 50: current_rate = erate::rate50; break;
		case 100: current_rate = erate::rate100; break;
		}
		std::array<std::uint8_t, 3> ret;
		ret[0] = control_byte(current_range, current_rate, current_dio);
		ret[1] = s_control_byte;
		ret[2] = s_default_byte;
		sample = ret;
		return true;
	}

	if(cbegin(file_line)[0] == 'D')
	{
		long const val = string_to_long(line_view{cbegin(file_line) + 1, file_line.size() - 1});
		VERIFY(val >= 0x00 && val <= 0x0F);
		current_dio = digital_io{static_cast<std::uint8_t>(val)};
		std::array<std::uint8_t, 3> ret;
		ret[0] = control_byte(current_range, current_rate, current_dio);
		ret[1] = s_control_byte;
		ret[2] = s_default_byte;
		sample = ret;
		return true;
	}

	if(cbegin(file_line)[0] == 'J')
	{
		long const val = string_to_long(line_view{cbegin(file_line) + 1, file_line.size() - 1});
		VERIFY(val >= 1 && val <= 4);
		std::array<std::uint8_t, 3> ret;
		ret[0] = static_cast<std::uint8_t>(val);
		ret[1] = s_jump_byte;
		ret[2] = s_default_byte;
		sample = ret;
		return true;
	}

	double const val = string_to_double(file_line);
	volts_10 const volts10 = convert_to_10_volts(val, current_range);
	sample = convert_to_volts_binary(volts10);
	return true;
}

std::uint8_t control_byte(erange const& range, erate const& rate, digital_io const& dio)
{
	std::uint8_t ret = 0;
	ret |= s_ranges[static_cast<std::uint8_t>(range)];
	ret |= s_rates[static_cast<std::uint8_t>(rate)];
	ret |= dio.get();
	return ret;
}

volts_10 convert_to_10_volts(double const& value, erange const& type)
{
	return volts_10{(s_volts_10_nominal / s_volts_conversion_table[static_cast<std::uint8_t>(type)]) * value};
}

double round_to_nearest_ties_to_even(double const& d)
{
  return d - std::remainder(d, 1.0);
}

std::array<std::uint8_t, 3> convert_to_volts_binary(volts_10 const& v)
{
	std::uint32_t bits = static_cast<std::uint32_t>((std::min)(s_20bits_as_double, round_to_nearest_ties_to_even((s_20bits_as_double / s_max10volts) * std::abs(v.get()))));
	if(v.get() < 0.0)
	{
		bits |= s_minus_bit;
	}
	bits &= s_21bits_mask;
	return std::array<std::uint8_t, 3>
	{
		(bits >> 0) & 0xFFu,
		(bits >> 8) & 0xFFu,
		(bits >> 16) & 0xFFu
	};
}

#endif
