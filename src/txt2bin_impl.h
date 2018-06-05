#pragma once
#if 0
#ifndef txt2bin_impl_h_included
#define txt2bin_impl_h_included


#include "io/line_view_fwd.h"

#include "c:/dev/git/github/joboccara/NamedType/named_type.hpp"

#include <cstdint> // std:: uint8_t std::uint32_t


enum class erange : std::uint8_t
{
	range10 = 0,
	range1 = 1,
	range01 = 2
};

enum class erate : std::uint8_t
{
	rate10 = 0,
	rate25 = 1,
	rate50 = 2,
	rate100 = 3
};

using digital_io = fluent::NamedType<std::uint8_t, struct digital_io_tag, fluent::Comparable>;
using volts_10 = fluent::NamedType<double, struct volts_10_tag>;
using volts_1 = fluent::NamedType<double, struct volts_1_tag>;
using volts_01 = fluent::NamedType<double, struct volts_01_tag>;


int txt2bin_impl(wchar_t const* const& block1_file_name, wchar_t const* const& block2_file_name, wchar_t const* const& block3_file_name, wchar_t const* const& block4_file_name, wchar_t const* const& cal_file_name, wchar_t const* const& eeprom_file_name);
bool create_sample(line_view const& file_line, std::array<std::uint8_t, 3>& sample, erange& current_range, erate& current_rate, digital_io& current_dio);
std::uint8_t control_byte(erange const& range, erate const& rate, digital_io const& dio);
volts_10 convert_to_10_volts(double const& value, erange const& type);
double round_to_nearest_ties_to_even(double const& d);
std::array<std::uint8_t, 3> convert_to_volts_binary(volts_10 const& v);


#endif
#endif
