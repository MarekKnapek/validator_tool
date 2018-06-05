#pragma once
#ifndef copy_inl_included
#define copy_inl_included


#include "copy.h"

#include <cstdint> // std::uint8_t std::uint32_t
#include <array> // std::array


template<typename T, typename U, unsigned N>
void copy(T& reader, U& writer)
{
	for(;;)
	{
		__declspec(align(N)) std::array<std::uint8_t, N> buff;
		std::uint32_t const bytes_read = reader.read_bytes(buff.data(), static_cast<std::uint32_t>(buff.size()));
		if(bytes_read == 0)
		{
			break;
		}
		writer.write_bytes(buff.data(), bytes_read);
	}
}


#endif
