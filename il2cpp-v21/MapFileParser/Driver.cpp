#include <fstream>
#include <stdexcept>
#include <cstring>
#include "Driver.h"
#include "MSVC/MSVCMapFileParser.h"
#include "Clang/ClangMapFileParser.h"
#include "SNC/SNCMapFileParser.h"
#include "GCC/GCCMapFileParser.h"
#include "SymbolInfoWriter.h"
#include "Statistics.h"

#ifdef _WINDOWS
#include <Windows.h>
#endif

namespace mapfileparser
{
	enum MapFileFormat
	{
		kMapFileFormatUnknown,
		kMapFileFormatMSVC,
		kMapFileFormatClang,
		kMapFileFormatSNC,
		kMapFileFormatGCC,
	};

	static MapFileParser* GetParser(MapFileFormat mapFileFormat)
	{
		switch (mapFileFormat)
		{
		case kMapFileFormatMSVC:
			return new MSVCMapFileParser();
		case kMapFileFormatClang:
			return new ClangMapFileParser();
		case kMapFileFormatSNC:
			return new SNCMapFileParser();
		case kMapFileFormatGCC:
			return new GCCMapFileParser();
		default:
			throw new std::runtime_error(std::string("Invalid map file format specified"));
		}
	}

	static const char* GetUsage()
	{
		return "Usage: MapFileParser -format=<MSVC|Clang|SNC|GCC> mapFile <-stats|outputFile>";
	}

	static void ParseInputAndWriteOutput(MapFileFormat mapFileFormat, std::ifstream& inputFile, std::ofstream& outputFile)
	{
		MapFileParser* parser = GetParser(mapFileFormat);
		MapFile mapFile = parser->Parse(inputFile);

		SymbolInfoWriter::Write(outputFile, mapFile);
	}

	static void ParseInputAndGenerateStatistics(MapFileFormat mapFileFormat, std::ifstream& inputFile, std::ostream& output)
	{
		MapFileParser* parser = GetParser(mapFileFormat);
		MapFile mapFile = parser->Parse(inputFile);

		Statistics::Generate(mapFile, output);
	}

	int Driver::Run(int argc, const char* const argv[], std::ostream& out)
	{
		if (argc != 4)
		{
			out << GetUsage() << std::endl;
			return 1;
		}

		MapFileFormat mapFileFormat = kMapFileFormatUnknown;
		if (!strcmp(argv[1], "-format=MSVC"))
			mapFileFormat = kMapFileFormatMSVC;
		else if (!strcmp(argv[1], "-format=Clang"))
			mapFileFormat = kMapFileFormatClang;
		else if (!strcmp(argv[1], "-format=SNC"))
			mapFileFormat = kMapFileFormatSNC;
		else if (!strcmp(argv[1], "-format=GCC"))
			mapFileFormat = kMapFileFormatGCC;

		if (mapFileFormat == kMapFileFormatUnknown)
		{
			out << "Unknown map file format.\n";
			out << GetUsage() << std::endl;
			return 1;
		}

		// If we have a runtime error, like a file failing to open or an error parsing,
		// we want to return 0, so that the build will succeed. Since map-file based
		// stack traces are not required to execute, we won't fail the build if
		// we can't generate the output file.

#ifdef _WINDOWS
		// On Windows we need special handling for a path with non-ASCII characters.
		// Use the Win32 API functions to get the proper path to the file.
		LPWSTR *wideStringArgumentList;
		int unused;

		wideStringArgumentList = ::CommandLineToArgvW(::GetCommandLineW(), &unused);
		const wchar_t* mapFileName = wideStringArgumentList[2];
#else
		const char* mapFileName = argv[2];
#endif

		std::ifstream inputFile;
		inputFile.open(mapFileName, std::ifstream::in);
		if (!inputFile.is_open())
		{
			out << "Map file " << argv[2] << " cannot be opened.\n";
			return 1;
		}

		if (strcmp(argv[3],"-stats") == 0)
		{
			if (mapFileFormat != kMapFileFormatClang)
			{
				out << "Statistics are only supported for Clang map files now.\n";
				out << GetUsage() << std::endl;
				return 1;
			}

			try
			{
				ParseInputAndGenerateStatistics(mapFileFormat, inputFile, out);
			}
			catch (const std::exception& e)
			{
				out << e.what() << std::endl;
			}
		}
		else
		{

#ifdef _WINDOWS
			const wchar_t* outputFileName = wideStringArgumentList[3];
#else
			const char* outputFileName = argv[3];
#endif
			std::ofstream outputFile;
			outputFile.open(outputFileName, std::ios::binary | std::ios::out);
			if (!outputFile.is_open())
			{
				out << "Output file " << argv[3] << "cannot be opened.\n";
				return 0;
			}

			try
			{
				ParseInputAndWriteOutput(mapFileFormat, inputFile, outputFile);
			}
			catch (const std::exception& e)
			{
				out << e.what() << std::endl;
			}
		}

		return 0;
	}
}
