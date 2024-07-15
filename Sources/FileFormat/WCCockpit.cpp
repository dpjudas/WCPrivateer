
#include "Precomp.h"
#include "WCCockpit.h"
#include "WCArchive.h"
#include "WCPak.h"
#include "FileEntryReader.h"

WCCockpit::WCCockpit(std::string name, WCArchive* archive)
{
	WCPak pak("DATA\\COCKPITS\\" + name + ".PAK", archive);

	FileEntryReader reader = archive->openFile("DATA\\COCKPITS\\" + name + ".IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("COCK");

	while (!reader.IsEndOfChunk())
	{
		std::string chunktag = reader.PushChunk();
		if (chunktag == "FORM")
		{
			std::string tag = reader.ReadTag();
			if (tag == "FRNT" || tag == "RITE" || tag == "BACK" || tag == "LEFT" || tag == "TUR1" || tag == "TUR2")
			{
				WCCockpitViewpoint* viewpoint = &front;
				if (tag == "FRNT") viewpoint = &front;
				else if (tag == "RITE") viewpoint = &right;
				else if (tag == "BACK") viewpoint = &back;
				else if (tag == "LEFT") viewpoint = &left;
				else if (tag == "TUR1") viewpoint = &turret1;
				else if (tag == "TUR2") viewpoint = &turret2;

				while (!reader.IsEndOfChunk())
				{
					std::string tag2 = reader.PushChunk();
					if (tag2 == "SHAP")
					{
						viewpoint->shape = std::make_unique<WCImage>(reader);
					}
					else if (tag2 == "TPLT")
					{
						viewpoint->tplt.resize(reader.GetChunkSize());
						reader.Read(viewpoint->tplt.data(), viewpoint->tplt.size());
					}
					else
					{
						throw std::runtime_error("Unknown cockpit tag name");
					}
					reader.PopChunk();
				}
			}
			else if (tag == "TRRT")
			{
				reader.PushChunk("INFO");
				trrt.unknown0 = reader.ReadUint32();
				trrt.unknown1 = reader.ReadUint32();
				reader.PopChunk();
			}
			else if (tag == "CMFD")
			{
				while (!reader.IsEndOfChunk())
				{
					std::string tag2 = reader.PushChunk();
					if (tag2 == "FORM")
					{
						reader.ReadTag("AMFD");
						reader.PushChunk("INFO");
						cmfd.x0 = reader.ReadInt16();
						cmfd.y0 = reader.ReadInt16();
						cmfd.x1 = reader.ReadInt16();
						cmfd.y1 = reader.ReadInt16();
						cmfd.unknown[0] = reader.ReadUint8();
						cmfd.unknown[1] = reader.ReadUint8();
						cmfd.unknown[2] = reader.ReadUint8();
						reader.PopChunk();
					}
					else if (tag2 == "SOFT")
					{
						std::vector<char> buffer(reader.GetChunkSize() + 1);
						reader.Read(buffer.data(), reader.GetChunkSize());
						software = buffer.data();
					}
					else
					{
						throw std::runtime_error("Unknown cockpit tag name");
					}
					reader.PopChunk();
				}
			}
			else if (tag == "CHUD")
			{
				while (!reader.IsEndOfChunk())
				{
					std::string tag2 = reader.PushChunk();
					if (tag2 == "HINF")
					{
						chud.hinf.resize(reader.GetChunkSize());
						reader.Read(chud.hinf.data(), chud.hinf.size());
					}
					else if (tag2 == "FORM")
					{
						std::string tag3 = reader.ReadTag();
						if (tag3 == "HSFT")
						{
							while (!reader.IsEndOfChunk())
							{
								std::string tag4 = reader.PushChunk();
								if (tag4 == "FORM")
								{
									std::string tag5 = reader.ReadTag();
									if (tag5 == "SSPD")
									{
										reader.PushChunk("INFO");
										setSpeed.x = reader.ReadInt16();
										setSpeed.y = reader.ReadInt16();
										char buffer[6] = {};
										reader.Read(buffer, 5);
										setSpeed.text = buffer;
										reader.PopChunk();
									}
									else if (tag5 == "ASPD")
									{
										reader.PushChunk("INFO");
										actualSpeed.x = reader.ReadInt16();
										actualSpeed.y = reader.ReadInt16();
										char buffer[6] = {};
										reader.Read(buffer, 5);
										actualSpeed.text = buffer;
										reader.PopChunk();
									}
									else if (tag5 == "TRGT")
									{
										reader.PushChunk("INFO");
										chud.trgtinfo.resize(reader.GetChunkSize());
										reader.Read(chud.trgtinfo.data(), chud.trgtinfo.size());
										reader.PopChunk();

										reader.PushChunk("FORM");
										reader.ReadTag("ITTS");

										reader.PushChunk("GUNS");
										guns.resize(reader.GetChunkSize());
										reader.Read(guns.data(), guns.size());
										reader.PopChunk();
										reader.PushChunk("SHAP");
										itts = std::make_unique<WCImage>(reader);
										reader.PopChunk();

										reader.PopChunk();
									}
									else if (tag5 == "CRSS")
									{
										reader.PushChunk("SHAP");
										crosshair = std::make_unique<WCImage>(reader);
										reader.PopChunk();
									}
									else if (tag5 == "NAVI")
									{
										reader.PushChunk("SHAP");
										navigationCrosshair = std::make_unique<WCImage>(reader);
										reader.PopChunk();
									}
									else
									{
										throw std::runtime_error("Unknown cockpit tag name");
									}
								}
								else
								{
									throw std::runtime_error("Unknown cockpit tag name");
								}
								reader.PopChunk();
							}
						}
						else if (tag3 == "DAMG")
						{
							reader.PushChunk("DAMG");
							chud.damg.resize(reader.GetChunkSize());
							reader.Read(chud.damg.data(), chud.damg.size());
							reader.PopChunk();
						}
						else
						{
							throw std::runtime_error("Unknown cockpit tag name");
						}
					}
					else
					{
						throw std::runtime_error("Unknown cockpit tag name");
					}
					reader.PopChunk();
				}
			}
			else if (tag == "DIAL")
			{
				while (!reader.IsEndOfChunk())
				{
					reader.PushChunk("FORM");
					std::string tag2 = reader.ReadTag();
					if (tag2 == "RADR")
					{
						reader.PushChunk("INFO");
						radarPos.x0 = reader.ReadInt16();
						radarPos.y0 = reader.ReadInt16();
						radarPos.x1 = reader.ReadInt16();
						radarPos.y1 = reader.ReadInt16();
						reader.PopChunk();
					}
					else if (tag2 == "SHLD")
					{
						reader.PushChunk("INFO");
						shieldPos.x0 = reader.ReadInt16();
						shieldPos.y0 = reader.ReadInt16();
						shieldPos.x1 = reader.ReadInt16();
						shieldPos.y1 = reader.ReadInt16();
						reader.PopChunk();

						reader.PushChunk("DATA");
						dial.shld.data.resize(reader.GetChunkSize());
						reader.Read(dial.shld.data.data(), dial.shld.data.size());
						reader.PopChunk();

						reader.PushChunk("SHAP");
						shield = std::make_unique<WCImage>(reader);
						reader.PopChunk();
					}
					else if (tag2 == "ENER")
					{
						while (!reader.IsEndOfChunk())
						{
							reader.PushChunk("FORM");
							reader.ReadTag("VIEW");

							WCCockpitEnergyView view;

							reader.PushChunk("INFO");
							view.x0 = reader.ReadInt16();
							view.y0 = reader.ReadInt16();
							view.x1 = reader.ReadInt16();
							view.y1 = reader.ReadInt16();
							//view.info.resize(reader.GetChunkSize());
							//reader.Read(view.info.data(), view.info.size());
							reader.PopChunk();

							reader.PushChunk("DATA");
							view.data.resize(reader.GetChunkSize());
							reader.Read(view.data.data(), view.data.size());
							reader.PopChunk();

							reader.PushChunk("SHAP");
							view.shape = std::make_unique<WCImage>(reader);
							reader.PopChunk();

							energy.push_back(std::move(view));

							if (!reader.IsEndOfChunk())
								throw std::runtime_error("More data in ENER chunk");
							reader.PopChunk();
						}
					}
					else if (tag2 == "FUEL")
					{
						reader.PushChunk("FORM");
						reader.ReadTag("VIEW");

						reader.PushChunk("INFO");
						fuelPos.x0 = reader.ReadInt16();
						fuelPos.y0 = reader.ReadInt16();
						fuelPos.x1 = reader.ReadInt16();
						fuelPos.y1 = reader.ReadInt16();
						reader.PopChunk();

						reader.PushChunk("DATA");
						dial.fuel.data.resize(reader.GetChunkSize());
						reader.Read(dial.fuel.data.data(), dial.fuel.data.size());
						reader.PopChunk();

						reader.PushChunk("SHAP");
						fuel = std::make_unique<WCImage>(reader);
						reader.PopChunk();

						if (!reader.IsEndOfChunk())
							throw std::runtime_error("More data in FUEL chunk");
						reader.PopChunk();
					}
					else if (tag2 == "AUTO")
					{
						reader.PushChunk("INFO");
						autoPos.x0 = reader.ReadInt16();
						autoPos.y0 = reader.ReadInt16();
						autoPos.x1 = reader.ReadInt16();
						autoPos.y1 = reader.ReadInt16();
						reader.PopChunk();

						reader.PushChunk("SHAP");
						autopilot = std::make_unique<WCImage>(reader);
						reader.PopChunk();

						if (!reader.IsEndOfChunk())
							throw std::runtime_error("More data in AUTO chunk");
					}
					else
					{
						throw std::runtime_error("Unknown cockpit tag name");
					}
					reader.PopChunk();
				}
			}
			else if (tag == "DAMG")
			{
				reader.PushChunk("DAMG");
				damg.resize(reader.GetChunkSize());
				reader.Read(damg.data(), damg.size());
				reader.PopChunk();
			}
			else if (tag == "CDMG")
			{
				reader.PushChunk("EXPL");
				cdmg.explosion.resize(reader.GetChunkSize());
				reader.Read(cdmg.explosion.data(), cdmg.explosion.size());
				reader.PopChunk();
				reader.PushChunk("SPRT");
				cdmg.sprite.resize(reader.GetChunkSize());
				reader.Read(cdmg.sprite.data(), cdmg.sprite.size());
				reader.PopChunk();
			}
			else
			{
				throw std::runtime_error("Unknown cockpit tag name");
			}
		}
		else if (chunktag == "OFFS")
		{
			offset[0] = reader.ReadInt16();
			offset[1] = reader.ReadInt16();
		}
		else if (chunktag == "FONT")
		{
			std::vector<char> buffer(reader.GetChunkSize() + 1);
			reader.Read(buffer.data(), reader.GetChunkSize());
			font = buffer.data();
		}
		else
		{
			throw std::runtime_error("Unknown cockpit tag name");
		}
		reader.PopChunk();
	}

	reader.PopChunk();
}

WCCockpit::~WCCockpit()
{
}

//////////////////////////////////////////////////////////////////////////////

WCCockpitSoftware::WCCockpitSoftware(std::string name, WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\COCKPITS\\" + name + ".IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("SOFT");

	reader.PushChunk("FORM");
	reader.ReadTag("STND");

	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("FORM");
		std::string tag = reader.ReadTag();
		if (tag == "COMM")
		{
			reader.PushChunk("INFO");
			commInfo.resize(reader.GetChunkSize());
			reader.Read(commInfo.data(), commInfo.size());
			reader.PopChunk();

			if (!reader.IsEndOfChunk())
				throw std::runtime_error("More data in COMM chunk");
		}
		else if (tag == "RECV")
		{
			reader.PushChunk("INFO");
			recvInfo.resize(reader.GetChunkSize());
			reader.Read(recvInfo.data(), recvInfo.size());
			reader.PopChunk();

			if (!reader.IsEndOfChunk())
				throw std::runtime_error("More data in RECV chunk");
		}
		else if (tag == "WEAP")
		{
			reader.PushChunk("INFO");
			weaponInfo.resize(reader.GetChunkSize());
			reader.Read(weaponInfo.data(), weaponInfo.size());
			reader.PopChunk();

			reader.PushChunk("GUNS");
			guns = std::make_unique<WCImage>(reader);
			reader.PopChunk();

			reader.PushChunk("WEAP");
			weapons = std::make_unique<WCImage>(reader);
			reader.PopChunk();

			if (!reader.IsEndOfChunk())
				throw std::runtime_error("More data in WEAP chunk");
		}
		else if (tag == "DMGD")
		{
			reader.PushChunk("INFO");
			damagedInfo.resize(reader.GetChunkSize());
			reader.Read(damagedInfo.data(), damagedInfo.size());
			reader.PopChunk();

			reader.PushChunk("FORM");
			reader.ReadTag("STRG");

			reader.PushChunk("SNUM");
			reader.PopChunk();

			reader.PushChunk("DATA");
			std::vector<char> buffer(reader.GetChunkSize());
			reader.Read(buffer.data(), buffer.size());
			std::vector<std::string> strings;
			std::string text;
			for (char c : buffer)
			{
				if (c == 0)
				{
					strings.push_back(text);
					text.clear();
				}
				else
				{
					text.push_back(c);
				}
			}
			if (!text.empty())
				strings.push_back(text);
			damagedText = std::move(strings);
			reader.PopChunk();

			if (!reader.IsEndOfChunk())
				throw std::runtime_error("More data in STRG chunk");

			reader.PopChunk();

			if (!reader.IsEndOfChunk())
				throw std::runtime_error("More data in DMGD chunk");
		}
		else if (tag == "CRGO")
		{
			reader.PushChunk("INFO");
			cargoInfo.resize(reader.GetChunkSize());
			reader.Read(cargoInfo.data(), cargoInfo.size());
			reader.PopChunk();

			reader.PushChunk("NAME");
			std::vector<char> buffer(reader.GetChunkSize());
			reader.Read(buffer.data(), buffer.size());
			std::vector<std::string> strings;
			std::string text;
			for (char c : buffer)
			{
				if (c == 0)
				{
					strings.push_back(text);
					text.clear();
				}
				else
				{
					text.push_back(c);
				}
			}
			if (!text.empty())
				strings.push_back(text);
			cargoNames = std::move(strings);
			reader.PopChunk();

			if (!reader.IsEndOfChunk())
				throw std::runtime_error("More data in CRGO chunk");
		}
		else if (tag == "DEST")
		{
			reader.PushChunk("INFO");
			destInfo.resize(reader.GetChunkSize());
			reader.Read(destInfo.data(), destInfo.size());
			reader.PopChunk();

			reader.PushChunk("FORM");
			reader.ReadTag("STRG");

			reader.PushChunk("SNUM");
			reader.PopChunk();

			reader.PushChunk("DATA");
			std::vector<char> buffer(reader.GetChunkSize());
			reader.Read(buffer.data(), buffer.size());
			std::vector<std::string> strings;
			std::string text;
			for (char c : buffer)
			{
				if (c == 0)
				{
					strings.push_back(text);
					text.clear();
				}
				else
				{
					text.push_back(c);
				}
			}
			if (!text.empty())
				strings.push_back(text);
			destText = std::move(strings);
			reader.PopChunk();

			if (!reader.IsEndOfChunk())
				throw std::runtime_error("More data in STRG chunk");

			reader.PopChunk();

			if (!reader.IsEndOfChunk())
				throw std::runtime_error("More data in DEST chunk");
		}
		else
		{
			throw std::runtime_error("Unknown cockpit software tag name");
		}
		reader.PopChunk();
	}

	reader.PopChunk();

	reader.PushChunk("FORM");
	reader.ReadTag("OPTN");

	reader.PushChunk("FORM");
	reader.ReadTag("TRAK");
	reader.PushChunk("INFO");
	trackInfo.resize(reader.GetChunkSize());
	reader.Read(trackInfo.data(), trackInfo.size());
	reader.PopChunk();
	reader.PopChunk();

	reader.PushChunk("FORM");
	reader.ReadTag("CAMS");
	reader.PushChunk("INFO");
	camerasInfo.resize(reader.GetChunkSize());
	reader.Read(camerasInfo.data(), camerasInfo.size());
	reader.PopChunk();
	reader.PopChunk();

	reader.PopChunk();

	reader.PopChunk();
}

WCCockpitSoftware::~WCCockpitSoftware()
{
}

//////////////////////////////////////////////////////////////////////////////

WCCockpitMisc::WCCockpitMisc(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\COCKPITS\\COCKMISC.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("MISC");
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
		
		reader.PushChunk("FORM");
		reader.ReadTag("EXPL");

		reader.PushChunk("INFO");
		explosionInfo.resize(reader.GetChunkSize());
		reader.Read(explosionInfo.data(), explosionInfo.size());
		reader.PopChunk();

		reader.PushChunk("SHAP");
		explosionShape = std::make_unique<WCImage>(reader);
		reader.PopChunk();

		reader.PopChunk();
	}
}

WCCockpitMisc::~WCCockpitMisc()
{
}

//////////////////////////////////////////////////////////////////////////////

WCCockpitPlaques::WCCockpitPlaques(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\COCKPITS\\PLAQUES.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("PLAQ");

	while (!reader.IsEndOfChunk())
	{
		std::string tag = reader.PushChunk();
		if (tag == "FORM")
		{
			std::string tag2 = reader.ReadTag();
			if (tag2 == "FONT")
			{
				std::string name = reader.ReadTag();
				reader.ReadUint32(); // all zeros

				reader.PushChunk("SHAP");
				fonts[name] = std::make_unique<WCImage>(reader);
				reader.PopChunk();

				if (!reader.IsEndOfChunk())
					throw std::runtime_error("More data in FONT chunk");
			}
			else
			{
				throw std::runtime_error("Unknown cockpit plaques tag name");
			}
		}
		else if (tag == "INFO")
		{
			info.resize(reader.GetChunkSize());
			reader.Read(info.data(), info.size());
		}
		else if (tag == "SHAP")
		{
			shape = std::make_unique<WCImage>(reader);
		}
		else
		{
			throw std::runtime_error("Unknown cockpit plaques tag name");
		}

		reader.PopChunk();
	}

	reader.PopChunk();
}

WCCockpitPlaques::~WCCockpitPlaques()
{
}
