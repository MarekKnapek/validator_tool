#pragma once
#ifndef srec_reader_h_included
#define srec_reader_h_included


#include <array> // std::array
#include <cstdint> // std::uint8_t std::uint32_t


template<typename T>
class srec_reader
{
public:
	srec_reader(T& line_reader);
	std::uint32_t read_bytes(std::uint8_t* const& buffer, std::uint32_t const& size);
private:
	static std::uint8_t from_hex(std::uint8_t const& hex_letter);
	static std::uint8_t from_hex(std::uint8_t const& hi, std::uint8_t const& lo);
private:
	std::array<std::uint8_t, 0xFF> m_buffer;
	std::uint32_t m_idx;
	std::uint32_t m_end;
	std::uint32_t m_address;
	T& m_line_reader;
};

template<typename T>
srec_reader<T> make_srec_reader(T& line_reader);


#include "srec_reader.inl"


#endif
