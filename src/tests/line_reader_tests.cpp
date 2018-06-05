#include "line_reader_tests.h"

#include "../io/line_reader.h" // line_reader
#include "../utils/verify.h" // VERIFY
#include "fake_byte_reader.h" // fake_byte_reader


static char const s_data1[] = "a""\x0D""\x0A""b""\x0D""\x0A""c""\x0D""\x0A";
static char const s_data2[] = "a""\x0D""\x0A""b""\x0D""\x0A""c";
static char const s_data3[] = "a""\x0A""b""\x0A""c""\x0A";
static char const s_data4[] = "a""\x0A""b""\x0A""c";
static char const* const s_data[] = {s_data1, s_data2, s_data3, s_data4};


void line_reader_tests()
{
	for(auto const& data : s_data)
	{
		fake_byte_reader byte_reader(reinterpret_cast<std::uint8_t const*>(data));
		auto reader = make_line_reader(byte_reader);
		line_view line;
		VERIFY(reader.read_line(line) == true);
		VERIFY(line.size() == 1);
		VERIFY(*cbegin(line) == 'a');
		VERIFY(reader.read_line(line) == true);
		VERIFY(line.size() == 1);
		VERIFY(*cbegin(line) == 'b');
		VERIFY(reader.read_line(line) == true);
		VERIFY(line.size() == 1);
		VERIFY(*cbegin(line) == 'c');
	}
}
