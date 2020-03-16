#include "writer.h"

#include "../utils/verify.h" // VERIFY

#include <memory> // std::addressof
#include <utility> // std::move std::swap
#include <cstdint> // std::uint8_t std::uint32_t


writer::writer() noexcept :
	m_type_eraser()
{
}

writer::writer(writer&& other) noexcept :
	writer()
{
	*this = std::move(other);
}

writer& writer::operator=(writer&& other) noexcept
{
	VERIFY(this != std::addressof(other));
	swap(other);
	return *this;
}

writer::~writer() noexcept
{
}

void writer::swap(writer& other) noexcept
{
	VERIFY(this != std::addressof(other));
	using std::swap;
	swap(this->m_type_eraser, other.m_type_eraser);
}

void writer::write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size)
{
	return m_type_eraser(buffer, size);
}
