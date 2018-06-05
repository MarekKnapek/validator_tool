#pragma once
#ifndef windows_file_h_included
#define windows_file_h_included


#include <windows.h> // HANDLE


class windows_file
{
public:
	windows_file() noexcept;
	windows_file(windows_file const&) = delete;
	windows_file(windows_file&& other) noexcept;
	windows_file& operator=(windows_file const&) = delete;
	windows_file& operator=(windows_file&& other) noexcept;
	explicit windows_file(HANDLE const& handle);
	~windows_file();
	void swap(windows_file& other) noexcept;
	void close();
public:
	static windows_file make_for_input(wchar_t const* const& file_name);
	static windows_file make_for_input_redirectable(wchar_t const* const& file_name);
	static windows_file make_for_output(wchar_t const* const& file_name);
	static windows_file make_for_output_redirectable(wchar_t const* const& file_name);
public:
	HANDLE const& get_handle() const;
private:
	HANDLE m_handle;
};

inline void swap(windows_file& a, windows_file& b) noexcept { a.swap(b); }


#endif
