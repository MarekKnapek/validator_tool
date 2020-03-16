#pragma once
#ifndef fake_byte_reader_h_included
#define fake_byte_reader_h_included


#include <cstdint> // std::uint8_t std::uint32_t


class fake_byte_reader
{
public:
	fake_byte_reader(std::uint8_t const* const& data);
	std::uint32_t read_bytes(std::uint8_t* const& buffer, std::uint32_t const& size);
private:
	std::uint8_t const* const m_data;
	std::uint32_t const m_size;
	std::uint32_t m_idx;
};


#endif
