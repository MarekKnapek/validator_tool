#include "IO.h"

#include "Utils.h" // VERIFY is_hex
#include "utils/verify.h"

#include <algorithm> // std::all_of
#include <cstdio> // fread


char reader_t<char>::operator()(std::FILE* const& strm) const
{
	char ret;
	VERIFY(fread(&ret, 1, sizeof(ret), strm) == sizeof(ret));
	auto const p = reinterpret_cast<char const*>(std::addressof(ret));
	VERIFY(std::all_of(p, p + sizeof(ret), [](auto const& e){ return is_hex(e); }));
	return ret;
}

hex_byte_t reader_t<hex_byte_t>::operator()(std::FILE* const& strm) const
{
	hex_byte_t ret;
	VERIFY(fread(&ret, 1, sizeof(ret), strm) == sizeof(ret));
	auto const p = reinterpret_cast<char const*>(std::addressof(ret));
	VERIFY(std::all_of(p, p + sizeof(ret), [](auto const& e){ return is_hex(e); }));
	return ret;
}

hex_address24_t reader_t<hex_address24_t>::operator()(std::FILE* const& strm) const
{
	hex_address24_t ret;
	VERIFY(fread(&ret, 1, sizeof(ret), strm) == sizeof(ret));
	auto const p = reinterpret_cast<char const*>(std::addressof(ret));
	VERIFY(std::all_of(p, p + sizeof(ret), [](auto const& e){ return is_hex(e); }));
	return ret;
}
