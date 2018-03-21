#include <cstdlib> // EXIT_FAILURE EXIT_SUCCESS
#include <cwchar> // std::wcscmp std::wprintf

#include "bin2hex.h"
#include "bin2txt.h"
#include "hex2bin.h"
#include "hw2bin.h"
#include "hw2hex.h"
#include "hw2hex.h"
#include "tests/tests.h"
#include "txt2bin.h"


struct command
{
	wchar_t const* m_string;
	int(*m_function)(int, wchar_t*[]);
};


static command const s_commands[] =
{
	{s_command_bin2hex, &bin2hex},
	{s_command_bin2txt, &bin2txt},
	{s_command_hex2bin, &hex2bin},
	{s_command_hw2bin, &hw2bin},
	{s_command_hw2hex, &hw2hex},
	{s_command_test, &tests},
	{s_command_txt2bin, &txt2bin},
};


int wmain(int argc, wchar_t* argv[])
{
	if(argc == 1)
	{
		std::wprintf(L"%s", L"Too few command line arguments! Possible commands:\n");
		for(auto const& c : s_commands)
		{
			std::wprintf(L"%s\n", c.m_string);
		}
		return EXIT_FAILURE;
	}
	for(auto const& c : s_commands)
	{
		if(std::wcscmp(argv[1], c.m_string) == 0)
		{
			return (*c.m_function)(argc, argv);
		}
	}
	std::wprintf(L"%s", L"Unknown command line argument!\n");
	return EXIT_FAILURE;
}
