#pragma once
#ifndef srec_writer_inl_included
#define srec_writer_inl_included


#include "srec_writer.h"

#include <algorithm> // std::min std::copy
#include <iterator> // std::begin std::cbegin std::cend
#include <numeric> // std::accumulate


static char const s_hex_alphabet[] = "0123456789ABCDEF";
static std::uint8_t const s_CR = 0x0D;
static std::uint8_t const s_LF = 0x0A;


template<typename T>
srec_writer<T>::srec_writer(T& byte_writer) :
	m_byte_writer(byte_writer),
	m_idx(),
	m_addr(),
	m_buffer()
{
}

template<typename T>
void srec_writer<T>::write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size)
{
	std::uint32_t const size1 = (std::min)(static_cast<std::uint32_t>(m_buffer.size()) - m_idx, size);
	std::uint32_t const size2 = size - size1;
	std::copy(buffer, buffer + size1, begin(m_buffer) + m_idx);
	m_idx += size1;
	if(m_idx == s_srec_writer_bytes_per_line)
	{
		std::array<std::uint8_t, 2 + 1 * 2 + 3 * 2 + s_srec_writer_bytes_per_line * 2 + 1 * 2 + 2> buff;
		buff[0] = 'S';
		buff[1] = '2';
		buff[2] = s_hex_alphabet[((3 + s_srec_writer_bytes_per_line + 1) >> 4) & 0x0F];
		buff[3] = s_hex_alphabet[((3 + s_srec_writer_bytes_per_line + 1) >> 0) & 0x0F];
		buff[4] = s_hex_alphabet[(m_addr >> (4 * 5)) & 0x0F];
		buff[5] = s_hex_alphabet[(m_addr >> (4 * 4)) & 0x0F];
		buff[6] = s_hex_alphabet[(m_addr >> (4 * 3)) & 0x0F];
		buff[7] = s_hex_alphabet[(m_addr >> (4 * 2)) & 0x0F];
		buff[8] = s_hex_alphabet[(m_addr >> (4 * 1)) & 0x0F];
		buff[9] = s_hex_alphabet[(m_addr >> (4 * 0)) & 0x0F];
		for(std::uint32_t i = 0; i != s_srec_writer_bytes_per_line; ++i)
		{
			buff[10 + i * 2 + 0] = s_hex_alphabet[(m_buffer[i] >> 4) & 0x0F];
			buff[10 + i * 2 + 1] = s_hex_alphabet[(m_buffer[i] >> 0) & 0x0F];
		}
		std::uint8_t checksum = 0;
		checksum += 3 + s_srec_writer_bytes_per_line + 1;
		checksum += (m_addr >> (8 * 2)) & 0xFF;
		checksum += (m_addr >> (8 * 1)) & 0xFF;
		checksum += (m_addr >> (8 * 0)) & 0xFF;
		checksum += std::accumulate(cbegin(m_buffer), cend(m_buffer), std::uint8_t{0});
		checksum = ~checksum;
		buff[10 + s_srec_writer_bytes_per_line * 2 + 0] = s_hex_alphabet[(checksum >> 4) & 0x0F];
		buff[10 + s_srec_writer_bytes_per_line * 2 + 1] = s_hex_alphabet[(checksum >> 0) & 0x0F];
		buff[10 + s_srec_writer_bytes_per_line * 2 + 1 * 2 + 0] = s_CR;
		buff[10 + s_srec_writer_bytes_per_line * 2 + 1 * 2 + 1] = s_LF;
		m_byte_writer.write_bytes(buff.data(), buff.size());
		m_idx = 0;
		m_addr += s_srec_writer_bytes_per_line;
		return write_bytes(buffer + size1, size2);
	}
}


template<typename T>
srec_writer<T> make_srec_writer(T& byte_writer)
{
	return srec_writer<T>{byte_writer};
}


#endif
