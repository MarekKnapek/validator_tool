#include "bin2txt.h"

#include "bin2txt_impl.h" // bin2txt_impl

#include <cstdlib> // EXIT_FAILURE
#include <cwchar> // std::wprintf


int bin2txt(int argc, wchar_t* argv[])
{
	if(argc != 8)
	{
		std::wprintf
		(
			L"%s",
			L"Invalid number of command line arguments!\n"
			"Converts contents of EEPROM form binary format to textual form.\n"
			"This command needs one input and five output file names.\n"
			"Four for four banks and one for calibration.\n"
			"You can specify . (dot) instead of input file name\n"
			"for redirection from/to stdin/stdout.\n"
		);
		return EXIT_FAILURE;
	}

	return bin2txt_impl(argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]);
}
