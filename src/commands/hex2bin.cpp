#include "hex2bin.h"

#include "hex2bin_impl.h" // hex2bin_impl

#include <cstdlib> // EXIT_FAILURE
#include <cwchar> // std::wprintf


int hex2bin(int argc, wchar_t* argv[])
{
	if(argc != 4)
	{
		std::wprintf
		(
			L"%s",
			L"Invalid number of command line arguments!\n"
			"Convert contents of EEPROM form SREC format to binary format.\n"
			"This command needs input and output file names.\n"
			"You can specify . (dot) instead of one or both file name(s)\n"
			"for redirection from/to stdin/stdout.\n"
		);
		return EXIT_FAILURE;
	}

	return hex2bin_impl(argv[2], argv[3]);
}
