#include "windows_file_reader.h"

#include "../utils/verify.h" // VERIFY

#include <windows.h> // ReadFile DWORD BOOL


windows_file_reader::windows_file_reader(HANDLE const& handle) :
	m_handle(handle)
{
}

std::uint32_t windows_file_reader::read_bytes(std::uint8_t* const& buffer, std::uint32_t const& size)
{
	DWORD size_read;
	BOOL const ret = ReadFile(m_handle, buffer, size, &size_read, nullptr);
	VERIFY(ret != 0);
	return size_read;
}
