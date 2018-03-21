#include "windows_file_byte_writer.h"

#include "../utils/verify.h" // VERIFY

#include <windows.h> // WriteFile DWORD BOOL


windows_file_byte_writer::windows_file_byte_writer(HANDLE const& handle) :
	m_handle(handle)
{
}

void windows_file_byte_writer::write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size)
{
	DWORD size_written;
	BOOL const ret = WriteFile(m_handle, buffer, size, &size_written, nullptr);
	VERIFY(ret != 0);
	VERIFY(size_written == size);
}
