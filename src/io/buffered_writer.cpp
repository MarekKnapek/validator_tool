#include "buffered_writer.h"

#include "../utils/verify.h" // VERIFY
#include "../utils/checked_iterator.h" // make_checked_iterator
#include "../utils/windows_memory.h" // allocate_big_memory

#include <algorithm> // std::min std::copy
#include <memory> // std::addressof
#include <utility> // std::move std::swap


namespace
{
	static constexpr std::uint32_t g_buffered_writer_buffer_size = 64 * 1024;
}


buffered_writer::buffered_writer() noexcept :
	m_byte_writer(),
	m_buffer(),
	m_idx()
{
}

buffered_writer::buffered_writer(buffered_writer&& other) noexcept :
	buffered_writer()
{
	*this = std::move(other);
}

buffered_writer& buffered_writer::operator=(buffered_writer&& other) noexcept
{
	VERIFY(this != std::addressof(other));
	this->swap(other);
	return *this;
}

buffered_writer::~buffered_writer()
{
	if(m_idx != 0)
	{
		m_byte_writer.write_bytes(m_buffer.get(), m_idx);
	}
}

void buffered_writer::swap(buffered_writer& other) noexcept
{
	using std::swap;
	swap(this->m_byte_writer, other.m_byte_writer);
	swap(this->m_buffer, other.m_buffer);
	swap(this->m_idx, other.m_idx);
}

buffered_writer::buffered_writer(writer&& byte_writer) :
	m_byte_writer(std::move(byte_writer)),
	m_buffer(),
	m_idx()
{
}

writer& buffered_writer::get()
{
	return m_byte_writer;
	// unit tests and debug only
}

writer const& buffered_writer::get() const
{
	return m_byte_writer;
	// unit tests and debug only
}

void buffered_writer::write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size)
{
	if(!m_buffer)
	{
		m_buffer.reset(static_cast<std::uint8_t*>(allocate_big_memory(g_buffered_writer_buffer_size)));
	}
	return write_bytes_nocheck(buffer, size);
}

void buffered_writer::write_bytes_nocheck(std::uint8_t const* const& buffer, std::uint32_t const& size)
{
	std::uint32_t const size1 = (std::min)(g_buffered_writer_buffer_size - m_idx, size);
	std::uint32_t const size2 = size - size1;
	std::copy(buffer, buffer + size1, make_checked_iterator(m_buffer.get(), g_buffered_writer_buffer_size) + m_idx);
	m_idx += size1;
	if(m_idx == g_buffered_writer_buffer_size)
	{
		m_byte_writer.write_bytes(m_buffer.get(), g_buffered_writer_buffer_size);
		m_idx = 0;
		return write_bytes(buffer + size1, size2);
	}
}
