#include "windows_memory.h"

#include "verify.h" // VERIFY

#include <windows.h> // VirtualAlloc MEM_RESERVE MEM_COMMIT PAGE_READWRITE BOOL VirtualFree MEM_RELEASE


void* allocate_big_memory(std::uint32_t const& size)
{
	void* const ptr = VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	VERIFY(ptr != nullptr);
	return ptr;
}

void free_big_memory(void* const& ptr)
{
	BOOL const ret = VirtualFree(ptr, 0, MEM_RELEASE);
	VERIFY(ret != 0);
}


void virtual_free_deleter::operator()(void* const& ptr) const
{
	free_big_memory(ptr);
}
