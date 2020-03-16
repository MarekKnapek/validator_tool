#pragma once
#ifndef line_view_h_included
#define line_view_h_included


#include <cstdint> // std::uint8_t std::uint32_t


class line_view
{
public:
	line_view();
	line_view(std::uint8_t const* const& begin, std::uint32_t const& size);
	line_view(std::uint8_t const* const& begin, std::uint8_t const* const& end);
public:
	std::uint8_t const* begin() const;
	std::uint8_t const* cbegin() const;
	std::uint8_t const* end() const;
	std::uint8_t const* cend() const;
	std::uint32_t size() const;
private:
	std::uint8_t const* m_begin;
	std::uint32_t m_size;
};

std::uint8_t const* begin(line_view const& view);
std::uint8_t const* cbegin(line_view const& view);
std::uint8_t const* end(line_view const& view);
std::uint8_t const* cend(line_view const& view);
std::uint32_t size(line_view const& view);


#endif
