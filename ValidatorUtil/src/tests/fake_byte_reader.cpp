#include "fake_byte_reader.h"

#include "../utils/checked_iterator.h"

#include <algorithm> // std::min std::copy
#include <cstring> // std::strlen


fake_byte_reader::fake_byte_reader(std::uint8_t const* const& data) :
	m_data(data),
	m_size(static_cast<std::uint32_t>(std::strlen(reinterpret_cast<char const*>(data)))),
	m_idx()
{
}

std::uint32_t fake_byte_reader::read_bytes(std::uint8_t* const& buffer, std::uint32_t const& size)
{
	std::uint32_t const size1 = (std::min)(m_size - m_idx, size);
	std::uint32_t const size2 = size - size1;
	std::copy(m_data + m_idx, m_data + size1, make_checked_iterator(buffer, size));
	m_idx += size1;
	return size1;
}
