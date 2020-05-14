#if ENABLE_UNIT_TESTS

#include <sstream>
#include <string>
#include "Runtime/Testing/Testing.h"
#include "../Driver.h"

using namespace mapfileparser;

TEST(DriverReturnsCorrectValueWithIncorrectNumberOfArguments)
{
	std::stringstream unused;
	CHECK_EQUAL(1, Driver::Run(1, NULL, unused));
}

TEST(DriverOutputsCorrectErrorWithIncorrectNumberOfArguments)
{
	std::stringstream output;
	Driver::Run(1, NULL, output);
	CHECK_EQUAL("Usage: MapFileParser -format=<MSVC|Clang|SNC|GCC> mapFile <-stats|outputFile>\n", output.str());
}

TEST(DriverReturnsCorrectValueWithIncorrectMapFileFormat)
{
	const char* arguments[4] =
	{
		"Unused",
		"-format=Foo",
		"Unused",
		"Unused"
	};

	std::stringstream unused;
	CHECK_EQUAL(1, Driver::Run(4, arguments, unused));
}

TEST(DriverOutputsCorrectErrorWithIncorrectMapFileFormat)
{
	const char* arguments[4] =
	{
		"Unused",
		"-format=Foo",
		"Unused",
		"Unused"
	};

	std::stringstream output;
	Driver::Run(4, arguments, output);
	CHECK_EQUAL("Unknown map file format.\nUsage: MapFileParser -format=<MSVC|Clang|SNC|GCC> mapFile <-stats|outputFile>\n", output.str());
}

TEST(DriverReturnsCorrectValueWithBadInputFile)
{
	const char* arguments[4] =
	{
		"Unused",
		"-format=MSVC",
		"NonexistentFile",
		"Unused"
	};

	std::stringstream unused;
	CHECK_EQUAL(1, Driver::Run(4, arguments, unused));
}

TEST(DriverOutputsCorrectErrorWithBadInputFile)
{
	const char* arguments[4] =
	{
		"Unused",
		"-format=MSVC",
		"NonexistentFile",
		"Unused"
	};

	std::stringstream output;
	Driver::Run(4, arguments, output);
	CHECK_EQUAL("Map file NonexistentFile cannot be opened.\n", output.str());
}

#endif // ENABLE_UNIT_TESTS
