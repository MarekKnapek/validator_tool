#include "hw2bin.h"

#include "utils/scope_exit.h"
#include "com_port.h"
#include "comm_response_packet.h"
#include "io/windows_file_writer.h"
#include "io/buffered_writer.h"
#include "io/windows_file.h"
#include "utils/verify.h"

#include <windows.h>
#include <cwchar>


int hw2bin(int argc, wchar_t* argv[])
{
	if(argc != 4)
	{
		std::wprintf(L"%s", L"Invalid number of command line arguments!\nThis command needs COM port number and file name.\nThe file name denotes output for contents of EEPROM (in binary format). You can specify . (dot) instead of output file name for redirection to stdout.\n");
		return EXIT_FAILURE;
	}

	wchar_t* end;
	long const val = std::wcstol(argv[2], &end, 10);
	VERIFY(val != LONG_MAX && val != LONG_MIN && errno != ERANGE);
	VERIFY(*end == L'\0');
	VERIFY(val >= 0 && val <= 255);
	std::uint8_t const com_port_number = static_cast<std::uint8_t>(val);

	return hw2bin_impl(com_port_number, argv[3]);
}

std::unique_ptr<progress> hw2bin_async(int argc, wchar_t* argv[])
{
	auto ret = std::make_unique<progress>();
	start_progress(*ret, 0x8'0000, &hw2bin, argc, argv, ret.get());
	return ret;
}

int hw2bin_impl(std::uint8_t const& com_port_number, wchar_t const* const& file_name, progress* const& prgs /* = nullptr */)
{
	if(prgs)
	{
		prgs->worker_thread__update_progress(0);
	}

	com_port com{com_port_number};

	windows_file wf = windows_file::make_for_output_redirectable(file_name);
	windows_file_writer wfbw(wf.get_handle());
	auto wrtr = buffered_writer(writer(std::ref(wfbw)));

	unsigned addr_old = 0;
	unsigned const cycles = 0x8'0000u / sizeof(comm_response_packet::payload_t);
	for(unsigned i = 0; i != cycles; ++i)
	{
		unsigned const addr = i * sizeof(comm_response_packet::payload_t);
		comm_response_packet const response_packet = com.read_flash_block(address24_t{addr});
		wrtr.write_bytes(response_packet.m_payload.data(), static_cast<std::uint32_t>(response_packet.m_payload.size()));
		if(prgs)
		{
			prgs->worker_thread__update_progress(addr);
			if(addr >= addr_old + 0x8'0000 / 1000)
			{
				// Main thread wakeup granularity: no sooner than after each 0.1 %.
				// Main thread might wakeup itself sooner.
				prgs->worker_thread__wakeup_main_thread();
				addr_old = addr;
			}
		}
	}
	unsigned const rest = 0x8'0000 - cycles * sizeof(comm_response_packet::payload_t);
	comm_response_packet const response_packet = com.read_flash_block(address24_t{0x8'0000 - rest});
	wrtr.write_bytes(response_packet.m_payload.data(), rest);

	int const exit_code = EXIT_SUCCESS;
	if(prgs)
	{
		prgs->worker_thread__set_exit_code(exit_code);
		prgs->worker_thread__update_progress(0x8'0000);
	}
	return exit_code;
}
