#pragma once
#ifndef buffered_reader_inl_included
#define buffered_reader_inl_included


#include "buffered_reader.h"
#include "../utils/checked_iterator.h"

#include <algorithm> // std::min std::copy


template<typename T>
buffered_reader<T>::buffered_reader(T& byte_reader) :
	m_buffer(),
	m_idx(0),
	m_size(0),
	m_byte_reader(byte_reader)
{
}

template<typename T>
buffered_reader<T>::~buffered_reader()
{
}

template<typename T>
std::uint32_t buffered_reader<T>::read_bytes(std::uint8_t* const& buffer, std::uint32_t const& size)
{
	std::uint32_t const size1 = (std::min)(m_size - m_idx, size);
	std::uint32_t const size2 = size - size1;
	std::copy(cbegin(m_buffer) + m_idx, cbegin(m_buffer) + m_idx + size1, make_checked_iterator(buffer, size));
	m_idx += size1;
	if(size2 != 0)
	{
		std::uint32_t const bytes_read = m_byte_reader.read_bytes(m_buffer.data(), static_cast<std::uint32_t>(m_buffer.size()));
		m_idx = 0;
		m_size = bytes_read;
		if(bytes_read == 0)
		{
			return size1;
		}
		return size1 + read_bytes(buffer + size1, size2);
	}
	return size1;
}


template<typename T>
buffered_reader<T> make_buffered_reader(T& byte_reader)
{
	return buffered_reader<T>{byte_reader};
}


#endif
