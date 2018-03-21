#include "IO.h" // read_from
#include "utils/verify.h"
#include "Utils.h" // VERIFY hex_byte_t hex_address24_t hex_to_bin

#include <algorithm> // std::all_of std::transform
#include <array>
#include <cstdint> // std::uint8_t std::uint16_t std::uint32_t
#include <cstdio> // std::FILE stdin stdout fread fwrite
#include <cstdlib> // EXIT_SUCCESS
#include <iterator> // std::cbegin std::begin std::cend
#include <numeric> // std::accumulate

#include <fcntl.h> // _O_BINARY
#include <io.h> // _setmode
#include <stdio.h> // _fileno


int Hex2Bin()
{
	std::FILE* const in = stdin;
	std::FILE* const out = stdout;

	int const set_mode_ret1 = _setmode(_fileno(in), _O_BINARY);
	VERIFY(set_mode_ret1 != 1 && errno != EBADF && errno != EINVAL);
	int const set_mode_ret2 = _setmode(_fileno(out), _O_BINARY);
	VERIFY(set_mode_ret1 != 1 && errno != EBADF && errno != EINVAL);

	std::uint32_t total_bytes = 0;
	for(;;)
	{
		char record_header;
		do
		{
			if(fread(&record_header, 1, sizeof(char), in) != sizeof(char))
			{
				goto out;
			}
		}while(record_header != 'S');
		auto const record_type = read_from<char>(in);
		VERIFY(record_type == '2');
		auto const bytes = hex_to_bin(read_from<hex_byte_t>(in));
		VERIFY(bytes >= 4);
		auto const address24 = hex_to_bin(read_from<hex_address24_t>(in));
		auto const data_len = bytes - 4;
		VERIFY(total_bytes == address24); // always starting at zero, discontinued memory regions not supported
		std::array<hex_byte_t, 0xFF> data_buff;
		VERIFY(fread(data_buff.data(), 1, data_len * sizeof(hex_byte_t), in) == data_len * sizeof(hex_byte_t));
		VERIFY(std::all_of(cbegin(data_buff), cbegin(data_buff) + data_len, [](auto const& e){ return is_hex(e); }));
		std::array<std::uint8_t, 0xFF> bytes_buff;
		std::transform(cbegin(data_buff), cbegin(data_buff) + data_len, begin(bytes_buff), [](auto const& e){ return hex_to_bin(e); });
		std::uint8_t const checksum_shall_be = hex_to_bin(read_from<hex_byte_t>(in));
		std::uint16_t checksum_intermediate = 0;
		checksum_intermediate += bytes;
		checksum_intermediate += address24.m_data[0] + address24.m_data[1] + address24.m_data[2];
		checksum_intermediate += std::accumulate(cbegin(bytes_buff), cbegin(bytes_buff) + data_len, 0);
		std::uint8_t const checksum_is = static_cast<std::uint8_t>((0xFFFFu - checksum_intermediate) & 0xFFu);
		VERIFY(checksum_is == checksum_shall_be);
		VERIFY(fwrite(bytes_buff.data(), 1, data_len, out) == data_len);
		total_bytes += data_len;
	}
out:;
	return EXIT_SUCCESS;
}

int hex2bin(int argc, wchar_t* argv[])
{
	return Hex2Bin();
}
