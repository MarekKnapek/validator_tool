#pragma once
#ifndef line_reader_h_included
#define line_reader_h_included


#include "line_view_fwd.h" // line_view

#include <array> // std::array
#include <cstdint> // std::uint8_t std::uint32_t


static std::uint32_t const s_line_reader_buffer_size = 1024;
static std::uint32_t const s_line_reader_buffer_align = 4 * 1024;


template<typename T>
class line_reader
{
public:
	line_reader(T& byte_reader);
	bool read_line(/* out */ line_view& line);
private:
	T& m_byte_reader;
	__declspec(align(s_line_reader_buffer_align)) std::array<std::uint8_t, s_line_reader_buffer_size * 2> m_buffer;
	std::uint32_t m_idx;
	std::uint32_t m_end;
};

template<typename T>
line_reader<T> make_line_reader(T& byte_reader);


#include "line_reader.inl"


#endif
