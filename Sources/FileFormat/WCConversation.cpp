
#include "Precomp.h"
#include "WCConversation.h"
#include "WCArchive.h"
#include "WCPak.h"
#include "WCSound.h"
#include "FileEntryReader.h"
#include <lzw/lzw.h>

WCConvBackgroundList::WCConvBackgroundList(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\OPTIONS\\CU.IFF");

	reader.PushChunk("FORM");
	reader.ReadTag("FILE");

	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("FORM");
		reader.ReadTag("CUBG");
		WCConvBackground bg;
		while (!reader.IsEndOfChunk())
		{
			std::string tag = reader.PushChunk();
			if (tag == "SHAP")
			{
				char buffer[8];
				reader.Read(buffer, 8);
				std::string filename;
				for (uint32_t i = 0; i < 8; i++)
				{
					char c = buffer[i];
					if (c == 0)
						break;
					filename.push_back(c);
				}
				bg.shape = filename;
				bg.pakindex = reader.ReadUint16();
				if (!reader.IsEndOfChunk())
					throw std::runtime_error("Premature end of SHAP");
			}
			else if (tag == "PALT")
			{
				bg.palette = std::make_unique<WCPalette>(reader);
			}
			else
			{
				throw std::runtime_error("Unknown tag");
			}
			reader.PopChunk();
		}
		backgrounds.push_back(std::move(bg));
		reader.PopChunk();
	}

	reader.PopChunk();

	reader.PushChunk("FORM");
	reader.ReadTag("BACK");
	reader.PushChunk("DATA");

	for (uint32_t i = 0, count = reader.GetChunkSize() / 9; i < count; i++)
	{
		uint8_t index = reader.ReadUint8();
		char buffer[8];
		reader.Read(buffer, 8);
		std::string text;
		for (uint32_t i = 0; i < 8; i++)
		{
			char c = buffer[i];
			if (c == 0)
				break;
			text.push_back(c);
		}
		backdata.push_back({ index, text });
	}

	reader.PopChunk();
	reader.PopChunk();
}

/////////////////////////////////////////////////////////////////////////////

WCConvFaceList::WCConvFaceList(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\OPTIONS\\FACES.IFF");

	reader.PushChunk("FORM");
	reader.ReadTag("FACE");

	while (!reader.IsEndOfChunk())
	{
		std::string tag = reader.PushChunk();
		if (tag == "PALT")
		{
			if (palette)
				throw std::runtime_error("PALT encountered more than once");
			palette = std::make_unique<WCPalette>(reader);
		}
		else if (tag == "FORM")
		{
			reader.ReadTag("SHAP");

			WCConvFace face;
			while (!reader.IsEndOfChunk())
			{
				std::string tag = reader.PushChunk();
				if (tag == "DATA")
				{
					char buffer[12];
					reader.Read(buffer, 12);
					std::string name;
					for (uint32_t i = 0; i < 12; i++)
					{
						char c = buffer[i];
						if (c == 0)
							break;
						name.push_back(c);
					}
					face.name = std::move(name);

					if (buffer[8] != 0 || buffer[9] != 0 || buffer[10] != 0 || buffer[11] != 0)
						throw std::runtime_error("Hmm!");

					face.eyesX = reader.ReadInt16();
					face.eyesY = reader.ReadInt16();
					face.mouthX = reader.ReadInt16();
					face.mouthY = reader.ReadInt16();
					face.uniformX = reader.ReadInt16();
					face.uniformY = reader.ReadInt16();
					face.unknown0X = reader.ReadInt16();
					face.unknown0Y = reader.ReadInt16();
					face.unknown1X = reader.ReadInt16();
					face.unknown1Y = reader.ReadInt16();
					face.unknown2X = reader.ReadInt16();
					face.unknown2Y = reader.ReadInt16();
					face.handX = reader.ReadInt16();
					face.handY = reader.ReadInt16();
				}
				else if (tag == "HEAD")
				{
					face.head = reader.ReadUint16();
				}
				else if (tag == "EYES")
				{
					face.eyes = reader.ReadUint16();
				}
				else if (tag == "MOTH")
				{
					face.mouth = reader.ReadUint16();
				}
				else if (tag == "UNIF")
				{
					face.uniform = reader.ReadUint16();
				}
				else if (tag == "HAND")
				{
					face.hand = reader.ReadUint16();
				}
				else
				{
					throw std::runtime_error("Unknown tag");
				}
				reader.PopChunk();
			}
			faces.push_back(std::move(face));
		}
		else
		{
			throw std::runtime_error("Unknown tag");
		}
		reader.PopChunk();
	}

	reader.PopChunk();
}

/////////////////////////////////////////////////////////////////////////////

WCConvEyesList::WCConvEyesList(WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\OPTIONS\\EYES.IFF");

	reader.PushChunk("FORM");
	reader.ReadTag("FACE");
	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("FORM");
		reader.ReadTag("XPRS");

		WCConvEyes eyes;

		while (!reader.IsEndOfChunk())
		{
			std::string tag = reader.PushChunk();
			if (tag == "INFO")
			{
				char buffer[8];
				reader.Read(buffer, 8);
				std::string name;
				for (uint32_t i = 0; i < 8; i++)
				{
					char c = buffer[i];
					if (c == 0)
						break;
					name.push_back(c);
				}
				eyes.name = std::move(name);
				eyes.unknown = reader.ReadUint16();
			}
			else if (tag == "EYES")
			{
				eyes.eyes.resize(reader.GetChunkSize());
				reader.Read(eyes.eyes.data(), eyes.eyes.size());
			}
			else
			{
				throw std::runtime_error("Unknown tag");
			}
			reader.PopChunk();
		}
		reader.PopChunk();

		eyesList.push_back(std::move(eyes));
	}
	reader.PopChunk();

	reader.PushChunk("FORM");
	reader.ReadTag("EYES");
	while (!reader.IsEndOfChunk())
	{
		reader.PushChunk("DEFS");

		WCConvEyesDef def;
		def.def.resize(reader.GetChunkSize());
		reader.Read(def.def.data(), def.def.size());

		reader.PopChunk();
		defs.push_back(std::move(def));
	}
	reader.PopChunk();

	reader.PushChunk("FORM");
	reader.ReadTag("MEDF");
	reader.PushChunk("DATA");
	medfdata.resize(reader.GetChunkSize());
	reader.Read(medfdata.data(), medfdata.size());
	reader.PopChunk();
	reader.PopChunk();
}

/////////////////////////////////////////////////////////////////////////////

WCConversation::WCConversation(const std::string& name, WCArchive* archive)
{
	FileEntryReader reader = archive->openFile("DATA\\CONV\\" + name + ".PFC");

	if (reader.ReadUint8() != 0) // always zero. Not sure what its purpose is
		throw std::runtime_error("Unexpected conversation start byte");

	while (reader.Tell() != reader.Size())
	{
		char actor[10] = {};
		char normal[10] = {};
		char location[10] = {};
		reader.Read(actor, 9);
		reader.Read(normal, 9);
		reader.Read(location, 9);

		WCConversationStep step;
		step.actorname = actor;
		step.normal = normal;
		step.location = location;
		step.unknown0 = reader.ReadInt16();
		step.unknown1 = reader.ReadInt16();

		while (reader.Tell() != reader.Size())
		{
			char c = reader.ReadUint8();
			if (c == 0)
				break;
			step.text.push_back(c);
		}
		
		// What is this?
		while (reader.Tell() != reader.Size())
		{
			uint8_t c = reader.ReadUint8();
			if (c == 0)
				break;
			step.unknown2.push_back(c);
		}

		steps.push_back(std::move(step));
	}

	reader = archive->openFile("DATA\\CONV\\" + name + ".VPK");

	uint32_t paksize = reader.ReadUint32();
	if (paksize != reader.Size())
		return;

	std::vector<size_t> offsets;
	uint32_t firstoffset = reader.ReadUint24();
	uint8_t unknown = reader.ReadUint8();
	offsets.push_back(firstoffset);

	uint32_t count = (firstoffset - 4) / 4;
	for (uint32_t i = 1; i < count; i++)
	{
		uint32_t offset = reader.ReadUint24();
		uint8_t unknown = reader.ReadUint8();
		if (offset < firstoffset || offset >= paksize)
			break;
		offsets.push_back(offset);
	}
	offsets.push_back(paksize);

	int filecount = (int)offsets.size() - 1;
	for (int index = 0; index < filecount; index++)
	{
		size_t entrysize = offsets[index + 1] - offsets[index];
		if (entrysize == 0)
			continue;

		reader.Seek(offsets[index]);
		uint32_t uncompressedsize = reader.ReadUint32();

		std::vector<uint8_t> input(entrysize);
		reader.Read(input.data(), input.size());

		std::vector<uint8_t> output(uncompressedsize);

		lzw_state state = {};
		int64_t result = lzw_decompress(&state, input.data(), input.size(), output.data(), output.size());
		if (result < 0)
			continue;

		sounds.emplace_back(FileEntryReader(std::move(output)));
	}
}
