
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

/////////////////////////////////////////////////////////////////////////////

WCTargetingType::WCTargetingType(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\TYPES\\TARGETNG.IFF");

	reader.PushChunk("FORM");
	reader.ReadTag("SYST");

	while (!reader.IsEndOfChunk())
	{
		WCTargetingTarget trgt;

		reader.PushChunk("FORM");
		reader.ReadTag("TRGT");

		reader.PushChunk("NAME");
		trgt.name = reader.ReadUint8();
		reader.PopChunk();

		reader.PushChunk("FORM");
		reader.ReadTag("RADR");

		reader.PushChunk("INFO");
		trgt.radarInfo.resize(reader.GetChunkSize() / 2);
		reader.Read(trgt.radarInfo.data(), trgt.radarInfo.size() * 2);
		reader.PopChunk();

		reader.PushChunk("SHAP");
		trgt.radarShape = std::make_unique<WCImage>(reader);
		reader.PopChunk();

		reader.PopChunk(); // RADR

		reader.PushChunk("FORM");
		reader.ReadTag("DAMG");
		reader.PushChunk("DAMG");
		trgt.damage0 = reader.ReadInt16();
		trgt.damage1 = reader.ReadInt16();
		reader.PopChunk();
		reader.PopChunk(); // DAMG

		reader.PushChunk("INFO");
		trgt.info.resize(reader.GetChunkSize());
		reader.Read(trgt.info.data(), trgt.info.size());
		reader.PopChunk();

		reader.PopChunk(); // TRGT

		targets.push_back(std::move(trgt));
	}

	reader.PopChunk();
}
