#include "../../Clang/ClangMapFileParser.h"
#include <sstream>

#if ENABLE_UNIT_TESTS

#include "Runtime/Testing/Testing.h"

using namespace mapfileparser;

static const char* mockMapFile = "# Path: /var/folders/8v/wyyvr8zd5rn3l9nfxz1dv4pw0000gn/T/il2cpp tests/csharp/SourceAssembly/GeneratedSources/_test.exe\n\
# Arch: i386\n\
# Object files:\n\
[0] linker synthesized\n\
[1] /usr/lib/libSystem.dylib\n\
[2] /usr/lib/libc++.dylib\n\
[3] /var/folders/8v/wyyvr8zd5rn3l9nfxz1dv4pw0000gn/T/il2cpp_cache/F14BC2A7F7430F841ABE66A030997030.o\n\
[4] /var/folders/8v/wyyvr8zd5rn3l9nfxz1dv4pw0000gn/T/il2cpp_cache/51181665DE4ABD7FD9F780396E0CD80A.o\n\
# Sections:\n\
# Address	Size		Segment	Section\n\
0x00001F40	0x00DDC22F	__TEXT	__text\n\
0x00DDE170	0x000009C6	__TEXT	__symbol_stub\n\
0x00DDEB38	0x0000098A	__TEXT	__stub_helper\n\
0x00DDF4C4	0x000FFD68	__TEXT	__gcc_except_tab\n\
0x00EDF230	0x0004CF94	__TEXT	__const\n\
0x00F2C1C4	0x00053A85	__TEXT	__cstring\n\
0x00F7FC49	0x00029CA8	__TEXT	__unwind_info\n\
0x00FA98F8	0x000AF708	__TEXT	__eh_frame\n\
0x01059000	0x00015ABC	__DATA	__nl_symbol_ptr\n\
0x0106EABC	0x00000684	__DATA	__la_symbol_ptr\n\
0x0106F140	0x0000006C	__DATA	__mod_init_func\n\
0x0106F1B0	0x0023B008	__DATA	__data\n\
0x012AA1B8	0x0005D044	__DATA	__const\n\
0x01307200	0x00020454	__DATA	__common\n\
0x01327658	0x0000AB48	__DATA	__bss\n\
# Symbols:\n\
# Address	Size		File  Name\n\
0x00DDE170	0x00000006[1] __NSGetEnviron2\n\
0x00001F40	0x000000D0 [  3] _U3CRegisterObjectU3Ec__AnonStorey2__ctor_m12113\n\
0x00002010	0x00000020 [  3] __ZL37il2cpp_codegen_method_info_from_indexj\n\
0x00DDE170	0x00000006[1] __NSGetEnviron\n\
0x00DDE176	0x00000006[1] __Unwind_Resume\n\
0x00002030	0x00000120 [  4] _U3CRegisterObjectU3Ec__AnonStorey2_U3CU3Em__1_m12114\n\
# Dead Stripped Symbols:\n\
<<dead>> 	0x00000060	[  2] l_OBJC_PROTOCOL_$_NSObject\n\
<<dead>> 	0x00000008	[  2] l_OBJC_LABEL_PROTOCOL_$_NSObject\n\
";


TEST (TestClangMapFileParse)
{
	std::stringstream ss (mockMapFile);
	ClangMapFileParser parser;
	MapFile mapFile = parser.Parse (ss);
	CHECK_EQUAL (15, mapFile.sections.size ());

	Section section = mapFile.sections[0];
	CHECK_EQUAL (0x00001F40, section.start);
	CHECK_EQUAL (0x00DDC22F, section.length);
	CHECK_EQUAL ("__text", section.name);
	CHECK_EQUAL ("__TEXT", section.segmentName);

	section = mapFile.sections[14];
	CHECK_EQUAL (0x01327658, section.start);
	CHECK_EQUAL (0x0000AB48, section.length);
	CHECK_EQUAL ("__bss", section.name);
	CHECK_EQUAL ("__DATA", section.segmentName);

	CHECK_EQUAL (3, mapFile.symbols.size ());

	Symbol symbol = mapFile.symbols[0];
	CHECK_EQUAL (0x00001F40, symbol.start);
	CHECK_EQUAL (0x000000D0, symbol.length);
	CHECK_EQUAL ("_U3CRegisterObjectU3Ec__AnonStorey2__ctor_m12113", symbol.name);
	CHECK_EQUAL (kSegmentTypeCode, symbol.segmentType);
	CHECK_EQUAL ("/var/folders/8v/wyyvr8zd5rn3l9nfxz1dv4pw0000gn/T/il2cpp_cache/F14BC2A7F7430F841ABE66A030997030.o", symbol.objectFile);

	symbol = mapFile.symbols[2];
	CHECK_EQUAL (0x00002030, symbol.start);
	CHECK_EQUAL (0x00000120, symbol.length);
	CHECK_EQUAL ("_U3CRegisterObjectU3Ec__AnonStorey2_U3CU3Em__1_m12114", symbol.name);
	CHECK_EQUAL (kSegmentTypeCode, symbol.segmentType);
	CHECK_EQUAL ("/var/folders/8v/wyyvr8zd5rn3l9nfxz1dv4pw0000gn/T/il2cpp_cache/51181665DE4ABD7FD9F780396E0CD80A.o", symbol.objectFile);
}

#endif // ENABLE_UNIT_TESTS
