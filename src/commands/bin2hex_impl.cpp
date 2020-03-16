#include "bin2hex_impl.h"

#include "../algorithm/copy.h" // copy
#include "../io/buffered_reader.h" // make_buffered_reader
#include "../io/buffered_writer.h" // buffered_writer
#include "../io/srec_writer.h" // make_srec_writer
#include "../io/windows_file.h" // windows_file
#include "../io/windows_file_reader.h" // windows_file_reader
#include "../io/windows_file_writer.h" // windows_file_writer

#include <cstdlib> // EXIT_SUCCESS


int bin2hex_impl(wchar_t const* const& input_file_name, wchar_t const* const& output_file_name)
{
	auto input_file = windows_file::make_for_input_redirectable(input_file_name);
	auto wfbr = windows_file_reader{input_file.get_handle()};
	auto reader = make_buffered_reader(wfbr);

	auto output_file = windows_file::make_for_output_redirectable(output_file_name);
	auto wfbw = windows_file_writer{output_file.get_handle()};
	auto bw = buffered_writer(writer(std::ref(wfbw)));
	auto wrtr = make_srec_writer(bw);

	copy(reader, wrtr);

	return EXIT_SUCCESS;
}
