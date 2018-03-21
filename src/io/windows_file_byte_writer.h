#pragma once
#ifndef windows_file_byte_writer_h_included
#define windows_file_byte_writer_h_included


#include <cstdint> // std::uint8_t std::uint32_t

#include <windows.h> // HANDLE


class windows_file_byte_writer
{
public:
	windows_file_byte_writer(HANDLE const& handle);
	void write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size);
private:
	HANDLE const m_handle;
};


#endif
