#pragma once
#ifndef aligned_alloc_h_defined
#define aligned_alloc_h_defined

#include <cstddef> // std::size_t


void* aligned_alloc(std::size_t const& alignment, std::size_t const& size);
void aligned_free(void* const& ptr);


#endif
