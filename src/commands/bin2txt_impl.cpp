#include "c:/dev/git/github/joboccara/NamedType/named_type.hpp"

#include "../utils/verify.h"
#include "../utils/scope_exit.h"

#include <array>
#include <algorithm>
#include <numeric>
#include <string>

#include <fcntl.h> // _O_BINARY
#include <io.h> // _setmode
#include <stdio.h> // _fileno

#include <windows.h>

#if 0
using digital_io = fluent::NamedType<std::uint8_t, struct digital_io_tag, fluent::Comparable>;

using volts_10 = fluent::NamedType<double, struct volts_10_tag>;
using volts_1 = fluent::NamedType<double, struct volts_1_tag>;
using volts_01 = fluent::NamedType<double, struct volts_01_tag>;
using volts_b = std::array<std::uint8_t, 3>;


enum class erange : std::uint8_t
{
	range10,
	range1,
	range01
};

enum class erate : std::uint8_t
{
	rate10,
	rate25,
	rate50,
	rate100
};


static const erange s_default_range = erange::range10;
static const erate s_default_rate = erate::rate10;
static const digital_io s_default_dio = digital_io{0x0F};

static const double s_volts_10_nominal = 10.0;
static const double s_volts_1_nominal = 1.414;
static const double s_volts_01_nominal = 0.157;
static const double s_d20bits = static_cast<double>(0x000F'FFFFu);
static const double s_max10volts = 10.357;
static const std::uint32_t s_minus_bit = 0x0010'0000u; // only bit 21 is set
static const std::uint32_t s_21bits_mask = 0x001F'FFFFu;
static const double s_20bits_as_double = static_cast<double>(0x000F'FFFF);
static const double s_volts_conversion_table[] = {s_volts_10_nominal, s_volts_1_nominal, s_volts_01_nominal};
static const std::uint8_t s_jump_byte = 0xFDu;
static const std::uint8_t s_control_byte = 0xFEu;
static const std::uint8_t s_default_byte = 0xFF;
static const std::uint8_t s_ranges[] = {0b0000'0000, 0b0001'0000, 0b0010'0000};
static const std::uint8_t s_rates[]  = {0b0000'0000, 0b0100'0000, 0b1000'0000, 0b1100'0000};
static const std::uint8_t s_digital_out_mask = 0b0000'1111;
static const std::uint8_t s_range_mask       = 0b0011'0000;
static const std::uint8_t s_rate_mask        = 0b1100'0000;

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


volts_10 convert_to_volts_10(volts_10 const& v)
{
	return volts_10((s_volts_10_nominal / s_volts_10_nominal) * v.get());
}

volts_10 convert_to_volts_10(volts_1 const& v)
{
	return volts_10((s_volts_10_nominal / s_volts_1_nominal) * v.get());
}

volts_10 convert_to_volts_10(volts_01 const& v)
{
	return volts_10((s_volts_10_nominal / s_volts_01_nominal) * v.get());
}

volts_1 convert_to_volts_1(volts_10 const& v)
{
	return volts_1((s_volts_1_nominal / s_volts_10_nominal) * v.get());
}

volts_1 convert_to_volts_1(volts_1 const& v)
{
	return volts_1((s_volts_1_nominal / s_volts_1_nominal) * v.get());
}

volts_1 convert_to_volts_1(volts_01 const& v)
{
	return volts_1((s_volts_1_nominal / s_volts_01_nominal) * v.get());
}

volts_01 convert_to_volts_01(volts_10 const& v)
{
	return volts_01((s_volts_01_nominal / s_volts_10_nominal) * v.get());
}

volts_01 convert_to_volts_01(volts_1 const& v)
{
	return volts_01((s_volts_01_nominal / s_volts_1_nominal) * v.get());
}

volts_01 convert_to_volts_01(volts_01 const& v)
{
	return volts_01((s_volts_01_nominal / s_volts_01_nominal) * v.get());
}

double round_to_nearest_ties_to_even(double const& d);
double round_to_nearest_ties_to_even(double const& d)
{
  return d - std::remainder(d, 1.0);
}

std::uint8_t control_byte(erange const& range, erate const& rate, std::uint8_t const& digital_out)
{
	std::uint8_t ret = 0;
	ret |= s_ranges[static_cast<std::uint8_t>(range)];
	ret |= s_rates[static_cast<std::uint8_t>(rate)];
	ret |= digital_out;
	return ret;
}

volts_b convert_to_volts_b(volts_10 const& v)
{
	std::uint32_t bits = static_cast<std::uint32_t>((std::min)(s_d20bits, round_to_nearest_ties_to_even((s_d20bits / s_max10volts) * std::abs(v.get()))));
	if(v.get() < 0.0)
	{
		bits |= s_minus_bit;
	}
	bits &= s_21bits_mask;
	return volts_b
	{
		(bits >> 0) & 0xFFu,
		(bits >> 8) & 0xFFu,
		(bits >> 16) & 0xFFu
	};
}


typedef std::array<std::uint8_t, 3> sample;

volts_10 convert_from_volts_binary(sample const& s)
{
	std::uint32_t const val_uint =
		(static_cast<std::uint32_t>(s[0]) << 0) |
		(static_cast<std::uint32_t>(s[1]) << 8) |
		(static_cast<std::uint32_t>(s[2]) << 16);
	VERIFY((val_uint & ~s_21bits_mask) == 0);
	bool const negative = (val_uint & s_minus_bit) == s_minus_bit;
	std::uint32_t const val_uint_abs = val_uint & ~s_minus_bit;
	double const val_dbl_abs = static_cast<double>(val_uint_abs) / (s_20bits_as_double / s_max10volts);
	double const val_dbl = negative ? -val_dbl_abs : val_dbl_abs;
	return volts_10{val_dbl};
}

double convert_from_10_volts(volts_10 const& value, erange const& type)
{
	return value.get() / (s_volts_10_nominal / s_volts_conversion_table[static_cast<std::uint8_t>(type)]);
}

bool process_sample(sample const& s, erange& current_range, erate& current_rate, digital_io& current_dio, std::string& out)
{
	if(s[0] == 0xFF && s[1] == 0xFF && s[2] == 0xFF)
	{
		return false;
	}
	else if(s[1] == s_control_byte && s[2] == s_default_byte)
	{
		VERIFY((s[0] & 0b0011'0000) != 0b0011'0000);
		auto const new_dio = static_cast<digital_io>((s[0] >> 0) & 0x0F);
		auto const new_range = static_cast<erange>  ((s[0] >> 4) & 0x03);
		auto const new_rate = static_cast<erate>    ((s[0] >> 6) & 0x03);
		VERIFY
		(
			(new_dio == current_dio && new_range == current_range && new_rate == current_rate) ||
			(new_dio != current_dio && new_range == current_range && new_rate == current_rate) ||
			(new_dio == current_dio && new_range != current_range && new_rate == current_rate) ||
			(new_dio == current_dio && new_range == current_range && new_rate != current_rate)
		);
		if(new_dio != current_dio)
		{
			current_dio = new_dio;
			out.resize(6);
			std::snprintf(const_cast<char*>(out.data()), 6, "D%hhu\x0D\x0A", new_dio.get());
			return true;
		}
		else if(new_range != current_range)
		{
			current_range = new_range;
			switch(current_range)
			{
			case erange::range10: out = "R10\x0D\x0A"; return true;
			case erange::range1: out = "R1\x0D\x0A"; return true;
			case erange::range01: out = "R100\x0D\x0A"; return true;
			}
			VERIFY(false);
		}
		else if(new_rate != current_rate)
		{
			current_rate = new_rate;
			switch(current_rate)
			{
			case erate::rate10: out = "F10\x0D\x0A"; return true;
			case erate::rate25: out = "F25\x0D\x0A"; return true;
			case erate::rate50: out = "F50\x0D\x0A"; return true;
			case erate::rate100: out = "F100\x0D\x0A"; return true;
			}
			VERIFY(false);
		}
		else
		{
			VERIFY(new_dio == current_dio && new_range == current_range && new_rate == current_rate);
			current_dio = new_dio;
			out.resize(6);
			std::snprintf(const_cast<char*>(out.data()), 6, "D%hhu\x0D\x0A", new_dio.get());
			return true;
		}
	}
	else if(s[1] == s_jump_byte && s[2] == s_default_byte)
	{
		VERIFY(s[0] >= 0 && s[0] <= 4);
		std::uint8_t const jump = s[0];
		switch(jump)
		{
		case 1: out = "J1\x0D\x0A"; return true;
		case 2: out = "J2\x0D\x0A"; return true;
		case 3: out = "J3\x0D\x0A"; return true;
		case 4: out = "J4\x0D\x0A"; return true;
		}
		VERIFY(false);
	}
	else
	{
		if(s[0] == 0x00 && s[1] == 0x00 && s[2] == 0x10)
		{
			// negative zero
			out = "-1.0e-100\x0D\x0A";
			return true;
		}
		else
		{
			volts_10 const volts10 = convert_from_volts_binary(s);
			double const volts = convert_from_10_volts(volts10, current_range);
			out.resize(256);
			std::snprintf(const_cast<char*>(out.data()), 256, "%.100g\x0D\x0A", volts);
			return true;
		}
	}
	out.clear();
	return true;
}


int Bin2Txt()
{
	std::FILE* const in = stdin;

	int const set_mode_ret1 = _setmode(_fileno(in), _O_BINARY);
	VERIFY(set_mode_ret1 != 1 && errno != EBADF && errno != EINVAL);

	HANDLE const out1 = CreateFileW(L"out_1.txt", GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	VERIFY(out1 != INVALID_HANDLE_VALUE);
	auto fn_out1 = mk::make_scope_exit([&]()
	{
		BOOL const ret = CloseHandle(out1);
		VERIFY(ret != 0);
	});
	HANDLE const out2 = CreateFileW(L"out_2.txt", GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	VERIFY(out2 != INVALID_HANDLE_VALUE);
	auto fn_out2 = mk::make_scope_exit([&]()
	{
		BOOL const ret = CloseHandle(out2);
		VERIFY(ret != 0);
	});
	HANDLE const out3 = CreateFileW(L"out_3.txt", GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	VERIFY(out3 != INVALID_HANDLE_VALUE);
	auto fn_out3 = mk::make_scope_exit([&]()
	{
		BOOL const ret = CloseHandle(out3);
		VERIFY(ret != 0);
	});
	HANDLE const out4 = CreateFileW(L"out_4.txt", GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	VERIFY(out4 != INVALID_HANDLE_VALUE);
	auto fn_out4 = mk::make_scope_exit([&]()
	{
		BOOL const ret = CloseHandle(out4);
		VERIFY(ret != 0);
	});
	HANDLE const outc = CreateFileW(L"out_c.txt", GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	VERIFY(outc != INVALID_HANDLE_VALUE);
	auto fn_outc = mk::make_scope_exit([&]()
	{
		BOOL const ret = CloseHandle(outc);
		VERIFY(ret != 0);
	});

	struct handle_block_start_end
	{
		HANDLE handle;
		std::uint32_t start_addr;
		std::uint32_t end_addr;
	};

	handle_block_start_end const blocks[] =
	{
		{out1, s_block1start, s_block1end},
		{out2, s_block2start, s_block2end},
		{out3, s_block3start, s_block3end},
		{out4, s_block4start, s_block4end},
	};

	std::string str;
	std::uint32_t address = 0;
	for(auto const& block : blocks)
	{
		erange current_range = s_default_range;
		erate current_rate = s_default_rate;
		digital_io current_dio = s_default_dio;

		VERIFY(address == block.start_addr);
		for(;;)
		{
			sample s;
			fread(s.data(), 1, sizeof(s), in);
			address += sizeof(sample);
			if(!process_sample(s, current_range, current_rate, current_dio, str))
			{
				break;
			}
			if(!str.empty())
			{
				auto const len = static_cast<std::uint32_t>(std::strlen(str.data()));
				DWORD written;
				BOOL const ret = WriteFile(block.handle, str.data(), len, &written, nullptr);
				VERIFY(ret != 0);
				VERIFY(written == len);
			}
			if(address == block.end_addr - 2)
			{
				break;
			}
		}
		for(;;)
		{
			sample s;
			fread(s.data(), 1, sizeof(s), in);
			address += sizeof(sample);
			VERIFY(s[0] == 0xFF && s[1] == 0xFF && s[2] == 0xFF);
			if(address == block.end_addr - 2)
			{
				break;
			}
		}
		std::uint8_t a;
		fread(&a, 1, 1, in);
		++address;
		VERIFY(a == 0xFF);
		std::uint8_t b;
		fread(&b, 1, 1, in);
		++address;
		VERIFY(b == 0x00 || b == 0xFF);
		VERIFY(address == block.end_addr);
	}
	VERIFY(address == s_calib_start);
	std::uint8_t cal_checksum_computed = 0;
	for(;;)
	{
		sample s;
		fread(s.data(), 1, sizeof(s), in);
		address += sizeof(s);
		cal_checksum_computed += std::accumulate(cbegin(s), cend(s), std::uint8_t{0});
		std::uint32_t const val_uint =
			(static_cast<std::uint32_t>(s[0]) << 0) |
			(static_cast<std::uint32_t>(s[1]) << 8) |
			(static_cast<std::uint32_t>(s[2]) << 16);
		str.resize(15);
		static_assert(std::is_same_v<std::uint32_t, unsigned>, "");
		std::snprintf(const_cast<char*>(str.data()), 15, "%u\x0D\x0A", val_uint);
		{
			auto const len = static_cast<std::uint32_t>(std::strlen(str.data()));
			DWORD written;
			BOOL const ret = WriteFile(outc, str.data(), len, &written, nullptr);
			VERIFY(ret != 0);
			VERIFY(written == len);
		}
		if(address == s_calib_end)
		{
			break;
		}
	}
	cal_checksum_computed = ~cal_checksum_computed + 2;
	std::uint8_t cal_checksum_read;
	VERIFY(address == s_checksum_s);
	fread(&cal_checksum_read, 1, 1, in);
	++address;
	VERIFY(address == s_checksum_e);
	VERIFY(cal_checksum_computed == cal_checksum_read);

	std::uint8_t a;
	fread(&a, 1, 1, in);
	++address;
	VERIFY(a == 0xFF);
	std::uint8_t b;
	fread(&b, 1, 1, in);
	++address;
	VERIFY(b == 0x00 || b == 0xFF);
	VERIFY(address == s_flashend);

	return 0;
}
#endif









#include "../io/windows_file.h"
#include "../io/windows_file_byte_reader.h"
#include "../io/windows_file_byte_writer.h"
#include "../io/buffered_reader.h"
#include "../io/buffered_writer.h"
#include "../io/txt_writer.h"
#include "../io/writer.h"


int bin2txt_impl(wchar_t const* const& input_file_name, wchar_t const* const& output_bank1_file_name, wchar_t const* const& output_bank2_file_name, wchar_t const* const& output_bank3_file_name, wchar_t const* const& output_bank4_file_name, wchar_t const* const& output_calibration_file_name)
{
#if 0
	auto input_file = windows_file::make_for_input_redirectable(input_file_name);
	auto wfbr = windows_file_byte_reader{input_file.get_handle()};
	auto reader = make_buffered_reader(wfbr);

	auto output_file = windows_file::make_for_output_redirectable(output_bank1_file_name);
	auto wfbw = windows_file_byte_writer{output_file.get_handle()};
	auto bw = buffered_writer(wfbw);
	auto wrtr = txt_writer(writer{std::move(bw)});



	//auto a = writer{buffered_writer<windows_file_byte_writer>{windows_file_byte_writer{HANDLE{}}}};
	//auto pa = new writer{buffered_writer<windows_file_byte_writer>{windows_file_byte_writer{HANDLE{}}}};
	//auto p2 = new writer[2]{buffered_writer<windows_file_byte_writer>{windows_file_byte_writer{HANDLE{}}}, buffered_writer<windows_file_byte_writer>{windows_file_byte_writer{HANDLE{}}}};

	auto a = writer{windows_file_byte_writer{HANDLE{}}};
	//auto b = writer{a};
	auto c = writer{writer{windows_file_byte_writer{HANDLE{}}}};
	auto d = writer{};
	auto e = writer{std::ref(std::ref(bw))};
	e.write_bytes(nullptr, 0);
#endif
	(void)input_file_name;
	(void)output_bank1_file_name;
	(void)output_bank2_file_name;
	(void)output_bank3_file_name;
	(void)output_bank4_file_name;
	(void)output_calibration_file_name;

	windows_file_byte_writer wfbw{HANDLE{}};
	buffered_writer bw{writer{std::ref(wfbw)}};

	writer w{std::ref(bw)};
	w.write_bytes(nullptr, 0);
	return EXIT_SUCCESS;
}





class XXX : public buffered_writer
{
};

