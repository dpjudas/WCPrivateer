
#include "Precomp.h"
#include "WCMovie.h"
#include "WCArchive.h"
#include "FileEntryReader.h"

WCMovie::WCMovie(const std::string& filename, WCArchive* archive)
{
	FileEntryReader reader = archive->openFile(filename);
	reader.PushChunk("FORM");
	reader.ReadTag("MOVI");
	while (!reader.IsEndOfChunk())
	{
		std::string tag = reader.PushChunk();
		if (tag == "CLRC")
		{
			clrc = reader.ReadInt16();
		}
		else if (tag == "SPED")
		{
			speed = reader.ReadInt16();
		}
		else if (tag == "FILE")
		{
			while (!reader.IsEndOfChunk())
			{
				WCMovieFile file;
				file.slot = reader.ReadInt16();
				while (true)
				{
					char c = reader.ReadUint8();
					if (c == 0)
						break;
					file.filename.push_back(c);
				}
				files.push_back(std::move(file));
			}
		}
		else if (tag == "FILD")
		{
			while (!reader.IsEndOfChunk())
			{
				fild.push_back(reader.ReadInt16());
			}
		}
		else if (tag == "SPRI")
		{
			while (!reader.IsEndOfChunk())
			{
				spri.push_back(reader.ReadUint8());
			}
		}
		else if (tag == "FORM")
		{
			tag = reader.ReadTag();
			if (tag == "ACTS")
			{
				while (!reader.IsEndOfChunk())
				{
					tag = reader.PushChunk();
					if (tag == "AFTR")
					{
						while (!reader.IsEndOfChunk())
						{
							acts.aftr.push_back(reader.ReadInt16());
						}
					}
					else if (tag == "BFOR")
					{
						while (!reader.IsEndOfChunk())
						{
							acts.bfor.push_back(reader.ReadInt16());
						}
					}
					else if (tag == "FILD")
					{
						while (!reader.IsEndOfChunk())
						{
							acts.fild.push_back(reader.ReadInt16());
						}
					}
					else if (tag == "SPRI")
					{
						while (!reader.IsEndOfChunk())
						{
							acts.spri.push_back(reader.ReadUint8());
						}
					}
					else if (tag == "FORM")
					{
						reader.ReadTag("SCEN");
						WCMovieScene scene;
						while (!reader.IsEndOfChunk())
						{
							tag = reader.PushChunk();
							if (tag == "FILD")
							{
								while (!reader.IsEndOfChunk())
								{
									scene.fild.push_back(reader.ReadInt16());
								}
							}
							else if (tag == "SPRI")
							{
								while (!reader.IsEndOfChunk())
								{
									scene.spri.push_back(reader.ReadUint8());
								}
							}
							else if (tag == "BFOR")
							{
								while (!reader.IsEndOfChunk())
								{
									scene.bfor.push_back(reader.ReadInt16());
								}
							}
							else
							{
								throw std::runtime_error("Unexpected tag");
							}
							reader.PopChunk();
						}
						acts.scenes.push_back(std::move(scene));
					}
					else
					{
						throw std::runtime_error("Unexpected tag");
					}
					reader.PopChunk();
				}
			}
			else
			{
				throw std::runtime_error("Unexpected tag");
			}
		}
		else
		{
			throw std::runtime_error("Unexpected tag");
		}
		reader.PopChunk();
	}
	reader.PopChunk();
}

/////////////////////////////////////////////////////////////////////////////

WCShipToName::WCShipToName(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\MIDGAMES\\SHIP2CHR.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("MIDG");
	reader.PushChunk("INFO");
	while (!reader.IsEndOfChunk())
	{
		char name[9] = {};
		for (int i = 0; i < 8; i++)
			name[i] = reader.ReadUint8();

		int index = reader.ReadUint16();
		names[index] = name;
	}
	reader.PopChunk();
	reader.PopChunk();
}

/////////////////////////////////////////////////////////////////////////////

WCMidgames::WCMidgames(const std::string& filename, WCArchive* archive)
{
	FileEntryReader reader = archive->openFile(filename);
	reader.PushChunk("FORM");
	reader.ReadTag("MIDG");
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

		reader.PushChunk("FNAM");
		std::string filename;
		while (!reader.IsEndOfChunk())
		{
			char c = reader.ReadUint8();
			if (c == 0)
				break;
			filename.push_back(c);
		}
		filenames.push_back(filename);
		reader.PopChunk();
	}
}

/////////////////////////////////////////////////////////////////////////////

WCDeathAppearance::WCDeathAppearance(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\MIDGAMES\\DEATHAPPR.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("DETH");
	reader.PushChunk("INFO");
	while (!reader.IsEndOfChunk())
	{
		char key[9] = {};
		for (int i = 0; i < 8; i++)
			key[i] = reader.ReadUint8();

		char value[9] = {};
		for (int i = 0; i < 8; i++)
			value[i] = reader.ReadUint8();

		deathNames[key] = value;
	}
	reader.PopChunk();
	reader.PopChunk();
}
