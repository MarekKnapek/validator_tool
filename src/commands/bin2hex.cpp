#include "bin2hex.h"

#include "bin2hex_impl.h" // bin2hex_impl

#include <cstdlib> // EXIT_FAILURE
#include <cwchar> // std::wprintf


int bin2hex(int argc, wchar_t* argv[])
{
	if(argc != 4)
	{
		std::wprintf
		(
			L"%s",
			L"Invalid number of command line arguments!\n"
			"Converts contents of EEPROM form binary format to SREC format.\n"
			"This command needs input and output file names.\n"
			"You can specify . (dot) instead of one or both file name(s)\n"
			"for redirection from/to stdin/stdout.\n"
		);
		return EXIT_FAILURE;
	}

	return bin2hex_impl(argv[2], argv[3]);
}
