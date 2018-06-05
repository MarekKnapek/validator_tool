#include "aligned_alloc.h"

#include "verify.h" // VERIFY

#include <algorithm> // std::max
#include <cstdint> // std::uintptr_t
#include <new> // operator new, operator delete


void* aligned_alloc(std::size_t const& alignment, std::size_t const& size)
{
	std::size_t const min_alignment = (std::max)(alignof(void*), (std::max)(sizeof(void*), alignment));
	std::size_t const need_bytes = size + (min_alignment - 1);
	std::size_t const alloc_bytes = need_bytes + sizeof(void*);
	void* const allocation = operator new(alloc_bytes);
	void* const min_ptr = static_cast<char*>(allocation) + sizeof(void*);
	std::uintptr_t const mod = reinterpret_cast<std::uintptr_t>(min_ptr) % min_alignment;
	void* const aligned_ptr = static_cast<char*>(min_ptr) + ((min_alignment - mod) % min_alignment);
	VERIFY(reinterpret_cast<std::uintptr_t>(aligned_ptr) % min_alignment == 0);
	VERIFY(reinterpret_cast<std::uintptr_t>(aligned_ptr) % alignment == 0);
	VERIFY(static_cast<char*>(aligned_ptr) >= static_cast<char*>(allocation));
	VERIFY(static_cast<char*>(aligned_ptr) >= static_cast<char*>(min_ptr));
	void const*& orig_alloc = *(static_cast<void const**>(aligned_ptr) - 1);
	orig_alloc = allocation;
	return aligned_ptr;
}

void aligned_free(void* const& ptr)
{
	void* const& orig_alloc = *(static_cast<void* const*>(ptr) - 1);
	operator delete(orig_alloc);
}
