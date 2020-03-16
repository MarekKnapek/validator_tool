#include "txt_writer.h"

#include <utility> // std::move std::swap


txt_writer::txt_writer() noexcept :
	m_writer()
{
}

txt_writer::txt_writer(writer&& wrtr) :
	m_writer(std::move(wrtr))
{
}

txt_writer::txt_writer(txt_writer&& other) noexcept :
	txt_writer()
{
	*this = std::move(other);
}

txt_writer& txt_writer::operator=(txt_writer&& other) noexcept
{
	this->swap(other);
	return *this;
}

txt_writer::~txt_writer() noexcept
{
}

void txt_writer::swap(txt_writer& other) noexcept
{
	using std::swap;
	swap(this->m_writer, other.m_writer);
}

void txt_writer::close() noexcept
{
	*this = txt_writer{};
}

void txt_writer::write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size)
{
	m_writer.write_bytes(buffer, size);
}
