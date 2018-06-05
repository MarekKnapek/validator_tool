#include "convert.h"

#include "../io/line_view.h" // line_view
#include "verify.h" // VERIFY

#include <cerrno> // errno ERANGE
#include <climits> // LONG_MAX LONG_MIN
#include <cmath> // HUGE_VAL
#include <cstdlib> // std::strtol std::strtod


long string_to_long(std::uint8_t const* const& begin, std::uint8_t const* const& end)
{
	VERIFY(begin <= end);
	char* computed_end = const_cast<char*>(reinterpret_cast<char const*>(end));
	long const val = std::strtol(reinterpret_cast<char const*>(begin), &computed_end, 10);
	VERIFY(val != LONG_MAX && val != LONG_MIN && errno != ERANGE);
	VERIFY(computed_end == reinterpret_cast<char const*>(end));
	return val;
}

long string_to_long(line_view const& line)
{
	return string_to_long(cbegin(line), cend(line));
}

double string_to_double(std::uint8_t const* const& begin, std::uint8_t const* const& end)
{
	VERIFY(begin <= end);
	char* computed_end = const_cast<char*>(reinterpret_cast<char const*>(end));
	double const val = std::strtod(reinterpret_cast<char const*>(begin), &computed_end);
	VERIFY(val != HUGE_VAL && errno != ERANGE);
	VERIFY(computed_end == reinterpret_cast<char const*>(end));
	return val;
}

double string_to_double(line_view const& line)
{
	return string_to_double(cbegin(line), cend(line));
}
