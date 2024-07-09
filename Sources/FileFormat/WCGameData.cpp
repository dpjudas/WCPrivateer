
#include "Precomp.h"
#include "WCGameData.h"
#include "WCArchive.h"
#include "WCPak.h"
#include "WCImage.h"
#include "FileEntryReader.h"

WCGameData::WCGameData(WCArchive* archive) : archive(archive)
{
	LoadFiles();
	LoadApprCock();
	LoadCommStuf();
	LoadCommTxt();
	LoadComodtyp();
	LoadCompText();
	LoadCU();
	LoadEyes();
	LoadFaces();
	LoadFonts();
	LoadGameFlow();
	LoadLandFee();
	LoadLimits();
	LoadMisnText();
	LoadOptions();
	LoadPCString();
	LoadShipArt();
	LoadShipMTxt();
	LoadShipStuf();
	LoadSoftTxt();
	LoadTypeNames();
	LoadGuns();
	LoadWeapons();
}

void WCGameData::LoadFiles()
{
	FileEntryReader reader = archive->openFile("DATA\\OPTIONS\\FILES.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("FILE");

	reader.PushChunk("STAT");
	while (!reader.IsEndOfChunk())
	{
		uint16_t index = reader.ReadUint16();
		std::string text;

		char buffer[8];
		reader.Read(buffer, 8);
		for (uint32_t i = 0; i < 8; i++)
		{
			char c = buffer[i];
			if (c == 0)
				break;
			text.push_back(c);
		}
	}
	reader.PopChunk();

	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("FORM");
		std::string tag1 = reader.ReadTag();
		while (!reader.IsEndOfChunk())
		{
			std::string tag2 = reader.PushChunk();
			// tag1 + tag2 matches one of the strings listed up in STAT. Not sure what the point is.

			std::vector<char> buffer(reader.GetChunkSize());
			reader.Read(buffer.data(), buffer.size());

			std::string text;
			for (size_t i = 0; i < buffer.size(); i++)
			{
				char c = buffer[i];
				if (c == 0)
					break;
				text.push_back(c);
			}

			files[tag1 + tag2] = text;

			reader.PopChunk();
		}
		reader.PopChunk();
	}
	reader.PopChunk();
}

void WCGameData::LoadApprCock()
{
	FileEntryReader reader = archive->openFile("DATA\\OPTIONS\\APPRCOCK.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("SHIP");
	reader.PushChunk("TABL");
	std::vector<uint32_t> table;
	int count = reader.GetChunkSize() / 4;
	for (int i = 0; i < count; i++)
	{
		table.push_back(reader.ReadUint32());
	}
	reader.PopChunk();
	reader.PopChunk();

	for (uint32_t offset : table)
	{
		if (offset == 0)
			break;

		reader.Seek(offset);
		reader.ReadUint32BE(); // Chunk size for what follows

		reader.PushChunk("ASHP");
		
		std::string key, value;
		char buffer[8];

		reader.Read(buffer, 8);
		for (uint32_t i = 0; i < 8; i++)
		{
			char c = buffer[i];
			if (c == 0)
				break;
			key.push_back(c);
		}

		reader.Read(buffer, 8);
		for (uint32_t i = 0; i < 8; i++)
		{
			char c = buffer[i];
			if (c == 0)
				break;
			value.push_back(c);
		}

		apprCock[key] = value;

		reader.PopChunk();
	}
}

void WCGameData::LoadCommStuf()
{
	FileEntryReader reader = archive->openFile("DATA\\OPTIONS\\COMMSTUF.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("COMD");
	reader.PushChunk("FORM");
	reader.ReadTag("ITRN");

	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("INFO");

		int index = reader.ReadUint8();
		commStuff.push_back(std::make_unique<WCImage>(reader));
		if (!reader.IsEndOfChunk())
			throw std::runtime_error("Expected end of INFO chunk");

		reader.PopChunk();
	}

	reader.PopChunk();
	reader.PopChunk();
}

void WCGameData::LoadCommTxt()
{
	commText = LoadStringList("DATA\\OPTIONS\\COMMTXT.IFF");
}

void WCGameData::LoadComodtyp()
{
	FileEntryReader reader = archive->openFile("DATA\\OPTIONS\\COMODTYP.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("COMD");

	auto list = std::make_unique<WCCommodityList>();

	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("FORM");
		reader.ReadTag("COMM");

		WCCommodity commodity;
		while (!reader.IsEndOfChunk())
		{
			std::string tag = reader.PushChunk();

			if (tag == "INFO")
			{
				commodity.info0 = reader.ReadUint16();
				commodity.info1 = reader.ReadUint16();
			}
			else if (tag == "LABL")
			{
				std::vector<uint8_t> buffer(reader.GetChunkSize());
				reader.Read(buffer.data(), buffer.size());

				std::string text;
				for (size_t i = 0; i < buffer.size(); i++)
				{
					char c = buffer[i];
					if (c == 0)
						break;
					text.push_back(c);
				}

				commodity.label = std::move(text);
			}
			else if (tag == "COST")
			{
				std::vector<int16_t> buffer(reader.GetChunkSize() / 2);
				reader.Read(buffer.data(), buffer.size() * 2);
				commodity.cost = std::move(buffer);
			}
			else if (tag == "AVAL")
			{
				std::vector<int16_t> buffer(reader.GetChunkSize() / 2);
				reader.Read(buffer.data(), buffer.size() * 2);
				commodity.availability = std::move(buffer);
			}
			else
			{
				throw std::runtime_error("Unknown tag");
			}

			reader.PopChunk();
		}
		list->commodities.push_back(std::move(commodity));

		reader.PopChunk();
	}

	reader.PopChunk();

	reader.PushChunk("FORM");
	reader.ReadTag("MDAT");

	reader.PushChunk("FROM");
	{
		std::vector<uint8_t> buffer(reader.GetChunkSize());
		reader.Read(buffer.data(), buffer.size());
		list->from = std::move(buffer);
	}
	reader.PopChunk();

	reader.PushChunk("SELL");
	{
		std::vector<uint8_t> buffer(reader.GetChunkSize());
		reader.Read(buffer.data(), buffer.size());
		list->sell = std::move(buffer);
	}
	reader.PopChunk();

	reader.PushChunk("DEST");
	{
		std::vector<uint8_t> buffer(reader.GetChunkSize());
		reader.Read(buffer.data(), buffer.size());
		list->dest = std::move(buffer);
	}
	reader.PopChunk();

	reader.PopChunk();

	commodityList = std::move(list);
}

void WCGameData::LoadCompText()
{
	FileEntryReader reader = archive->openFile("DATA\\OPTIONS\\COMPTEXT.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("COMP");
	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("FORM");
		std::string tag = reader.ReadTag();
		while (!reader.IsEndOfChunk())
		{
			std::string key = reader.PushChunk();

			std::vector<uint8_t> buffer(reader.GetChunkSize());
			reader.Read(buffer.data(), buffer.size());

			std::string text;
			for (size_t i = 0; i < buffer.size(); i++)
			{
				char c = buffer[i];
				if (c == 0)
					break;
				text.push_back(c);
			}

			if (tag == "MRCH")
				merchantText[key] = text;
			else if (tag == "MERC")
				mercenaryText[key] = text;
			else if (tag == "AUTO")
				missionComputerText[key] = text;

			reader.PopChunk();
		}
		reader.PopChunk();
	}
	reader.PopChunk();
}

void WCGameData::LoadCU()
{
	convBackgroundList = std::make_unique<WCConvBackgroundList>(archive);
}

void WCGameData::LoadEyes()
{
	convEyesList = std::make_unique<WCConvEyesList>(archive);
}

void WCGameData::LoadFaces()
{
	convFaceList = std::make_unique<WCConvFaceList>(archive);
}

void WCGameData::LoadFonts()
{
	FileEntryReader reader = archive->openFile("DATA\\OPTIONS\\FONTS.IFF");

	reader.PushChunk("FORM");
	reader.ReadTag("FILE");

	reader.PushChunk("FONT");
	reader.PushChunk("conv");
	convFont = std::make_unique<WCImage>(reader);
	reader.PopChunk();
	reader.PopChunk();

	reader.PushChunk("FONT");
	reader.PushChunk("none");
	reader.PopChunk();
	reader.PopChunk();

	reader.PopChunk();
}

void WCGameData::LoadGameFlow()
{
	FileEntryReader reader = archive->openFile("DATA\\OPTIONS\\GAMEFLOW.IFF");

	reader.PushChunk("FORM");
	reader.ReadTag("GAME");

	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("FORM");
		std::string tag = reader.ReadTag();
		if (tag == "MISS")
		{
			WCGameFlowMission mission;

			reader.PushChunk("INFO");
			mission.info = reader.ReadUint8();
			reader.PopChunk();

			reader.PushChunk("TUNE");
			mission.tune = reader.ReadUint8();
			reader.PopChunk();

			reader.PushChunk("EFCT");
			std::vector<uint8_t> effect(reader.GetChunkSize());
			reader.Read(effect.data(), effect.size());
			mission.effect = std::move(effect);
			reader.PopChunk();

			while (!reader.IsEndOfChunk())
			{
				reader.PushChunk("FORM");
				std::string tag2 = reader.ReadTag();
				if (tag2 == "SCEN")
				{
					WCGameFlowScene scene;

					reader.PushChunk("INFO");
					scene.info = reader.ReadUint8();
					if (!reader.IsEndOfChunk())
						throw std::runtime_error("More data in scene info");
					reader.PopChunk();
					while (!reader.IsEndOfChunk())
					{
						reader.PushChunk("FORM");
						tag2 = reader.ReadTag();

						if (tag2 == "SPRT")
						{
							WCGameFlowSprite sprite;
							while (!reader.IsEndOfChunk())
							{
								std::string tag3 = reader.PushChunk();
								if (tag3 == "INFO")
								{
									sprite.info = reader.ReadUint8();
								}
								else if (tag3 == "EFCT")
								{
									std::vector<uint8_t> sprteffect(reader.GetChunkSize());
									reader.Read(sprteffect.data(), sprteffect.size());
									sprite.effect = std::move(sprteffect);
								}
								else if (tag3 == "REQU")
								{
									std::vector<uint8_t> requ(reader.GetChunkSize());
									reader.Read(requ.data(), requ.size());
									sprite.request = std::move(requ);
								}
								else
								{
									throw std::runtime_error("Unknown tag " + tag3);
								}
								if (!reader.IsEndOfChunk())
									throw std::runtime_error("More data in sprite chunk");
								reader.PopChunk();
							}
							scene.sprites.push_back(sprite);
						}
						else
						{
							throw std::runtime_error("Unknown tag " + tag2);
						}

						reader.PopChunk();
						mission.scenes.push_back(scene);
					}
				}
				else
				{
					throw std::runtime_error("Unknown tag " + tag2);
				}
				reader.PopChunk();
			}

			gameFlow.missions.push_back(std::move(mission));
		}
		else if (tag == "CONV")
		{
			reader.PushChunk("DATA");
			while (!reader.IsEndOfChunk())
			{
				char buffer[9] = {};
				reader.Read(buffer, 8);
				std::string text = buffer;
				gameFlow.conversations.push_back(std::move(text));
			}
			reader.PopChunk();
		}
		else
		{
			throw std::runtime_error("Unknown tag " + tag);
		}
		reader.PopChunk();
	}

	reader.PopChunk();
}

void WCGameData::LoadLandFee()
{
	FileEntryReader reader = archive->openFile("DATA\\OPTIONS\\LANDFEE.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("LFEE");
	reader.PushChunk("DATA");
	landFee = reader.ReadUint32();
	reader.PopChunk();
	reader.PopChunk();
}

void WCGameData::LoadLimits()
{
	FileEntryReader reader = archive->openFile("DATA\\OPTIONS\\LIMITS.IFF");

	reader.PushChunk("FORM");
	reader.ReadTag("SHIP");

	while (!reader.IsEndOfChunk())
	{
		std::string name = reader.PushChunk();
		if (name == "FORM")
		{
			reader.ReadTag("ENGN");

			while (!reader.IsEndOfChunk())
			{
				reader.PushChunk("FORM");
				reader.ReadTag("TYPE");

				WCLimitEngine engine;

				reader.PushChunk("INFO");
				engine.info = reader.ReadUint8();
				reader.PopChunk();

				reader.PushChunk("GENR");
				for (int i = 0; i < 3; i++)
					engine.genr[i] = reader.ReadInt16();
				reader.PopChunk();

				limits.engines.push_back(std::move(engine));

				reader.PopChunk();
			}
		}
		else
		{
			std::vector<uint8_t> data(reader.GetChunkSize());
			reader.Read(data.data(), data.size());

			WCLimitNamed named;
			named.name = name;
			named.unknown = std::move(data);
			limits.named.push_back(std::move(named));
		}
		reader.PopChunk();
	}

	reader.PopChunk();
}

void WCGameData::LoadMisnText()
{
	FileEntryReader reader = archive->openFile("DATA\\OPTIONS\\MISNTEXT.IFF");

	reader.PushChunk("FORM");
	reader.ReadTag("MISN");

	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("FORM");
		std::string tag = reader.ReadTag();

		reader.PushChunk("COUN");
		uint8_t count = reader.ReadUint8();
		reader.PopChunk();

		while (!reader.IsEndOfChunk())
		{
			std::string tag2 = reader.PushChunk();
			if (tag2 == "NAME")
			{
				std::vector<char> buffer(reader.GetChunkSize());
				reader.Read(buffer.data(), buffer.size());
				std::vector<std::string> texts;
				size_t last = 0;
				for (size_t pos = 0; pos < buffer.size(); pos++)
				{
					if (buffer[pos] == 0)
					{
						texts.push_back(std::string(&buffer[last], pos - last));
						last = pos + 1;
					}
				}
				if (last != buffer.size())
					texts.push_back(std::string(&buffer[last], buffer.size() - last));

				if (tag == "CORP")
					missionText.corporations = std::move(texts);
				else if (tag == "ENMY")
					missionText.enemies = std::move(texts);
				else
					throw std::runtime_error("Unknown tag " + tag);
			}
			else if (tag2 == "SMRY")
			{
				std::vector<char> buffer(reader.GetChunkSize());
				reader.Read(buffer.data(), buffer.size());
				std::string text(buffer.data(), buffer.size());
				while (!text.empty() && text.back() == 0)
					text.pop_back();
				missionText.summary[tag] = std::move(text);
			}
			else if (tag2 == "COPY")
			{
				std::vector<char> buffer(reader.GetChunkSize());
				reader.Read(buffer.data(), buffer.size());
				std::string text(buffer.data(), buffer.size());
				while (!text.empty() && text.back() == 0)
					text.pop_back();
				missionText.copy[tag] = std::move(text);
			}
			else
			{
				throw std::runtime_error("Unknown tag2 " + tag2);
			}
			reader.PopChunk();
		}
		reader.PopChunk();
	}
	reader.PopChunk();
}

void WCGameData::LoadOptions()
{
	sceneList = std::make_unique<WCSceneList>(archive);
}

void WCGameData::LoadPCString()
{
	pcString = LoadStringList("DATA\\OPTIONS\\PCSTRING.IFF");
}

void WCGameData::LoadShipArt()
{
	FileEntryReader reader = archive->openFile("DATA\\OPTIONS\\SHIPART.IFF");

	reader.PushChunk("FORM");
	reader.ReadTag("SHAR");
	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("FORM");
		std::string tag = reader.ReadTag();
		
		WCShipArt shipart;
		while (!reader.IsEndOfChunk())
		{
			reader.PushChunk("FORM");
			std::string tag2 = reader.ReadTag();
			if (tag2 == "APPR")
			{
				reader.PushChunk("SHAP");
				shipart.shape = std::make_unique<WCImage>(reader);
				reader.PopChunk();
				if (!reader.IsEndOfChunk())
					throw std::runtime_error("More data in chunk");
			}
			else if (tag2 == "GUNS")
			{
				while (!reader.IsEndOfChunk())
				{
					reader.PushChunk("FORM");
					std::string tag3 = reader.ReadTag();
					if (tag3 == "GUNT")
					{
						WCShipArtGunType guntype;

						reader.PushChunk("INFO");
						guntype.info = reader.ReadUint8();
						if (!reader.IsEndOfChunk())
							throw std::runtime_error("More data in chunk");
						reader.PopChunk();
						reader.PushChunk("DESC");
						std::vector<uint8_t> desc(reader.GetChunkSize());
						reader.Read(desc.data(), desc.size());
						guntype.desc = std::move(desc);
						reader.PopChunk();
						reader.PushChunk("SHAP");
						guntype.shape = std::make_unique<WCImage>(reader);
						reader.PopChunk();
						if (!reader.IsEndOfChunk())
							throw std::runtime_error("More data in chunk");

						shipart.guns.push_back(std::move(guntype));
					}
					else
					{
						throw std::runtime_error("Unknown tag3 " + tag3);
					}
					reader.PopChunk();
				}
			}
			else if (tag2 == "MISC")
			{
				while (!reader.IsEndOfChunk())
				{
					reader.PushChunk("FORM");
					std::string tag3 = reader.ReadTag();
					if (tag3 == "SCAT")
					{
						WCShipArtGunType scat;
						reader.PushChunk("INFO");
						scat.info = reader.ReadUint8();
						if (!reader.IsEndOfChunk())
							throw std::runtime_error("More data in chunk");
						reader.PopChunk();
						reader.PushChunk("DESC");
						std::vector<uint8_t> desc(reader.GetChunkSize());
						reader.Read(desc.data(), desc.size());
						scat.desc = std::move(desc);
						reader.PopChunk();
						reader.PushChunk("SHAP");
						scat.shape = std::make_unique<WCImage>(reader);
						reader.PopChunk();
						if (!reader.IsEndOfChunk())
							throw std::runtime_error("More data in chunk");
						shipart.misc.push_back(std::move(scat));
					}
					else
					{
						throw std::runtime_error("Unknown tag3 " + tag3);
					}
					reader.PopChunk();
				}
			}
			else
			{
				throw std::runtime_error("Unknown tag2 " + tag2);
			}
			reader.PopChunk();
		}
		reader.PopChunk();

		shipArt[tag] = std::move(shipart);
	}

	reader.PopChunk();
}

void WCGameData::LoadShipMTxt()
{
	shipMText = LoadStringList("DATA\\OPTIONS\\SHIPMTXT.IFF");
}

void WCGameData::LoadShipStuf()
{
	FileEntryReader reader = archive->openFile("DATA\\OPTIONS\\SHIPSTUF.IFF");

	// label and images for ship parts (laser, mass driver, etc.)

	reader.PushChunk("FORM");
	reader.ReadTag("SHPM");

	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("FORM");
		std::string tag = reader.ReadTag();
		if (tag == "GUNT" || tag == "SOFT")
		{
			WCShipStuffItem item;

			reader.PushChunk("INFO");
			item.info[0] = reader.ReadUint8();
			item.info[1] = reader.ReadUint8();
			reader.PopChunk();

			reader.PushChunk("LABL");
			std::vector<uint8_t> buffer(reader.GetChunkSize());
			reader.Read(buffer.data(), buffer.size());
			std::string label;
			for (size_t i = 0; i < buffer.size(); i++)
			{
				char c = buffer[i];
				if (c == 0)
					break;
				label.push_back(c);
			}
			item.label = std::move(label);
			reader.PopChunk();

			reader.PushChunk("COST");
			item.buyprice = reader.ReadUint32();
			item.sellprice = reader.ReadUint32();
			reader.PopChunk();

			reader.PushChunk("SHAP");
			item.shape = std::make_unique<WCImage>(reader);
			reader.PopChunk();

			if (tag == "SOFT")
				softwareStuff.push_back(std::move(item));
			else
				shipStuff.push_back(std::move(item));
		}
		else
		{
			throw std::runtime_error("Unknown tag");
		}

		reader.PopChunk();
	}

	reader.PopChunk();
}

void WCGameData::LoadSoftTxt()
{
	softText = LoadStringList("DATA\\OPTIONS\\SOFTTXT.IFF");
}

void WCGameData::LoadTypeNames()
{
	std::vector<std::string> strings = LoadStringList("DATA\\OPTIONS\\TYPENAMS.IFF");
	for (size_t i = 0, count = strings.size() / 2; i < count; i++)
	{
		typeNames[strings[i * 2]] = strings[i * 2 + 1];
	}
}

void WCGameData::LoadGuns()
{
	FileEntryReader reader = archive->openFile("DATA\\TYPES\\GUNS.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("GUNS");
	reader.PushChunk("TABL");
	std::vector<uint32_t> table;
	int count = reader.GetChunkSize() / 4;
	for (int i = 0; i < count; i++)
	{
		table.push_back(reader.ReadUint32());
	}
	reader.PopChunk();
	reader.PopChunk();

	for (uint32_t offset : table)
	{
		if (offset == 0)
			break;

		reader.Seek(offset);
		reader.ReadUint32BE(); // Chunk size for what follows

		reader.PushChunk("UNIT");

		char shortname[6] = {};
		char longname[17] = {};
		reader.Read(shortname, 5);
		reader.Read(longname, 16);

		WCGun gun;
		gun.shortname = shortname;
		gun.longname = longname;
		gun.abuse = reader.ReadUint24();
		gun.velocity = reader.ReadUint24();
		gun.lifetime = reader.ReadUint32(); // fixed point 24:8
		gun.refirerate = reader.ReadUint32(); // fixed point 24:8
		gun.energy = reader.ReadUint16();
		gun.damage = reader.ReadInt16(); // divide by 10 to get gigajoule value
		gun.unknown0 = reader.ReadUint8();
		gun.unknown1 = reader.ReadUint32();
		guns.push_back(std::move(gun));

		reader.PopChunk();
	}
}

void WCGameData::LoadWeapons()
{
	FileEntryReader reader = archive->openFile("DATA\\TYPES\\WEAPONS.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("WEAP");

	reader.PushChunk("FORM");
	reader.ReadTag("LNCH");
	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("UNIT");

		WCLaunchWeapon weapon;
		weapon.velocity = reader.ReadUint16();
		weapon.lifetime = reader.ReadUint16();
		weapon.unknown = reader.ReadUint16();
		weapon.damage = reader.ReadUint8();
		launchWeapons.push_back(weapon);

		reader.PopChunk();
	}
	reader.PopChunk();

	reader.PushChunk("FORM");
	reader.ReadTag("MISL");
	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("UNIT");

		WCMissileWeapon weapon;

		weapon.index = reader.ReadUint8();

		char shortname[9] = {};
		char longname[17] = {};
		reader.Read(shortname, 8);
		reader.Read(longname, 16);

		weapon.shortname = shortname;
		weapon.longname = longname;
		weapon.velocity = reader.ReadUint16();
		weapon.lifetime = reader.ReadUint16();
		weapon.unknown0 = reader.ReadUint16();
		weapon.damage = reader.ReadUint8();
		reader.Read(weapon.unknown1, 4);
		missileWeapons.push_back(weapon);

		reader.PopChunk();
	}
	reader.PopChunk();
	reader.PopChunk();
}

std::vector<std::string> WCGameData::LoadStringList(const std::string& filename)
{
	FileEntryReader reader = archive->openFile(filename);
	reader.PushChunk("FORM");
	reader.ReadTag("STRG");

	reader.PushChunk("SNUM");
	int count = reader.ReadUint16();
	reader.PopChunk();

	std::vector<std::string> list;
	list.reserve(count);

	reader.PushChunk("DATA");
	for (int i = 0; i < count; i++)
	{
		std::string text;
		while (true)
		{
			char c = reader.ReadUint8();
			if (c == 0)
				break;
			text.push_back(c);
		}
		list.push_back(std::move(text));
	}
	reader.PopChunk();

	reader.PopChunk();
	return list;
}

/*

DATA.AIDS.ATTITUDE.IFF
DATA.AIDS.BASE_AGR.IFF
DATA.AIDS.BASE_DER.IFF
DATA.AIDS.BASE_MIN.IFF
DATA.AIDS.BASE_NC.IFF
DATA.AIDS.BASE_ND.IFF
DATA.AIDS.BASE_OX.IFF
DATA.AIDS.BASE_PER.IFF
DATA.AIDS.BASE_PIR.IFF
DATA.AIDS.BASE_PLE.IFF
DATA.AIDS.BASE_REF.IFF
DATA.AIDS.BOU_3C.IFF
DATA.AIDS.BOU_4C.IFF
DATA.AIDS.BOU_AA.IFF
DATA.AIDS.BOU_AF.IFF
DATA.AIDS.BOU_DF.IFF
DATA.AIDS.BOU_SA.IFF
DATA.AIDS.BOU_SF.IFF
DATA.AIDS.COM_AF.IFF
DATA.AIDS.COM_DF.IFF
DATA.AIDS.COM_SF.IFF
DATA.AIDS.CONFED1.IFF
DATA.AIDS.CON_AA.IFF
DATA.AIDS.CON_DP.IFF
DATA.AIDS.CON_SA.IFF
DATA.AIDS.FORGE.IFF
DATA.AIDS.GARVICK.IFF
DATA.AIDS.KIL1.IFF
DATA.AIDS.KIL_AA.IFF
DATA.AIDS.KIL_AF.IFF
DATA.AIDS.KIL_DA.IFF
DATA.AIDS.KIL_DF.IFF
DATA.AIDS.KIL_SF.IFF
DATA.AIDS.KROIZ.IFF
DATA.AIDS.MANEUVER.IFF
DATA.AIDS.MER_AA.IFF
DATA.AIDS.MER_AP.IFF
DATA.AIDS.MER_DP.IFF
DATA.AIDS.MER_SP.IFF
DATA.AIDS.MFDFACES.IFF
DATA.AIDS.MIGGS.IFF
DATA.AIDS.MIL_1A.IFF
DATA.AIDS.MIL_AA.IFF
DATA.AIDS.MIL_DA.IFF
DATA.AIDS.MIL_SA.IFF
DATA.AIDS.PIR_3B.IFF
DATA.AIDS.PIR_5A.IFF
DATA.AIDS.PIR_AA.IFF
DATA.AIDS.PIR_AF.IFF
DATA.AIDS.PIR_DA.IFF
DATA.AIDS.PIR_SA.IFF
DATA.AIDS.PIR_SF.IFF
DATA.AIDS.REIS.IFF
DATA.AIDS.RETRO1.IFF
DATA.AIDS.RETRO2.IFF
DATA.AIDS.RHOMBUS.IFF
DATA.AIDS.RIORD1.IFF
DATA.AIDS.SDRONE.IFF
DATA.AIDS.SEELIG.IFF
DATA.AIDS.SSCOUT.IFF
DATA.AIDS.TOTH.IFF
DATA.APPEARNC.AGRIC.IFF
DATA.APPEARNC.ALIEN.IFF
DATA.APPEARNC.ASTROID1.IFF
DATA.APPEARNC.ASTROID2.IFF
DATA.APPEARNC.BIGEXPL.IFF
DATA.APPEARNC.BODYPRT1.IFF
DATA.APPEARNC.BODYPRT2.IFF
DATA.APPEARNC.BRDSWORD.IFF
DATA.APPEARNC.CAPGOOD.IFF
DATA.APPEARNC.CLNKDETH.IFF
DATA.APPEARNC.CLUNKER.IFF
DATA.APPEARNC.CONST.IFF
DATA.APPEARNC.DEMON.IFF
DATA.APPEARNC.DERLCT.IFF
DATA.APPEARNC.DETHEXPL.IFF
DATA.APPEARNC.DETROIT.IFF
DATA.APPEARNC.DRALTHI.IFF
DATA.APPEARNC.DRAYMAN.IFF
DATA.APPEARNC.DRONE.IFF
DATA.APPEARNC.DUMBFIRE.IFF
DATA.APPEARNC.DUST1.IFF
DATA.APPEARNC.DUST2.IFF
DATA.APPEARNC.DUST3.IFF
DATA.APPEARNC.EJECT.IFF
DATA.APPEARNC.FIGHTER.IFF
DATA.APPEARNC.FITEDETH.IFF
DATA.APPEARNC.FOOD.IFF
DATA.APPEARNC.FRIGATE.IFF
DATA.APPEARNC.FRNDFOE.IFF
DATA.APPEARNC.FUELS.IFF
DATA.APPEARNC.GALAXY1.IFF
DATA.APPEARNC.GALAXY2.IFF
DATA.APPEARNC.GALAXY3.IFF
DATA.APPEARNC.GALAXY4.IFF
DATA.APPEARNC.GASGIANT.IFF
DATA.APPEARNC.GLADIUS.IFF
DATA.APPEARNC.GOTHRI.IFF
DATA.APPEARNC.HEATSEEK.IFF
DATA.APPEARNC.IMAGEREC.IFF
DATA.APPEARNC.IONIC.IFF
DATA.APPEARNC.JUMP.IFF
DATA.APPEARNC.JUMPFLSH.IFF
DATA.APPEARNC.KAMEKH.IFF
DATA.APPEARNC.LASER.IFF
DATA.APPEARNC.LUXURY.IFF
DATA.APPEARNC.MAGIC.IFF
DATA.APPEARNC.MARS.IFF
DATA.APPEARNC.MASS.IFF
DATA.APPEARNC.MEDEXPL.IFF
DATA.APPEARNC.MERCHANT.IFF
DATA.APPEARNC.MESON.IFF
DATA.APPEARNC.MICRO.IFF
DATA.APPEARNC.MOON1.IFF
DATA.APPEARNC.MOON2.IFF
DATA.APPEARNC.MOON3.IFF
DATA.APPEARNC.MRCHDETH.IFF
DATA.APPEARNC.NEBULA1.IFF
DATA.APPEARNC.NEBULA2.IFF
DATA.APPEARNC.NEBULA3.IFF
DATA.APPEARNC.NEUTRON.IFF
DATA.APPEARNC.OXFORD.IFF
DATA.APPEARNC.PARTICLE.IFF
DATA.APPEARNC.PERRY.IFF
DATA.APPEARNC.PLASMA.IFF
DATA.APPEARNC.PLEASURE.IFF
DATA.APPEARNC.PROCESS.IFF
DATA.APPEARNC.RAWMAT.IFF
DATA.APPEARNC.REFINE.IFF
DATA.APPEARNC.ROIDBASE.IFF
DATA.APPEARNC.SCOUT.IFF
DATA.APPEARNC.SKELETON.IFF
DATA.APPEARNC.SLAVES.IFF
DATA.APPEARNC.SMLEXPL.IFF
DATA.APPEARNC.STARS.PAK
DATA.APPEARNC.STARWHIT.IFF
DATA.APPEARNC.STILETTO.IFF
DATA.APPEARNC.SUNBIN1.IFF
DATA.APPEARNC.SUNBLUE.IFF
DATA.APPEARNC.SUNBRWN.IFF
DATA.APPEARNC.SUNRED.IFF
DATA.APPEARNC.SUNYEL.IFF
DATA.APPEARNC.TACHYON.IFF
DATA.APPEARNC.TALMIL.IFF
DATA.APPEARNC.TALPIR.IFF
DATA.APPEARNC.TALRELIG.IFF
DATA.APPEARNC.TORPEDO.IFF
DATA.APPEARNC.TRACTOR.IFF
DATA.APPEARNC.TRASH1.IFF
DATA.APPEARNC.TRASH2.IFF
DATA.APPEARNC.TRASH3.IFF
DATA.APPEARNC.TRASH4.IFF
DATA.APPEARNC.TUG.IFF
DATA.APPEARNC.TUGDETH.IFF
DATA.APPEARNC.URANUS.IFF
DATA.APPEARNC.WEAPONS.IFF
DATA.COCKPITS.CLUNKCK.IFF
DATA.COCKPITS.CLUNKCK.PAK
DATA.COCKPITS.COCKMISC.IFF
DATA.COCKPITS.FIGHTCK.IFF
DATA.COCKPITS.FIGHTCK.PAK
DATA.COCKPITS.ITTS.IFF
DATA.COCKPITS.MERCHCK.IFF
DATA.COCKPITS.MERCHCK.PAK
DATA.COCKPITS.PLAQUES.IFF
DATA.COCKPITS.SOFTWARE.IFF
DATA.COCKPITS.TUGCK.IFF
DATA.COCKPITS.TUGCK.PAK
DATA.CONV.AGRIRUMR.IFF
DATA.CONV.BASERUMR.IFF
DATA.CONV.BUZZOFF.IFF
DATA.CONV.GENRUMOR.IFF
DATA.CONV.MERCINFO.IFF
DATA.CONV.MINERUMR.IFF
DATA.CONV.MRCHINFO.IFF
DATA.CONV.PIRTRUMR.IFF
DATA.CONV.PLESRUMR.IFF
DATA.CONV.PLOTRUMR.IFF
DATA.CONV.REFIRUMR.IFF
DATA.CONV.RUMORS.IFF
DATA.CONV.*.PFC
DATA.CONV.*.VPF
DATA.CONV.*.VPK
DATA.CONV.SERRUM0.IFF
..
DATA.CONV.SERRUM7.IFF
DATA.CONV.TOOPOOR.PFC
DATA.CONV.TOOPOOR.VPK
DATA.MISSIONS.BFILMNGR.IFF
DATA.MISSIONS.PLOTMSNS.IFF
DATA.MISSIONS.S0MA.IFF
..
DATA.MISSIONS.S7MB.IFF
DATA.MISSIONS.TRADER.IFF
DATA.MISSIONS.UNIVERSE.IFF
DATA.SECTORS.BASES.IFF
DATA.SECTORS.QUADRANT.IFF
DATA.SECTORS.SECTORS.IFF
DATA.SECTORS.TABLE.DAT
DATA.SECTORS.TEAMS.IFF
DATA.TYPES.ASTRTYPE.IFF
DATA.TYPES.BASETYPE.IFF
DATA.TYPES.BEAMTYPE.IFF
DATA.TYPES.BRODTYPE.IFF
DATA.TYPES.CARGO.IFF
DATA.TYPES.CDBRTYPE.IFF
DATA.TYPES.CENTTYPE.IFF
DATA.TYPES.CPODTYPE.IFF
DATA.TYPES.DEMNTYPE.IFF
DATA.TYPES.DRALTYPE.IFF
DATA.TYPES.DRAYTYPE.IFF
DATA.TYPES.DRONTYPE.IFF
DATA.TYPES.DUSTTYPE.IFF
DATA.TYPES.ENERGY.IFF
DATA.TYPES.EXPLTYPE.IFF
DATA.TYPES.FLSHTYPE.IFF
DATA.TYPES.GLADTYPE.IFF
DATA.TYPES.GLXYTYPE.IFF
DATA.TYPES.GOTHTYPE.IFF
DATA.TYPES.IONCTYPE.IFF
DATA.TYPES.JUMPTYPE.IFF
DATA.TYPES.KAMKTYPE.IFF
DATA.TYPES.LASRTYPE.IFF
DATA.TYPES.MASSTYPE.IFF
DATA.TYPES.MESNTYPE.IFF
DATA.TYPES.MSSLTYPE.IFF
DATA.TYPES.NAVMAP.IFF
DATA.TYPES.NTRNTYPE.IFF
DATA.TYPES.ORIOTYPE.IFF
DATA.TYPES.PARATYPE.IFF
DATA.TYPES.PARTTYPE.IFF
DATA.TYPES.PIRATYPE.IFF
DATA.TYPES.PLSMTYPE.IFF
DATA.TYPES.SHIELDS.IFF
DATA.TYPES.SMALLSYS.IFF
DATA.TYPES.SSCTTYPE.IFF
DATA.TYPES.STILTYPE.IFF
DATA.TYPES.TALNTYPE.IFF
DATA.TYPES.TALNWIMP.IFF
DATA.TYPES.TARGETNG.IFF
DATA.TYPES.TARSTYPE.IFF
DATA.TYPES.TCHNTYPE.IFF
DATA.TYPES.TORPTYPE.IFF
DATA.TYPES.TRSHTYPE.IFF

*/