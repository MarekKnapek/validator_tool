#pragma once
#ifndef buffered_writer_inl_included
#define buffered_writer_inl_included


#include "buffered_writer.h"

#include <algorithm> // std::min std::copy
#include <iterator> // std::begin


template<typename T>
buffered_writer<T>::buffered_writer(T& byte_writer) :
	m_byte_writer(byte_writer),
	m_idx(),
	m_buffer()
{
}

template<typename T>
void buffered_writer<T>::write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size)
{
	std::uint32_t const size1 = (std::min)(static_cast<std::uint32_t>(m_buffer.size()) - m_idx, size);
	std::uint32_t const size2 = size - size1;
	std::copy(buffer, buffer + size1, begin(m_buffer) + m_idx);
	m_idx += size1;
	if(m_idx == static_cast<std::uint32_t>(m_buffer.size()))
	{
		m_byte_writer.write_bytes(m_buffer.data(), m_idx);
		m_idx = 0;
		return write_bytes(buffer + size1, size2);
	}
}

template<typename T>
buffered_writer<T>::~buffered_writer()
{
	if(m_idx != 0)
	{
		m_byte_writer.write_bytes(m_buffer.data(), m_idx);
	}
}


template<typename T>
buffered_writer<T> make_buffered_writer(T& byte_writer)
{
	return buffered_writer<T>{byte_writer};
}


#endif
