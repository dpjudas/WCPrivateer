
#include "Precomp.h"
#include "WCTypes.h"
#include "WCArchive.h"
#include "FileEntryReader.h"

WCAstroidType::WCAstroidType(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\TYPES\\ASTRTYPE.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("REAL");
	reader.PushChunk("FORM");
	reader.ReadTag("ASTR");
	reader.PushChunk("INFO");
	reader.Read(info, 11 * 2);
	reader.PopChunk();
	reader.PopChunk();
	reader.PopChunk();
}

/////////////////////////////////////////////////////////////////////////////

WCBaseType::WCBaseType(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\TYPES\\BASETYPE.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("REAL");
	reader.PushChunk("FORM");
	reader.ReadTag("BASE");
	reader.PopChunk();
	reader.PopChunk();
}

/////////////////////////////////////////////////////////////////////////////

WCBeamType::WCBeamType(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\TYPES\\BEAMTYPE.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("REAL");
	reader.PushChunk("FORM");
	reader.ReadTag("BEAM");
	reader.PushChunk("INFO");
	info0 = reader.ReadInt16();
	info1 = reader.ReadInt16();
	info2 = reader.ReadInt16();
	reader.PopChunk();

	reader.PushChunk("FORM");
	reader.ReadTag("HAZE");

	reader.PushChunk("FORM");
	reader.ReadTag("BEAM");
	reader.PushChunk("HAZE");
	beamhaze.resize(reader.GetChunkSize());
	reader.Read(beamhaze.data(), beamhaze.size());
	reader.PopChunk();
	reader.PopChunk(); // BEAM

	reader.PushChunk("FORM");
	reader.ReadTag("SPHR");

	reader.PushChunk("INFO");
	sphereinfo0 = reader.ReadInt16();
	sphereinfo1 = reader.ReadInt16();
	reader.PopChunk();

	reader.PushChunk("HAZE");
	spherehaze.resize(reader.GetChunkSize());
	reader.Read(spherehaze.data(), spherehaze.size());
	reader.PopChunk();

	reader.PopChunk(); // SPHR

	reader.PopChunk(); // HAZE
	reader.PopChunk(); // BEAM
	reader.PopChunk();
}

/////////////////////////////////////////////////////////////////////////////

WCFighterType::WCFighterType(const std::string& filename, WCArchive* archive)
{
	FileEntryReader reader = archive->openFile(filename);
	reader.PushChunk("FORM");
	reader.ReadTag("REAL");
	reader.PushChunk("FORM");
	reader.ReadTag("FITE");

	reader.PushChunk("CTRL");
	control = reader.ReadString(reader.GetChunkSize());
	reader.PopChunk();

	reader.PushChunk("FORM");
	reader.ReadTag("WEAP");
	reader.PushChunk("GUNS");
	guns = reader.ReadArray8(reader.GetChunkSize());
	reader.PopChunk();
	reader.PushChunk("LNCH");
	launch = reader.ReadArray8(reader.GetChunkSize());
	reader.PopChunk();
	reader.PushChunk("MISL");
	missile = reader.ReadArray8(reader.GetChunkSize());
	reader.PopChunk();
	reader.PopChunk(); // WEAP

	reader.PushChunk("FORM");
	reader.ReadTag("ENER");
	reader.PushChunk("INFO");
	energyName = reader.ReadString(8);
	energyInfo = reader.ReadArray8(reader.GetChunkSize() - 8);
	reader.PopChunk();
	reader.PushChunk("DAMG");
	energyDamage = reader.ReadArray8(reader.GetChunkSize());
	reader.PopChunk();
	reader.PopChunk(); // ENER

	reader.PushChunk("FORM");
	reader.ReadTag("SHLD");
	reader.PushChunk("INFO");
	shieldName = reader.ReadString(8);
	shieldInfo = reader.ReadArray8(reader.GetChunkSize() - 8);
	reader.PopChunk();
	reader.PushChunk("ARMR");
	shieldArmor = reader.ReadArray8(reader.GetChunkSize());
	reader.PopChunk();
	reader.PushChunk("DAMG");
	shieldDamage = reader.ReadArray8(reader.GetChunkSize());
	reader.PopChunk();
	reader.PopChunk(); // SHLD

	reader.PushChunk("FORM");
	reader.ReadTag("CRGO");
	reader.PushChunk("INFO");
	cargoName = reader.ReadString(8);
	cargoInfo = reader.ReadArray8(reader.GetChunkSize() - 8);
	reader.PopChunk();
	reader.PushChunk("DATA");
	cargoData = reader.ReadArray8(reader.GetChunkSize());
	reader.PopChunk();
	reader.PushChunk("DAMG");
	cargoDamage = reader.ReadArray8(reader.GetChunkSize());
	reader.PopChunk();
	reader.PopChunk(); // CRGO

	reader.PushChunk("FORM");
	reader.ReadTag("COMM");
	reader.PushChunk("INFO");
	commName = reader.ReadString(8);
	commInfo = reader.ReadArray8(reader.GetChunkSize() - 8);
	reader.PopChunk();
	reader.PopChunk(); // COMM

	reader.PushChunk("TRRT");
	turret = reader.ReadArray8(reader.GetChunkSize());
	reader.PopChunk();

	reader.PopChunk();
	reader.PopChunk();
}

/////////////////////////////////////////////////////////////////////////////

WCProjectileType::WCProjectileType(const std::string& filename, WCArchive* archive)
{
	FileEntryReader reader = archive->openFile(filename);
	reader.PushChunk("FORM");
	reader.ReadTag("REAL");
	reader.PushChunk("FORM");
	reader.ReadTag("PROJ");
	reader.PushChunk("INFO");
	info = reader.ReadArray8(reader.GetChunkSize());
	reader.PopChunk();
	reader.PopChunk();
	reader.PopChunk();
}

/////////////////////////////////////////////////////////////////////////////

WCJumpType::WCJumpType(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\TYPES\\JUMPTYPE.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("REAL");
	reader.PushChunk("FORM");
	reader.ReadTag("JMPT");
	reader.PushChunk("INFO");
	info = reader.ReadUint32();
	reader.PopChunk();
	reader.PushChunk("FORM");
	reader.ReadTag("HAZE");
	reader.PushChunk("INFO");
	hazeinfo0 = reader.ReadInt16();
	hazeinfo1 = reader.ReadInt16();
	reader.PopChunk();
	reader.PushChunk("HAZE");
	haze.resize(reader.GetChunkSize());
	reader.Read(haze.data(), haze.size());
	reader.PopChunk();
	reader.PopChunk();
	reader.PopChunk();
	reader.PopChunk();
}

/////////////////////////////////////////////////////////////////////////////

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
