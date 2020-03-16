#include "tests.h"

#include "line_reader_tests.h"
#include "writer_tests.h"
#include "type_eraser_tests.h"


int tests(int argc, wchar_t* argv[])
{
	(void)argc;
	(void)argv;
	line_reader_tests();
	writer_tests();
	type_eraser_tests();
	return 0;
}
