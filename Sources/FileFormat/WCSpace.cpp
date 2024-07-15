
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
					// data here
				}
				else if (tag2 == "SHAP")
				{
					shape = std::make_unique<WCImage>(reader);
				}
				else if (tag2 == "SKEL")
				{
				}
				else if (tag2 == "FORM")
				{
					reader.ReadTag("MFDS");
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
			throw std::runtime_error("Implement BM3D loading");
		}
		else
		{
			throw std::runtime_error("Unknown sprite tag name");
		}
		reader.PopChunk();
	}

	/*
	reader.PushChunk("FORM");
	reader.ReadTag("PAL ");
	reader.ReadTag("CMAP");
	// data here
	reader.PopChunk();
	*/

	reader.PopChunk();
}

WCSpaceSprite::~WCSpaceSprite()
{
}
