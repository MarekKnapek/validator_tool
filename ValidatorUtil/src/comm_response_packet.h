#pragma once
#ifndef comm_response_packet_h_included
#define comm_response_packet_h_included


#include <array> // std::array
#include <cstdint> // std::uint8_t


struct comm_response_packet
{
	typedef std::array<std::uint8_t, 3> header_t;
	typedef std::array<std::uint8_t, 118> payload_t;
	typedef std::array<std::uint8_t, 1> footer_t;
	header_t m_header;
	payload_t m_payload;
	footer_t m_footer;
};


#endif
