#pragma once
#ifndef hw2hex_h_included
#define hw2hex_h_included


#include "progress.h"

#include <cstdint> // std::uint8_t


static wchar_t const s_command_hw2hex[] = L"/hw2hex";


int hw2hex(int argc, wchar_t* argv[]);
std::unique_ptr<progress> hw2hex_async(int argc, wchar_t* argv[]);
int hw2hex_impl(std::uint8_t const& com_port_number, wchar_t const* const& file_name, progress* const& prgs = nullptr);


#endif
