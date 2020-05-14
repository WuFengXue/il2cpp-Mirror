#pragma once

#include <ostream>

namespace mapfileparser
{

class Driver
{
public:
	static int Run(int argc, const char* const argv[], std::ostream& out);
};

}
