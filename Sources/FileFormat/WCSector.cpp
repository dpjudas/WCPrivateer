
#include "Precomp.h"
#include "WCSector.h"
#include "WCArchive.h"
#include "FileEntryReader.h"

WCSectorData::WCSectorData(WCArchive* archive)
{
	LoadBases(archive);
	LoadQuadrant(archive);
	LoadSectors(archive);
	LoadTable(archive);
	LoadTeams(archive);
}

void WCSectorData::LoadBases(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\SECTORS\\BASES.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("BASE");

	reader.PushChunk("DATA");
	reader.Read(basedata, 6);
	reader.PopChunk();

	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("INFO");
		WCSectorBase base;
		base.index = reader.ReadUint8();
		base.type = (WCBaseType)reader.ReadUint8();
		std::vector<char> buffer(reader.GetChunkSize() - 2 + 1);
		reader.Read(buffer.data(), reader.GetChunkSize() - 2);
		base.name = buffer.data();
		bases.push_back(std::move(base));
		reader.PopChunk();
	}

	reader.PopChunk();
}

void WCSectorData::LoadQuadrant(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\SECTORS\\QUADRANT.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("UNIV");

	reader.PushChunk("INFO");
	uint8_t quadrantCount = reader.ReadUint8();
	reader.PopChunk();

	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("FORM");
		reader.ReadTag("QUAD");

		WCQuadrant quad;

		reader.PushChunk("INFO");
		quad.a = reader.ReadInt16();
		quad.b = reader.ReadInt16();
		std::vector<char> buffer(reader.GetChunkSize() - 4 + 1);
		reader.Read(buffer.data(), reader.GetChunkSize() - 4);
		quad.name = buffer.data();
		reader.PopChunk();

		while (!reader.IsEndOfChunk())
		{
			reader.PushChunk("FORM");
			reader.ReadTag("SYST");

			WCSystem system;

			reader.PushChunk("INFO");
			system.a = reader.ReadUint8();
			system.b = reader.ReadInt16();
			system.c = reader.ReadInt16();
			std::vector<char> buffer2(reader.GetChunkSize() - 5 + 1);
			reader.Read(buffer2.data(), reader.GetChunkSize() - 5);
			system.name = buffer2.data();
			reader.PopChunk();

			if (!reader.IsEndOfChunk())
			{
				reader.PushChunk("BASE");
				std::vector<uint8_t> data(reader.GetChunkSize());
				reader.Read(data.data(), data.size());
				system.baseIndexes.resize(data.size());
				for (size_t i = 0, count = data.size(); i < count; i++)
					system.baseIndexes[i] = data[i];
				reader.PopChunk();
			}

			quad.systems.push_back(std::move(system));

			reader.PopChunk();
		}

		quadrants.push_back(std::move(quad));

		reader.PopChunk();
	}

	reader.PopChunk();
}

void WCSectorData::LoadSectors(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\SECTORS\\SECTORS.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("SSSS");
	reader.PushChunk("TABL");
	std::vector<uint32_t> table;
	int count = reader.GetChunkSize() / 4;
	for (int i = 0; i < count; i++)
	{
		table.push_back(reader.ReadUint32());
	}
	reader.PopChunk();
	reader.PopChunk(false);

	for (uint32_t offset : table)
	{
		if (offset == 0)
			break;

		reader.Seek(offset);
		reader.ReadUint32(); // Chunk size for what follows

		reader.PushChunk("FORM");
		reader.ReadTag("SCTR");

		WCSector sector;

		while (!reader.IsEndOfChunk())
		{
			std::string tag = reader.PushChunk();
			if (tag == "NAME")
			{
				sector.name = reader.ReadUint8();
			}
			else if (tag == "LABL")
			{
				std::vector<char> buffer(reader.GetChunkSize() + 1);
				reader.Read(buffer.data(), buffer.size() - 1);
				sector.label = buffer.data();
			}
			else if (tag == "RACE")
			{
				sector.race[0] = reader.ReadUint8();
				sector.race[1] = reader.ReadUint8();
			}
			else if (tag == "SPHR")
			{
				int count = reader.GetChunkSize() / 19;
				for (int i = 0; i < count; i++)
				{
					WCSectorSphere sphere;
					sphere.sphereIndex = reader.ReadUint8();
					sphere.sectorIndex = reader.ReadInt16();
					sphere.x = reader.ReadInt24();
					reader.ReadUint8();
					sphere.y = reader.ReadInt24();
					reader.ReadUint8();
					sphere.z = reader.ReadInt24();
					reader.ReadUint8();
					reader.Read(sphere.unknown4, 4);
					sector.spheres.push_back(sphere);
				}

				//sector.sphr.resize(reader.GetChunkSize());
				//reader.Read(sector.sphr.data(), sector.sphr.size());
			}
			else if (tag == "FORM")
			{
				std::string tag2 = reader.ReadTag();
				if (tag2 == "SCRP")
				{
					while (!reader.IsEndOfChunk())
					{
						std::string tag3 = reader.PushChunk();
						if (tag3 == "CAST")
						{
							char buffer[9] = {};
							while (!reader.IsEndOfChunk())
							{
								reader.Read(buffer, 8);
								sector.cast.push_back(buffer);
							}
						}
						else if (tag3 == "FLAG")
						{
							sector.flag = reader.ReadUint32();
						}
						else if (tag3 == "PROG")
						{
							sector.prog.resize(reader.GetChunkSize() / 2);
							reader.Read(sector.prog.data(), sector.prog.size() * 2);
						}
						else if (tag3 == "FORM")
						{
							std::string tag4 = reader.ReadTag();
							if (tag4 == "PART")
							{
								while (!reader.IsEndOfChunk())
								{
									std::string tag5 = reader.PushChunk();
									if (tag5 == "CNST")
									{
										sector.cnst.resize(reader.GetChunkSize());
										reader.Read(sector.cnst.data(), sector.cnst.size());
									}
									else if (tag5 == "WAND")
									{
										int count = reader.GetChunkSize() / 46;
										for (int i = 0; i < count; i++)
										{
											WCSectorEncounterItem item;
											item.chance = reader.ReadUint8();
											item.index = reader.ReadUint16();
											char buffer[9] = {};
											reader.Read(buffer, 8);
											item.type = buffer;
											reader.Read(buffer, 8);
											item.name = buffer;
											reader.Read(item.unknown2, 2);
											item.sphereIndex = reader.ReadUint8();
											reader.Read(item.unknown24, 24);
											sector.encounters.push_back(std::move(item));
										}
									}
									else if (tag5 == "JUMP")
									{
										int count = reader.GetChunkSize() / 46;
										for (int i = 0; i < count; i++)
										{
											WCSectorJumpItem item;
											item.index = reader.ReadUint16();
											char buffer[9] = {};
											reader.Read(buffer, 8);
											item.type = buffer;
											reader.Read(buffer, 8);
											item.name = buffer;
											reader.Read(item.unknown2, 2);
											item.sphereIndex = reader.ReadUint8();
											reader.Read(item.unknown25, 25);
											sector.jumps.push_back(std::move(item));
										}
									}
									else if (tag5 == "BASE")
									{
										int count = reader.GetChunkSize() / 46;
										for (int i = 0; i < count; i++)
										{
											WCSectorBaseItem item;
											item.a = reader.ReadUint8();
											item.b = reader.ReadUint8();
											char buffer[9] = {};
											reader.Read(buffer, 8);
											item.type = buffer;
											reader.Read(buffer, 8);
											item.name = buffer;
											reader.Read(item.unknown5, 5);
											item.x = reader.ReadInt24();
											reader.ReadUint8();
											item.y = reader.ReadInt24();
											reader.ReadUint8();
											item.z = reader.ReadInt24();
											reader.ReadUint8();
											reader.Read(item.unknown11, 11);
											sector.bases.push_back(std::move(item));
										}
									}
									else if (tag5 == "OJMP")
									{
										sector.ojmp.resize(reader.GetChunkSize());
										reader.Read(sector.ojmp.data(), sector.ojmp.size());
									}
									else
									{
										throw std::runtime_error("Unknown tag");
									}
									reader.PopChunk();
								}
							}
							else if (tag4 == "PLAY")
							{
								while (!reader.IsEndOfChunk())
								{
									reader.PushChunk("SCEN");
									std::vector<uint8_t> scene(reader.GetChunkSize());
									reader.Read(scene.data(), scene.size());
									sector.scenes.push_back(std::move(scene));
									reader.PopChunk(false);
								}
							}
							else if (tag4 == "LOAD")
							{
								char buffer[9] = {};

								reader.PushChunk("OBJT");
								while (!reader.IsEndOfChunk())
								{
									reader.Read(buffer, 8);
									sector.objtypes.push_back(buffer);
								}
								reader.PopChunk();

								reader.PushChunk("APPR");
								while (!reader.IsEndOfChunk())
								{
									reader.Read(buffer, 8);
									sector.appr.push_back(buffer);
								}
								reader.PopChunk();
							}
							else
							{
								throw std::runtime_error("Unknown tag");
							}
						}
						reader.PopChunk();
					}
				}
				else if (tag2 == "GLXY")
				{
					while (!reader.IsEndOfChunk())
					{
						std::string tag3 = reader.PushChunk();
						if (tag3 == "DUST")
						{
							// Always empty
						}
						else if (tag3 == "STRS")
						{
							char buffer[9] = {};
							reader.Read(buffer, 8);
							sector.starspak = buffer;
							sector.stars.resize((reader.GetChunkSize() - 8) / 2);
							reader.Read(sector.stars.data(), sector.stars.size() * 2);
						}
						else if (tag3 == "FORM")
						{
							reader.ReadTag("SUNS");
							while (!reader.IsEndOfChunk())
							{
								WCSectorSun sun;
								reader.PushChunk("SUNS");
								char buffer[9] = {};
								reader.Read(buffer, 8);
								sun.name = buffer;
								sun.a = reader.ReadInt16();
								sun.b = reader.ReadInt16();
								sun.c = reader.ReadInt16();
								sun.d = reader.ReadInt16();
								reader.PopChunk();
								sector.suns.push_back(std::move(sun));
							}
						}
						else
						{
							throw std::runtime_error("Unknown tag");
						}

						reader.PopChunk();
					}
				}
				else
				{
					throw std::runtime_error("Unknown tag");
				}
			}
			else
			{
				throw std::runtime_error("Unknown tag");
			}
			reader.PopChunk();
		}

		reader.PopChunk();
		sectors.push_back(std::move(sector));
	}
}

void WCSectorData::LoadTable(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\SECTORS\\TABLE.DAT");
	while (true)
	{
		WCSectorTableEntry entry;
		reader.Read(entry.data, 68);
		table.push_back(entry);
		uint8_t value = reader.ReadUint8();
		if (value == 0)
			break;
	}
}

void WCSectorData::LoadTeams(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\SECTORS\\TEAMS.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("TEAM");
	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("FRMN");
		WCSectorTeam team;
		team.data.resize(reader.GetChunkSize());
		reader.Read(team.data.data(), team.data.size());
		teams.push_back(std::move(team));
		reader.PopChunk();
	}
	reader.PopChunk();
}
