#pragma once
#ifndef buffered_writer_h_included
#define buffered_writer_h_included


#include <array> // std::array
#include <cstdint> // std::uint8_t std::uint32_t


static std::uint32_t const s_buffered_writer_buffer_size = 4 * 1024;
static std::uint32_t const s_buffered_writer_buffer_align = 4 * 1024;


template<typename T>
class buffered_writer
{
public:
	buffered_writer(T& byte_writer);
	~buffered_writer();
	void write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size);
private:
	T& m_byte_writer;
	std::uint32_t m_idx;
	__declspec(align(s_buffered_writer_buffer_align)) std::array<std::uint8_t, s_buffered_writer_buffer_size> m_buffer;
};

template<typename T>
buffered_writer<T> make_buffered_writer(T& byte_writer);


#include "buffered_writer.inl"


#endif
