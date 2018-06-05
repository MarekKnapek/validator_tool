#include "../io/buffered_writer.h" // buffered_writer
#include "../io/line_reader.h" // make_line_reader
#include "../io/srec_reader.h" // make_srec_reader
#include "../io/windows_file_byte_reader.h" // windows_file_byte_reader
#include "../io/windows_file_byte_writer.h" // windows_file_byte_writer
#include "../utils/scope_exit.h" // mk::make_scope_exit
#include "../utils/verify.h" // VERIFY

#include <cstdlib> // EXIT_SUCCESS
#include <cwchar> // std::wcsncmp
#include <iterator> // std::size

#include <windows.h> // HANDLE GetStdHandle STD_INPUT_HANDLE INVALID_HANDLE_VALUE CreateFileW GENERIC_READ FILE_SHARE_READ OPEN_EXISTING FILE_ATTRIBUTE_NORMAL CloseHandle STD_OUTPUT_HANDLE GENERIC_WRITE CREATE_ALWAYS


static wchar_t const s_hex2bin_input_redirect[] = L".";
static wchar_t const s_hex2bin_output_redirect[] = L".";



int hex2bin_impl(wchar_t const* const& input_file_name, wchar_t const* const& output_file_name)
{
	HANDLE input_file_handle;
	if(std::wcsncmp(input_file_name, s_hex2bin_input_redirect, std::size(s_hex2bin_input_redirect) - 1) == 0)
	{
		input_file_handle = GetStdHandle(STD_INPUT_HANDLE);
		VERIFY(input_file_handle != INVALID_HANDLE_VALUE);
		VERIFY(input_file_handle != nullptr);
	}
	else
	{
		input_file_handle = CreateFileW(input_file_name, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		VERIFY(input_file_handle != INVALID_HANDLE_VALUE);
	}
	auto const fn_close_input_handle = mk::make_scope_exit([&]()
	{
		BOOL const ret = CloseHandle(input_file_handle);
		VERIFY(ret != 0);
	});

	HANDLE output_file_handle;
	if(std::wcsncmp(output_file_name, s_hex2bin_output_redirect, std::size(s_hex2bin_output_redirect) - 1) == 0)
	{
		output_file_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		VERIFY(output_file_handle != INVALID_HANDLE_VALUE);
		VERIFY(output_file_handle != nullptr);
	}
	else
	{
		output_file_handle = CreateFileW(output_file_name, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		VERIFY(output_file_handle != INVALID_HANDLE_VALUE);
	}
	auto const fn_close_output_handle = mk::make_scope_exit([&]()
	{
		BOOL const ret = CloseHandle(output_file_handle);
		VERIFY(ret != 0);
	});

	windows_file_byte_reader wfbr(input_file_handle);
	auto line_reader = make_line_reader(wfbr);
	auto reader = make_srec_reader(line_reader);
	windows_file_byte_writer wfbw(output_file_handle);
	auto wrtr = buffered_writer(writer(std::ref(wfbw)));
	for(;;)
	{
		__declspec(align(1024)) std::array<std::uint8_t, 1024> buff;
		std::uint32_t const bytes_read = reader.read_bytes(buff.data(), static_cast<std::uint32_t>(buff.size()));
		if(bytes_read == 0)
		{
			break;
		}
		wrtr.write_bytes(buff.data(), bytes_read);
	}

	return EXIT_SUCCESS;
}
