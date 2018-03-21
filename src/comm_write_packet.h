#pragma once
#ifndef comm_write_packet_h_included
#define comm_write_packet_h_included


#include <array> // std::array
#include <cstdint> // std::uint8_t


struct comm_write_packet
{
	typedef std::array<std::uint8_t, 6> header_t;
	typedef std::array<std::uint8_t, 0xFF> payload_t;
	typedef std::array<std::uint8_t, 1> footer_t;
	header_t m_header;
	payload_t m_payload;
	footer_t m_footer;
};


#endif
