#include <stdexcept>
#include "../../Clang/ClangSectionParser.h"

#if ENABLE_UNIT_TESTS

#include "Runtime/Testing/Testing.h"

using namespace mapfileparser;

TEST (TestClangSectionParseError)
{
	CHECK_THROW (ClangSectionParser::Parse (""), std::runtime_error);
}

TEST (TestClangSectionParse)
{
	Section section = ClangSectionParser::Parse ("0x00001F40	0x00DDC22F	__TEXT	__text");
	CHECK_EQUAL (0x00001F40, section.start);
	CHECK_EQUAL (0x00DDC22F, section.length);
	CHECK_EQUAL ("__text", section.name);
	CHECK_EQUAL ("__TEXT", section.segmentName);
	CHECK_EQUAL (kSegmentTypeCode, section.segmentType);
}

TEST(TestClangSectionParse64Bit)
{
	Section section = ClangSectionParser::Parse ("0x1000072D0	0x018B902C	__TEXT	__text");
	CHECK_EQUAL (0x1000072D0, section.start);
	CHECK_EQUAL (0x018B902C, section.length);
	CHECK_EQUAL ("__text", section.name);
	CHECK_EQUAL ("__TEXT", section.segmentName);
	CHECK_EQUAL (kSegmentTypeCode, section.segmentType);
}

TEST(TestClangSectionParseInDataSegment)
{
	Section section = ClangSectionParser::Parse ("0x01059000	0x00015ABC	__DATA	__nl_symbol_ptr");
	CHECK_EQUAL (0x01059000, section.start);
	CHECK_EQUAL (0x00015ABC, section.length);
	CHECK_EQUAL ("__nl_symbol_ptr", section.name);
	CHECK_EQUAL ("__DATA", section.segmentName);
	CHECK_EQUAL (kSegmentTypeData, section.segmentType);
}

#endif // ENABLE_UNIT_TESTS
