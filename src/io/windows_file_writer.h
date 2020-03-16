#pragma once
#ifndef windows_file_writer_h_included
#define windows_file_writer_h_included


#include "../utils/type_traits.h" // is_writer_trait

#include <cstdint> // std::uint8_t std::uint32_t

#include <windows.h> // HANDLE


class windows_file_writer
{
public:
	windows_file_writer() noexcept;
	windows_file_writer(windows_file_writer const& other);
	windows_file_writer(windows_file_writer&& other) noexcept;
	windows_file_writer& operator=(windows_file_writer const& other);
	windows_file_writer& operator=(windows_file_writer&& other) noexcept;
	~windows_file_writer();
	void swap(windows_file_writer& other) noexcept;
public:
	windows_file_writer(HANDLE const& handle);
public:
	HANDLE& get(); // unit tests and debug only
	HANDLE const& get() const; // unit tests and debug only
	void write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size);
private:
	HANDLE m_handle;
};

inline void swap(windows_file_writer& a, windows_file_writer& b) noexcept { a.swap(b); }

template<>
struct is_writer_trait<windows_file_writer> : public std::true_type
{
};


#endif
