#include "windows_file.h"

#include "../utils/verify.h" // VERIFY

#include <cwchar> // std::wcsncmp
#include <iterator> // std::size
#include <utility> // std::move std::swap

#include <windows.h> // BOOL CloseHandle CreateFileW GENERIC_READ FILE_SHARE_READ OPEN_EXISTING FILE_ATTRIBUTE_NORMAL INVALID_HANDLE_VALUE GetStdHandle STD_INPUT_HANDLE GENERIC_WRITE CREATE_ALWAYS STD_OUTPUT_HANDLE


static wchar_t const s_file_name_redirect_placeholder[] = L".";


windows_file::windows_file() noexcept :
	m_handle{}
{
}

windows_file::windows_file(windows_file&& other) noexcept :
	windows_file{}
{
	*this = std::move(other);
}

windows_file& windows_file::operator=(windows_file&& other) noexcept
{
	this->swap(other);
	return *this;
}

windows_file::windows_file(HANDLE const& handle) :
	m_handle(handle)
{
}

windows_file::~windows_file()
{
	if(m_handle != HANDLE{})
	{
		BOOL const ret = CloseHandle(m_handle);
		VERIFY(ret != 0);
	}
}

void windows_file::swap(windows_file& other) noexcept
{
	using std::swap;
	swap(this->m_handle, other.m_handle);
}

void windows_file::close()
{
	*this = windows_file{};
}

windows_file windows_file::make_for_input(wchar_t const* const& file_name)
{
	HANDLE const handle = CreateFileW(file_name, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	VERIFY(handle != INVALID_HANDLE_VALUE);
	return windows_file{handle};
}

windows_file windows_file::make_for_input_redirectable(wchar_t const* const& file_name)
{
	if(std::wcsncmp(file_name, s_file_name_redirect_placeholder, std::size(s_file_name_redirect_placeholder) - 1) == 0)
	{
		HANDLE const handle = GetStdHandle(STD_INPUT_HANDLE);
		VERIFY(handle != INVALID_HANDLE_VALUE);
		VERIFY(handle != HANDLE{});
		return windows_file{handle};
	}
	return make_for_input(file_name);
}

windows_file windows_file::make_for_output(wchar_t const* const& file_name)
{
	HANDLE const handle = CreateFileW(file_name, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	VERIFY(handle != INVALID_HANDLE_VALUE);
	return windows_file{handle};
}

windows_file windows_file::make_for_output_redirectable(wchar_t const* const& file_name)
{
	if(std::wcsncmp(file_name, s_file_name_redirect_placeholder, std::size(s_file_name_redirect_placeholder) - 1) == 0)
	{
		HANDLE const handle = GetStdHandle(STD_OUTPUT_HANDLE);
		VERIFY(handle != INVALID_HANDLE_VALUE);
		VERIFY(handle != HANDLE{});
		return windows_file{handle};
	}
	return make_for_output(file_name);
}

HANDLE const& windows_file::get_handle() const
{
	return m_handle;
}
