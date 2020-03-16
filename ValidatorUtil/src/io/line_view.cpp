#include "line_view.h"

#include "../utils/verify.h" // VERIFY

#include <cstddef> // std::ptrdiff_t


line_view::line_view() :
	m_begin(),
	m_size()
{
}

line_view::line_view(std::uint8_t const* const& begin, std::uint32_t const& size) :
	m_begin(begin),
	m_size(size)
{
}

line_view::line_view(std::uint8_t const* const& begin, std::uint8_t const* const& end) :
	m_begin(begin),
	m_size(static_cast<std::uint32_t>(static_cast<std::ptrdiff_t>(end - begin)))
{
	VERIFY(begin <= end);
}

std::uint8_t const* line_view::begin() const
{
	return m_begin;
}

std::uint8_t const* line_view::cbegin() const
{
	return m_begin;
}

std::uint8_t const* line_view::end() const
{
	return m_begin + m_size;
}

std::uint8_t const* line_view::cend() const
{
	return m_begin + m_size;
}

std::uint32_t line_view::size() const
{
	return m_size;
}


std::uint8_t const* begin(line_view const& view)
{
	return view.begin();
}

std::uint8_t const* cbegin(line_view const& view)
{
	return view.cbegin();
}

std::uint8_t const* end(line_view const& view)
{
	return view.end();
}

std::uint8_t const* cend(line_view const& view)
{
	return view.cend();
}

std::uint32_t size(line_view const& view)
{
	return view.size();
}
