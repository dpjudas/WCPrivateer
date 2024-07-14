
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
						reader.PopChunk();
					}
					else if (tag2 == "SOFT")
					{
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
					}
					else if (tag2 == "FORM")
					{
						std::string tag3 = reader.ReadTag();
						if (tag3 == "HSFT")
						{
							while (!reader.IsEndOfChunk())
							{
								std::string tag4 = reader.PushChunk();
								if (tag4 == "SSPD")
								{
									reader.PushChunk("INFO");
									reader.PopChunk();
								}
								else if (tag4 == "FORM")
								{
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
			}
			else if (tag == "DAMG")
			{
				reader.PushChunk("DAMG");
				reader.PopChunk();
			}
			else if (tag == "CDMG")
			{
				reader.PushChunk("EXPL");
				reader.PopChunk();
				reader.PushChunk("SPRT");
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
