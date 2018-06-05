#include "com_port.h"

#include "Utils.h" // VERIFY
#include "comm_response_packet.h"
#include "comm_write_packet.h"
#include "utils/verify.h"

#include <algorithm>
#include <array>
#include <cwchar> // std::swprintf
#include <memory>
#include <numeric>

#include <windows.h> // CreateFileW GENERIC_READ GENERIC_WRITE OPEN_EXISTING FILE_ATTRIBUTE_NORMAL HANDLE INVALID_HANDLE_VALUE CloseHandle


static int const s_com_in_queue_len = 128;
static int const s_com_out_queue_len = 128;
static wchar_t const s_com_dcb_config_string[] = L"baud=115200 parity=n data=8 stop=1";
static std::uint8_t const s_comm_SFMARK = 0xA5;
static std::uint8_t const s_comm_command_read_flash = 1;
static std::uint8_t const s_comm_command_write_flash = 2;
static std::uint8_t const s_comm_command_erase_flash = 3;
static std::uint8_t const s_comm_response_read_flash = s_comm_command_read_flash | 0x80;
static std::uint8_t const s_comm_response_write_flash = s_comm_command_write_flash | 0x80;
static std::uint8_t const s_comm_response_erase_flash = s_comm_command_erase_flash | 0x80;


com_port::com_port(std::uint8_t const& /*com_number*/) :
	m_handle()
{
#if 0
	std::array<wchar_t, 256> buff;
	unsigned char const com_number_uchar = com_number;
	int const print_ret = std::swprintf(buff.data(), buff.size(), LR"---(\\.\COM%hhd)---", com_number_uchar);
	VERIFY(print_ret > 0);

	HANDLE const handle = CreateFileW(buff.data(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	VERIFY(handle != INVALID_HANDLE_VALUE);
	m_handle = handle;

	BOOL const setup = SetupComm(m_handle, s_com_in_queue_len, s_com_out_queue_len);
	VERIFY(setup != 0);
	DCB dcb{};
	dcb.DCBlength = sizeof(dcb);
	BOOL const state = GetCommState(m_handle, &dcb);
	VERIFY(state != 0);
	BOOL const build = BuildCommDCBW(s_com_dcb_config_string, &dcb);
	VERIFY(build != 0);
	BOOL const set = SetCommState(m_handle, &dcb);
	VERIFY(set != 0);
	COMMTIMEOUTS timeouts{};
	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 200;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 200;
	BOOL const timeouts_ret = SetCommTimeouts(m_handle, &timeouts);
	VERIFY(timeouts_ret != 0);
	BOOL const escape_ret1 = EscapeCommFunction(m_handle, SETRTS);
	VERIFY(escape_ret1 != 0);
  BOOL const escape_ret2 = EscapeCommFunction(m_handle, CLRDTR);
	VERIFY(escape_ret2 != 0);
#endif
}

com_port::~com_port()
{
	if(m_handle != HANDLE{})
	{
		BOOL const close = CloseHandle(m_handle);
		VERIFY(close != 0);
	}
}

comm_response_packet com_port::read_flash_block(address24_t const& /*address*/)
{
#if 0
	std::array<std::uint8_t, 6> command;
	command[0] = s_comm_SFMARK;
	command[1] = s_comm_command_read_flash;
	command[2] = address.m_data[0];
	command[3] = address.m_data[1];
	command[4] = address.m_data[2];
	command[5] = ~std::accumulate(cbegin(command) + 1, cend(command) - 1, static_cast<std::uint8_t>(0));
	write(command.data(), static_cast<std::uint32_t>(command.size()));

	comm_response_packet packet;
	read(reinterpret_cast<std::uint8_t*>(std::addressof(packet)), sizeof(packet));
	VERIFY(packet.m_header[0] == s_comm_SFMARK);
	VERIFY(packet.m_header[1] == s_comm_response_read_flash);
	VERIFY(packet.m_header[2] == sizeof(packet.m_payload));
	auto const packet_cbegin = reinterpret_cast<std::uint8_t const*>(std::addressof(packet));
	std::uint8_t const response_checksum = std::accumulate(packet_cbegin + 1, packet_cbegin + sizeof(packet), static_cast<std::uint8_t>(0));
	VERIFY(response_checksum == 0xFF);
	return packet;
#endif
	return {};
}

void com_port::write_flash_block(address24_t const& /*addr*/, comm_write_packet& /*packet*/, std::uint8_t const& /*bytes_to_write*/)
{
#if 0
	packet.m_header[0] = s_comm_SFMARK;
	packet.m_header[1] = s_comm_command_write_flash;
	packet.m_header[2] = bytes_to_write;
	packet.m_header[3] = addr.m_data[0];
	packet.m_header[4] = addr.m_data[1];
	packet.m_header[5] = addr.m_data[2];
	packet.m_footer[0] = ~(std::accumulate(cbegin(packet.m_header) + 1, cend(packet.m_header), static_cast<std::uint8_t>(0)) + std::accumulate(cbegin(packet.m_payload), cend(packet.m_payload), static_cast<std::uint8_t>(0)));
	write(reinterpret_cast<std::uint8_t const*>(std::addressof(packet)), sizeof(packet));

#pragma warning(push)
#pragma warning(disable:6001) // Using uninitialized memory.
	std::array<std::uint8_t, 3> response;
	read(response.data(), static_cast<std::uint32_t>(response.size()));
#pragma warning(pop)
	VERIFY(response[0] == s_comm_SFMARK);
	VERIFY(response[1] == s_comm_response_write_flash);
	std::uint8_t const response_checksum = std::accumulate(cbegin(response) + 1, cend(response), static_cast<std::uint8_t>(0));
	VERIFY(response_checksum == 0xFF);
#endif
}

void com_port::read(std::uint8_t* const& data, int const& len)
{
	DWORD bytes_read;
	BOOL const read_ret = ReadFile(m_handle, data, len, &bytes_read, nullptr);
	VERIFY(read_ret != 0);
	//VERIFY(bytes_read == len);
}

void com_port::write(std::uint8_t const* const& data, int const& len)
{
	DWORD bytes_written;
	BOOL const write_ret = WriteFile(m_handle, data, len, &bytes_written, nullptr);
	VERIFY(write_ret != 0);
	//VERIFY(bytes_written == len);
}

void com_port::erase_flash()
{
#if 0
	std::array<std::uint8_t, 3> command;
	command[0] = s_comm_SFMARK;
	command[1] = s_comm_command_erase_flash;
	command[2] = ~std::accumulate(cbegin(command) + 1, cend(command) - 1, static_cast<std::uint8_t>(0));
	write(command.data(), static_cast<std::uint32_t>(command.size()));

	COMMTIMEOUTS timeouts1{};
	timeouts1.ReadIntervalTimeout = 0;
	timeouts1.ReadTotalTimeoutMultiplier = 0;
	timeouts1.ReadTotalTimeoutConstant = 10'000;
	timeouts1.WriteTotalTimeoutMultiplier = 0;
	timeouts1.WriteTotalTimeoutConstant = 10'000;
	BOOL const timeouts_ret1 = SetCommTimeouts(m_handle, &timeouts1);
	VERIFY(timeouts_ret1 != 0);

#pragma warning(push)
#pragma warning(disable:6001) // Using uninitialized memory.
	std::array<std::uint8_t, 3> packet;
	read(packet.data(), static_cast<std::uint32_t>(packet.size()));
#pragma warning(pop)
	VERIFY(packet[0] == s_comm_SFMARK);
	VERIFY(packet[1] == s_comm_response_erase_flash);
	auto const packet_cbegin = reinterpret_cast<std::uint8_t const*>(std::addressof(packet));
	std::uint8_t const response_checksum = std::accumulate(packet_cbegin + 1, packet_cbegin + sizeof(packet), static_cast<std::uint8_t>(0));
	VERIFY(response_checksum == 0xFF);

	COMMTIMEOUTS timeouts2{};
	timeouts2.ReadIntervalTimeout = 0;
	timeouts2.ReadTotalTimeoutMultiplier = 0;
	timeouts2.ReadTotalTimeoutConstant = 200;
	timeouts2.WriteTotalTimeoutMultiplier = 0;
	timeouts2.WriteTotalTimeoutConstant = 200;
	BOOL const timeouts_ret2 = SetCommTimeouts(m_handle, &timeouts2);
	VERIFY(timeouts_ret2 != 0);
#endif
}
