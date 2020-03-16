#pragma once
#ifndef convert_h_included
#define convert_h_included


#include "../io/line_view_fwd.h" // line_view

#include <cstdint> // std::uint8_t


long string_to_long(std::uint8_t const* const& begin, std::uint8_t const* const& end);
long string_to_long(line_view const& line);
double string_to_double(std::uint8_t const* const& begin, std::uint8_t const* const& end);
double string_to_double(line_view const& line);


#endif
