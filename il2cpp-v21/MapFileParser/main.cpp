#include <iostream>
#include "Driver.h"

#if ENABLE_UNIT_TESTS
#include "Runtime/Testing/Testing.h"
#include "External/UnitTest++/src/TestReporterStdout.h"
#include "External/UnitTest++/src/Test.h"

struct AlwaysTrue
{
	bool operator()(const UnitTest::Test* const) const
	{
		return true;
	}
};
#endif

int main (int argc, char** argv)
{
#if ENABLE_UNIT_TESTS
	for (int i = 1; i < argc; i++)
	{
		if (strcmp (argv[i], "-runNativeTests") == 0 || strcmp(argv[i], "-runUnitTests") == 0)
		{
			UnitTest::TestReporterStdout reporter;

			UnitTest::TestRunner runner (reporter);

			return runner.RunTestsIf (UnitTest::Test::GetTestList (), NULL, AlwaysTrue (), 0);
		}
	}
#endif

	return mapfileparser::Driver::Run(argc, argv, std::cout);
}
