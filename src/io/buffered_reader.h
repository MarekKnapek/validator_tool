#pragma once
#ifndef buffered_reader_h_included
#define buffered_reader_h_included


#include <array> // std::array
#include <cstdint> // std::uint8_t std::uint32_t


static std::uint32_t const s_buffered_reader_buffer_size = 4 * 1024;
static std::uint32_t const s_buffered_reader_buffer_align = 4 * 1024;


template<typename T>
class buffered_reader
{
public:
	buffered_reader(T& byte_reader);
	~buffered_reader();
	std::uint32_t read_bytes(std::uint8_t* const& buffer, std::uint32_t const& size);
private:
	__declspec(align(s_buffered_reader_buffer_align)) std::array<std::uint8_t, s_buffered_reader_buffer_size> m_buffer;
	std::uint32_t m_idx;
	std::uint32_t m_size;
	T& m_byte_reader;
};

template<typename T>
buffered_reader<T> make_buffered_reader(T& byte_reader);


#include "buffered_reader.inl"


#endif
