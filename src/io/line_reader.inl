#pragma once
#ifndef line_reader_inl_included
#define line_reader_inl_included


#include "line_reader.h"

#include "line_view.h" // line_view
#include "../Utils.h"

#include <algorithm> // std::copy


template<typename T>
line_reader<T>::line_reader(T& byte_reader) :
	m_buffer(),
	m_idx(static_cast<std::uint32_t>(m_buffer.size())),
	m_end(static_cast<std::uint32_t>(m_buffer.size())),
	m_byte_reader(byte_reader)
{
}

template<typename T>
bool line_reader<T>::read_line(/* out */ line_view& line)
{
	if(m_idx >= s_line_reader_buffer_size)
	{
		std::uint32_t const len1 = m_idx;
		std::uint32_t const len2 = m_end - m_idx;
		std::copy(cbegin(m_buffer) + len1, cend(m_buffer), begin(m_buffer));
		std::uint32_t const read = m_byte_reader.read_bytes(&*(begin(m_buffer) + len2), len1);
		std::uint32_t const new_idx = 0;
		std::uint32_t const new_end = len2 + read;
		m_idx = new_idx;
		m_end = new_end;
	}
	for(std::uint32_t i = m_idx; i != m_end; ++i)
	{
		if(m_buffer[i] == s_LF)
		{
			line = line_view{&*(cbegin(m_buffer) + m_idx), i - m_idx};
			m_idx = i + 1;
			return true;
		}
		else if(m_buffer[i] == s_CR && i + 1 != m_end && m_buffer[i + 1] == s_LF)
		{
			line = line_view{&*(cbegin(m_buffer) + m_idx), i - m_idx};
			m_idx = i + 2;
			return true;
		}
	}
	line = line_view{&*(cbegin(m_buffer) + m_idx), m_end - m_idx};
	bool const ret = m_idx != m_end;
	m_idx = m_end;
	return ret;
}


template<typename T>
line_reader<T> make_line_reader(T& byte_reader)
{
	return line_reader<T>{byte_reader};
}


#endif
