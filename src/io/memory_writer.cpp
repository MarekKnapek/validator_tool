#include "memory_writer.h"

#include <utility> // std::move std::swap
#include <cstring> // std::memcpy


memory_writer::memory_writer() noexcept :
	m_destination()
{
}

memory_writer::memory_writer(memory_writer&& other) noexcept :
	memory_writer()
{
	*this = std::move(other);
}

memory_writer& memory_writer::operator=(memory_writer&& other) noexcept
{
	swap(other);
	return *this;
}

memory_writer::~memory_writer() noexcept
{
}

void memory_writer::swap(memory_writer& other) noexcept
{
	using std::swap;
	swap(this->m_destination, other.m_destination);
}

memory_writer::memory_writer(std::uint8_t* const& destination) :
	m_destination(destination)
{
}

void memory_writer::write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size)
{
	std::memcpy(m_destination, buffer, size);
	m_destination += size;
}
