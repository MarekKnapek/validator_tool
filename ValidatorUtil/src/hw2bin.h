#pragma once
#ifndef hw2bin_h_included
#define hw2bin_h_included


#include "progress.h"

#include <cstdint> // std::uint8_t


static wchar_t const s_command_hw2bin[] = L"/hw2bin";


int hw2bin(int argc, wchar_t* argv[]);
std::unique_ptr<progress> hw2bin_async(int argc, wchar_t* argv[]);
int hw2bin_impl(std::uint8_t const& com_port_number, wchar_t const* const& file_name, progress* const& prgs = nullptr);


#endif
