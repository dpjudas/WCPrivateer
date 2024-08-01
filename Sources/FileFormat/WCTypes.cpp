
#include "Precomp.h"
#include "WCTypes.h"
#include "WCArchive.h"
#include "FileEntryReader.h"

WCNavMapType::WCNavMapType(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\TYPES\\NAVMAP.IFF");

	reader.PushChunk("FORM");
	reader.ReadTag("NMAP");

	reader.PushChunk("SHAP");
	background = std::make_unique<WCImage>(reader);
	reader.PopChunk();

	reader.PushChunk("GRID");
	grid = std::make_unique<WCImage>(reader);
	reader.PopChunk();

	reader.PushChunk("MOUS");
	mouse = std::make_unique<WCImage>(reader);
	reader.PopChunk();

	reader.PushChunk("FORM");
	reader.ReadTag("BTTN");

	reader.PushChunk("QUAD");
	button.quad = std::make_unique<WCImage>(reader);
	reader.PopChunk();

	reader.PushChunk("NAVI");
	button.navi = std::make_unique<WCImage>(reader);
	reader.PopChunk();

	reader.PushChunk("NEXT");
	button.next = std::make_unique<WCImage>(reader);
	reader.PopChunk();

	reader.PopChunk(); // BTTN

	reader.PushChunk("FORM");
	reader.ReadTag("WNDW");

	reader.PushChunk("MAIN");
	reader.Read(wndw.main, 8);
	reader.PopChunk();

	reader.PushChunk("INFO");
	reader.Read(wndw.info, 8);
	reader.PopChunk();

	reader.PushChunk("BTTN");
	reader.Read(wndw.button, 24);
	reader.PopChunk();

	reader.PopChunk(); // WNDW

	reader.PushChunk("VARS");
	reader.Read(vars, 16);
	reader.PopChunk();

	reader.PushChunk("COLR");
	reader.Read(color, 8);
	reader.PopChunk();

	reader.PushChunk("FORM");
	reader.ReadTag("OBJE");
	reader.PushChunk("FORM");
	reader.ReadTag("STRG");
	reader.PushChunk("SNUM");
	reader.ReadUint16();
	reader.PopChunk();
	reader.PushChunk("DATA");
	std::vector<char> buffer(reader.GetChunkSize());
	reader.Read(buffer.data(), buffer.size());
	std::string str;
	for (char c : buffer)
	{
		if (c == 0)
		{
			if (!str.empty())
				obje.push_back(str);
			str.clear();
		}
		else
		{
			str.push_back(c);
		}
	}
	if (!str.empty())
		obje.push_back(str);
	reader.PopChunk();
	reader.PopChunk(); // STRG
	reader.PopChunk(); // OBJE

	reader.PushChunk("FORM");
	reader.ReadTag("BASN");
	reader.PushChunk("FORM");
	reader.ReadTag("STRG");
	reader.PushChunk("SNUM");
	reader.ReadUint16();
	reader.PopChunk();
	reader.PushChunk("DATA");
	buffer.resize(reader.GetChunkSize());
	reader.Read(buffer.data(), buffer.size());
	str.clear();
	for (char c : buffer)
	{
		if (c == 0)
		{
			if (!str.empty())
				basn.push_back(str);
			str.clear();
		}
		else
		{
			str.push_back(c);
		}
	}
	if (!str.empty())
		basn.push_back(str);
	reader.PopChunk();
	reader.PopChunk(); // STRG
	reader.PopChunk(); // BASN

	reader.PushChunk("FORM");
	reader.ReadTag("BASS");
	reader.PushChunk("FORM");
	reader.ReadTag("STRG");
	reader.PushChunk("SNUM");
	reader.ReadUint16();
	reader.PopChunk();
	reader.PushChunk("DATA");
	buffer.resize(reader.GetChunkSize());
	reader.Read(buffer.data(), buffer.size());
	str.clear();
	for (char c : buffer)
	{
		if (c == 0)
		{
			if (!str.empty())
				bass.push_back(str);
			str.clear();
		}
		else
		{
			str.push_back(c);
		}
	}
	if (!str.empty())
		bass.push_back(str);
	reader.PopChunk();
	reader.PopChunk(); // STRG
	reader.PopChunk(); // BASS

	reader.PopChunk();
}
