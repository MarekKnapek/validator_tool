#pragma once


#include "Utils.h" // address24_t
#include "comm_response_packet_fwd.h" // comm_response_packet
#include "comm_write_packet_fwd.h" // comm_write_packet


#include <cstdint> // std::uint8_t


#include <windows.h> // HANDLE


class com_port
{
public:
	com_port(std::uint8_t const& com_number);
	~com_port();
public:
	comm_response_packet read_flash_block(address24_t const& address);
	void write_flash_block(address24_t const& addr, comm_write_packet& packet, std::uint8_t const& bytes_to_write);
private:
	void read(std::uint8_t* const& data, int const& len);
	void write(std::uint8_t const* const& data, int const& len);
	void erase_flash();
	HANDLE m_handle;
};
