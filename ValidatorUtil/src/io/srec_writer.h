#pragma once
#ifndef srec_writer_h_included
#define srec_writer_h_included


#include "../utils/type_traits.h" // is_writer

#include <array> // std::array
#include <cstdint> // std::uint8_t std::uint32_t


static std::uint32_t const s_srec_writer_bytes_per_line = 32;


template<typename T>
class srec_writer
{
public:
	srec_writer(T& byte_writer);
	~srec_writer();
	void write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size);
private:
	void write_line();
private:
	std::array<std::uint8_t, s_srec_writer_bytes_per_line> m_buffer;
	std::uint32_t m_idx;
	std::uint32_t m_addr;
	T& m_byte_writer;
};

template<typename T>
srec_writer<T> make_srec_writer(T& byte_writer);

template<typename T>
struct is_writer_trait<srec_writer<T>> : public std::true_type
{
};


#include "srec_writer.inl"


#endif
