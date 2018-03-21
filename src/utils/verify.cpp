#include "verify.h"

#include <cstdlib> // std::exit


void verify_failed(int const& line)
{
	std::exit(line);
}
