#include "txt2bin.h"

#include "txt2bin_impl.h"

#include <cwchar> // std::wprintf
#include <cstdlib> // EXIT_FAILURE


int txt2bin(int argc, wchar_t* argv[])
{
	if(argc != 8)
	{
		std::wprintf(L"%s", L"Invalid number of command line arguments!\nThis command needs 6 file names.\nFour input bank files, one input calibration file and one output EEPROM file. You can specify . (dot) instead of output file name for redirection to stdout.\n");
		return EXIT_FAILURE;
	}
	return txt2bin_impl(argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]);
}
