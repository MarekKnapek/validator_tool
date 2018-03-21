#include "IO.h" // write_to
#include "Utils.h" // VERIFY hex_byte_t address24_t bin_to_hex
#include "utils/verify.h"

#include <algorithm> // std::transform
#include <array>
#include <cstdint> // std::uint8_t std::uint16_t
#include <cstdio> // std::FILE stdin stdout fread fwrite
#include <cstdlib> // EXIT_SUCCESS
#include <iterator> // std::begin std::cbegin std::cend std::size
#include <limits> // std::numeric_limits
#include <numeric> // std::accumulate

#include <fcntl.h> // _O_BINARY
#include <io.h> // _setmode
#include <stdio.h> // _fileno


static const unsigned s_elems_per_line = 32;
static const char s_record_header = 'S';
static const char s_record_type = '2';
static const char s_record_footer[] = "\x0D\x0A";
static const auto s_record_footer_len = std::size(s_record_footer) - 1;


int Bin2Hex()
{
	std::FILE* const in = stdin;
	std::FILE* const out = stdout;

	int const set_mode_ret1 = _setmode(_fileno(in), _O_BINARY);
	VERIFY(set_mode_ret1 != 1 && errno != EBADF && errno != EINVAL);
	int const set_mode_ret2 = _setmode(_fileno(out), _O_BINARY);
	VERIFY(set_mode_ret2 != 1 && errno != EBADF && errno != EINVAL);

	address24_t address{};
	for(;;)
	{
		// hex record structure
		// header 1 byte
		// type 1 byte
		// len 1 hex byte, 2 bytes
		// address 3 hex bytes, 6 bytes
		// data 32 hex bytes,  64 bytes
		// checksum 1 hex byte,  2 bytes
		// footer (CRLF) 2 bytes
		// 78 bytes in total
		std::array<std::uint8_t, s_elems_per_line> in_buff;
		std::array<std::uint8_t, 1 + 1 + 1 * 2 + 3 * 2 + s_elems_per_line * 2 + 1 * 2 + s_record_footer_len> out_buff;
		auto const elems_read_ = fread(in_buff.data(), 1, sizeof(in_buff), in);
		VERIFY(elems_read_ <= sizeof(in_buff) && elems_read_ <= (std::numeric_limits<std::uint8_t>::max)());
		auto const elems_read = static_cast<std::uint8_t>(elems_read_);
		if(elems_read == 0)
		{
			break;
		}
		auto out_it = begin(out_buff);
		out_it = write_to(s_record_header, out_it);
		out_it = write_to(s_record_type, out_it);
		out_it = write_to(bin_to_hex(3 + elems_read + 1), out_it); // +3 fro address, +1 for checksum
		out_it = write_to(bin_to_hex(address), out_it);
		std::array<hex_byte_t, s_elems_per_line> hex_data;
		std::transform(cbegin(in_buff), cbegin(in_buff) + elems_read, begin(hex_data), [](auto const& e){ return bin_to_hex(e); });
		out_it = write_to(cbegin(hex_data), cbegin(hex_data) + elems_read, out_it);
		std::uint16_t checksum_intermediate = 0;
		checksum_intermediate += 3 + elems_read + 1; // +3 fro address, +1 for checksum
		checksum_intermediate += address.m_data[0] + address.m_data[1] + address.m_data[2];
		checksum_intermediate += std::accumulate(cbegin(in_buff), cbegin(in_buff) + elems_read, 0);
		std::uint8_t const checksum_is = static_cast<std::uint8_t>((0xFFFFu - checksum_intermediate) & 0xFFu);
		out_it = write_to(bin_to_hex(checksum_is), out_it);
		out_it = write_to(std::cbegin(s_record_footer), std::cbegin(s_record_footer) + s_record_footer_len, out_it);
		auto const bytes_to_write = out_it - cbegin(out_buff);
		VERIFY(fwrite(out_buff.data(), 1, bytes_to_write, out) == bytes_to_write);
		address += elems_read;
		if(elems_read != s_elems_per_line)
		{
			break;
		}
	}
	return EXIT_SUCCESS;
}

int bin2hex(int argc, wchar_t* argv[])
{
	return Bin2Hex();
}
