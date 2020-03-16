#pragma once
#ifndef srec_reader_inl_included
#define srec_reader_inl_included


#include "srec_reader.h"

#include "../utils/verify.h" // VERIFY
#include "line_view.h" // line_view
#include "../utils/checked_iterator.h" // make_checked_iterator

#include <algorithm> // std::min std::copy
#include <numeric> // std::accumulate


template<typename T>
srec_reader<T>::srec_reader(T& line_reader) :
	m_buffer(),
	m_idx(),
	m_end(),
	m_address(),
	m_line_reader(line_reader)
{
}

template<typename T>
std::uint32_t srec_reader<T>::read_bytes(std::uint8_t* const& buffer, std::uint32_t const& size)
{
	std::uint32_t const size1 = (std::min)(m_end - m_idx, size);
	std::uint32_t const size2 = size - size1;
	std::copy(cbegin(m_buffer) + m_idx, cbegin(m_buffer) + m_idx + size1, make_checked_iterator(buffer, size));
	m_idx += size1;
	std::uint32_t ret = size1;
	if(size2 != 0)
	{
		line_view line;
		if(!m_line_reader.read_line(line))
		{
			return ret;
		}
		VERIFY(line.size() >= 2 + 1 * 2 + 3 * 2 + 1 * 2 + 1 * 2); // +2 for header, +1*2 for size, +3*2 for address, +1*2 for data, +1*2 for checksum
		VERIFY(cbegin(line)[0] == 'S');
		VERIFY(cbegin(line)[1] == '2'); // Only three bytes long addresses supported.
		std::uint8_t const size_ = from_hex(cbegin(line)[2], cbegin(line)[3]) - 3 - 1; // -3 for address, -1 for checksum
		VERIFY(size_ >= 3 + 1 + 1); // +3 for address, +1 for data, +1 for checksum
		VERIFY(line.size() >= 2u + 1u * 2u + 3u * 2u + size_ * 2u + 1u * 2u); // +2 for header, +1*2 for size, +3*2 for address, +size*2 for data, +1*2 for checksum
		std::uint32_t const address =
			(from_hex(cbegin(line)[4], cbegin(line)[5]) << 16) |
			(from_hex(cbegin(line)[6], cbegin(line)[7]) << 8) |
			(from_hex(cbegin(line)[8], cbegin(line)[9]) << 0);
		VERIFY(address == m_address);
		for(std::uint8_t i = 0; i != size_; ++i)
		{
			m_buffer[i] = from_hex
			(
				cbegin(line)[10 + i * 2 + 0],
				cbegin(line)[10 + i * 2 + 1]
			);
		}
		std::uint8_t const checksum_read = from_hex
		(
			cbegin(line)[10 + size_ * 2 + 0],
			cbegin(line)[10 + size_ * 2 + 1]
		);
		std::uint8_t checksum_computed = 0;
		checksum_computed += 3 + size_ + 1; // +3 for address, +1 for checksum
		checksum_computed += (address >> 16) & 0xFF;
		checksum_computed += (address >> 8) & 0xFF;
		checksum_computed += (address >> 0) & 0xFF;
		checksum_computed += std::accumulate(cbegin(m_buffer), cbegin(m_buffer) + size_, std::uint8_t{0});
		checksum_computed = ~checksum_computed;
		VERIFY(checksum_read == checksum_computed);
		m_address += size_;
		m_idx = 0;
		m_end = size_;
		return ret + read_bytes(buffer + size1, size2);
	}
	return ret;
}

template<typename T>
std::uint8_t srec_reader<T>::from_hex(std::uint8_t const& hex_letter)
{
	std::uint8_t const& h = hex_letter;
	VERIFY((h >= '0' && h <= '9') || (h >= 'A' && h <= 'F') || (h >= 'a' && h <= 'f'));
	std::uint8_t ret = ((h >= '0' && h <= '9') ? (h - '0') : ((h >= 'A' && h <= 'F') ? (10 + (h - 'A')) : (10 + (h - 'a'))));
	VERIFY(ret >= 0 && ret <= 0x0F);
	return ret;
}

template<typename T>
std::uint8_t srec_reader<T>::from_hex(std::uint8_t const& hi, std::uint8_t const& lo)
{
	return
		(from_hex(hi) << 4) |
		(from_hex(lo) << 0);
}


template<typename T>
srec_reader<T> make_srec_reader(T& line_reader)
{
	return srec_reader<T>{line_reader};
}


#endif
