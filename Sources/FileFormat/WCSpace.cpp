
#include "Precomp.h"
#include "WCSpace.h"
#include "WCArchive.h"
#include "WCPak.h"
#include "FileEntryReader.h"

WCSpaceSprite::WCSpaceSprite(std::string name, WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\APPEARNC\\" + name + ".IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("APPR");

	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("FORM");
		std::string tag = reader.ReadTag();
		if (tag == "BMAP")
		{
			while (!reader.IsEndOfChunk())
			{
				std::string tag2 = reader.PushChunk();
				if (tag2 == "INFO")
				{
					info.resize(reader.GetChunkSize());
					reader.Read(info.data(), info.size());
				}
				else if (tag2 == "SHAP")
				{
					shape = std::make_unique<WCImage>(reader);
				}
				else if (tag2 == "SKEL")
				{
					skel.resize(reader.GetChunkSize());
					reader.Read(skel.data(), skel.size());
				}
				else if (tag2 == "FORM")
				{
					reader.ReadTag("MFDS");
					reader.PushChunk("TARG");
					target = std::make_unique<WCImage>(reader);
					reader.PopChunk();
				}
				else
				{
					throw std::runtime_error("Unknown sprite tag name");
				}
				reader.PopChunk();
			}
		}
		else if (tag == "BM3D")
		{
			throw std::runtime_error("Use WCSpaceShip for BM3D files");
		}
		else
		{
			throw std::runtime_error("Unknown sprite tag name");
		}
		reader.PopChunk();
	}

	reader.PopChunk();
}

WCSpaceSprite::~WCSpaceSprite()
{
}

////////////////////////////////////////////////////////////////////////////

WCSpaceShip::WCSpaceShip(std::string name, WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\APPEARNC\\" + name + ".IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("APPR");

	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("FORM");
		std::string tag = reader.ReadTag();
		if (tag == "BM3D")
		{
			while (!reader.IsEndOfChunk())
			{
				std::string tag2 = reader.PushChunk();
				if (tag2 == "INFO")
				{
					// data here
				}
				else if (tag2 == "VSHP")
				{
					size_t chunkoffset = reader.Tell();
					uint32_t chunksize = reader.GetChunkSize();

					uint32_t sectionoffset = 0;
					do
					{
						reader.Seek(chunkoffset + sectionoffset);
						uint32_t sectionsize = reader.ReadUint32();

						reader.Seek(chunkoffset + sectionoffset);
						shapes.push_back(std::make_unique<WCImage>(reader));

						sectionoffset += sectionsize;
					} while (sectionoffset + 4 <= chunksize);
				}
				else if (tag2 == "SKEL")
				{
					skel.resize(reader.GetChunkSize());
					reader.Read(skel.data(), skel.size());
				}
				else if (tag2 == "SCOR")
				{
					scor.resize(reader.GetChunkSize());
					reader.Read(scor.data(), scor.size());
				}
				else if (tag2 == "FORM")
				{
					reader.ReadTag("MFDS");
					reader.PushChunk("TARG");
					target = std::make_unique<WCImage>(reader);
					reader.PopChunk();
				}
				else if (tag2 == "BURN")
				{
					burn = std::make_unique<WCImage>(reader);
				}
				else
				{
					throw std::runtime_error("Unknown ship tag name");
				}
				reader.PopChunk();
			}
		}
		else if (tag == "BMAP")
		{
			throw std::runtime_error("Use WCSpaceSprite for BMAP files");
		}
		else
		{
			throw std::runtime_error("Unknown ship tag name");
		}
		reader.PopChunk();
	}

	reader.PopChunk();
}

WCSpaceShip::~WCSpaceShip()
{
}
