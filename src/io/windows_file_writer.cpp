#include "windows_file_writer.h"

#include "../utils/verify.h" // VERIFY

#include <utility> // std::move std::swap
#include <memory> // std::addressof

#include <windows.h> // DWORD BOOL WriteFile


windows_file_writer::windows_file_writer() noexcept :
	m_handle()
{
}

windows_file_writer::windows_file_writer(windows_file_writer const& other) :
	windows_file_writer()
{
	*this = other;
}

windows_file_writer::windows_file_writer(windows_file_writer&& other) noexcept :
	windows_file_writer()
{
	*this = std::move(other);
}

windows_file_writer& windows_file_writer::operator=(windows_file_writer const& other)
{
	VERIFY(this != std::addressof(other));
	m_handle = other.m_handle;
	return *this;
}

windows_file_writer& windows_file_writer::operator=(windows_file_writer&& other) noexcept
{
	VERIFY(this != std::addressof(other));
	this->swap(other);
	return *this;
}

windows_file_writer::~windows_file_writer()
{
}

void windows_file_writer::swap(windows_file_writer& other) noexcept
{
	VERIFY(this != std::addressof(other));
	using std::swap;
	swap(this->m_handle, other.m_handle);
}

windows_file_writer::windows_file_writer(HANDLE const& handle) :
	m_handle(handle)
{
}

HANDLE& windows_file_writer::get()
{
	// unit tests and debug only
	return m_handle;
}

HANDLE const& windows_file_writer::get() const
{
	// unit tests and debug only
	return m_handle;
}

void windows_file_writer::write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size)
{
	DWORD size_written;
	BOOL const ret = WriteFile(m_handle, buffer, size, &size_written, nullptr);
	VERIFY(ret != 0);
	VERIFY(size_written == size);
}
