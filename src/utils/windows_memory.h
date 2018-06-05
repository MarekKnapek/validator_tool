#pragma once
#ifndef windows_memory_h_included
#define windows_memory_h_included


#include <cstdint> // std::uint32_t


void* allocate_big_memory(std::uint32_t const& size);
void free_big_memory(void* const& ptr);


struct virtual_free_deleter
{
	void operator()(void* const& ptr) const;
};


#endif
