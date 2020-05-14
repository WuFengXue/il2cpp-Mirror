#include <stdexcept>
#include "../../Clang/ClangSymbolParser.h"

#if ENABLE_UNIT_TESTS

#include "Runtime/Testing/Testing.h"

using namespace mapfileparser;

TEST (TestParseClangError)
{
	CHECK_THROW (ClangSymbolParser::Parse (""), std::runtime_error);
}

TEST (TestParseClang)
{
	ClangSymbol clangSymbol = ClangSymbolParser::Parse (" 0x00001F40	0x000000D0	[  3] _U3CRegisterObjectU3Ec__AnonStorey2__ctor_m12113");
	CHECK_EQUAL (0x00001F40, clangSymbol.symbol.start);
	CHECK_EQUAL (0x000000D0, clangSymbol.symbol.length);
	CHECK_EQUAL ("_U3CRegisterObjectU3Ec__AnonStorey2__ctor_m12113", clangSymbol.symbol.name);
	CHECK_EQUAL (3, clangSymbol.objectFileIndex);
}

TEST(TestParseClang64Bit)
{
	ClangSymbol clangSymbol = ClangSymbolParser::Parse(" 0x100EC9264	0x0000004C	[709] __ZNK5Umbra11TomeContext10getGateMapEPKv");
	CHECK_EQUAL(0x100EC9264, clangSymbol.symbol.start);
	CHECK_EQUAL(0x0000004C, clangSymbol.symbol.length);
	CHECK_EQUAL("__ZNK5Umbra11TomeContext10getGateMapEPKv", clangSymbol.symbol.name);
	CHECK_EQUAL(709, clangSymbol.objectFileIndex);
}

#endif // ENABLE_UNIT_TESTS
