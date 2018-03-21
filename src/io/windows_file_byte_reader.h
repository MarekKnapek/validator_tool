#pragma once
#ifndef windows_file_byte_reader_h_included
#define windows_file_byte_reader_h_included


#include <cstdint> // std::uint8_t std::uint32_t

#include <windows.h> // HANDLE


class windows_file_byte_reader
{
public:
	windows_file_byte_reader(HANDLE const& handle);
	std::uint32_t read_bytes(std::uint8_t* const& buffer, std::uint32_t const& size);
private:
	HANDLE const m_handle;
};


#endif
