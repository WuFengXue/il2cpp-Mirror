#include <stdexcept>
#include "../../MSVC/MSVCSectionParser.h"

#if ENABLE_UNIT_TESTS

#include "Runtime/Testing/Testing.h"

using namespace mapfileparser;

TEST (TestSectionParse)
{
	CHECK_THROW (MSVCSectionParser::Parse (""), std::runtime_error);
}

TEST (TestSectionParseMSVC)
{
	Section section = MSVCSectionParser::Parse (" 0001:00000000 0096f9beH .text                   CODE");
	CHECK_EQUAL (0x00000000, section.start);
	CHECK_EQUAL (0x0096f9be, section.length);
	CHECK_EQUAL (".text", section.name);
	CHECK_EQUAL ("1", section.segmentName);
	CHECK_EQUAL(kSegmentTypeCode, section.segmentType);
}

TEST(TestSectionParseMSVCInDataSegment)
{
	Section section = MSVCSectionParser::Parse(" 0002:00000000 00000308H .idata$5                DATA");
	CHECK_EQUAL(0x00000000, section.start);
	CHECK_EQUAL(0x00000308, section.length);
	CHECK_EQUAL(".idata$5", section.name);
	CHECK_EQUAL("2", section.segmentName);
	CHECK_EQUAL(kSegmentTypeData, section.segmentType);
}

#endif // ENABLE_UNIT_TESTS
