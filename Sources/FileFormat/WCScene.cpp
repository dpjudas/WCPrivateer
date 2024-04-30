
#include "Precomp.h"
#include "WCScene.h"
#include "WCArchive.h"
#include "WCPak.h"
#include "WCPalette.h"
#include "FileEntryReader.h"

WCSceneList::WCSceneList(WCArchive* archive)
{
	WCPak palpak("DATA\\OPTIONS\\OPTPALS.PAK", archive);

	FileEntryReader reader = archive->openFile("DATA\\OPTIONS\\OPTIONS.IFF");

	reader.PushChunk("FORM");
	reader.ReadTag("OPTS");
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
		reader.ReadTag("SCEN");

		WCScene scene;
		while (!reader.IsEndOfChunk())
		{
			reader.PushChunk("FORM");
			std::string tag = reader.ReadTag();
			if (tag == "REGN")
			{
				scene.regions.push_back(ReadRegion(reader));
			}
			else if (tag == "BACK")
			{
				scene.background = ReadBackground(reader);
			}
			else if (tag == "FORE")
			{
				scene.foreground = ReadForeground(reader);
			}
			else if (tag == "SHIP")
			{
				auto palette = std::make_unique<WCPalette>(palpak.openFile(scene.background.palette));
				scene.ship = ReadShip(reader, palette.get());
			}
			else if (tag == "LOOK")
			{
				scene.look = ReadLook(reader);
			}

			reader.PopChunk();
		}

		reader.PopChunk();
		scenes.push_back(std::move(scene));
	}
}

WCRegion WCSceneList::ReadRegion(FileEntryReader& reader)
{
	WCRegion region;
	while (!reader.IsEndOfChunk())
	{
		std::string tag = reader.PushChunk();
		if (tag == "INFO")
		{
			if (reader.GetChunkSize() != 1)
				throw std::runtime_error("Unexpected REGN.INFO size");
			region.target = reader.ReadUint8();
		}
		else if (tag == "LABL")
		{
			std::string text;
			for (int i = 0, count = reader.GetChunkSize(); i < count; i++)
			{
				char c = reader.ReadUint8();
				if (c == 0)
					break;
				text.push_back(c);
			}
			region.label = std::move(text);
		}
		else if (tag == "CORD")
		{
			region.x1 = reader.ReadUint16();
			region.y1 = reader.ReadUint16();
			region.x2 = reader.ReadUint16();
			region.y2 = reader.ReadUint16();
		}
		else
		{
			throw std::runtime_error("Unexpected tag");
		}
		reader.PopChunk();
	}
	return region;
}

WCSceneBackground WCSceneList::ReadBackground(FileEntryReader& reader)
{
	WCSceneBackground bg;
	while (!reader.IsEndOfChunk())
	{
		std::string tag = reader.PushChunk();
		if (tag == "PALT")
		{
			if (reader.GetChunkSize() != 2)
				throw std::runtime_error("Unexpected BACK.PALT size");
			bg.palette = reader.ReadUint16();
		}
		else if (tag == "SHAP")
		{
			WCSceneShape shape;
			shape.type = reader.ReadUint16();
			shape.optpakIndex = reader.ReadUint16();
			for (int i = 0, count = reader.GetChunkSize() - 4; i < count; i++)
				shape.unknown.push_back(reader.ReadUint8());
			bg.shapes.push_back(std::move(shape));
		}
		else
		{
			throw std::runtime_error("Unexpected tag");
		}
		reader.PopChunk();
	}
	return bg;
}

WCSceneForeground WCSceneList::ReadForeground(FileEntryReader& reader)
{
	WCSceneForeground fg;
	while (!reader.IsEndOfChunk())
	{
		std::string tag = reader.PushChunk();
		if (tag == "PALT")
		{
			if (reader.GetChunkSize() != 2)
				throw std::runtime_error("Unexpected BACK.PALT size");
			fg.palette = reader.ReadUint16();
		}
		else if (tag == "FORM")
		{
			reader.ReadTag("SPRT");

			WCSceneSprite sprite;
			while (!reader.IsEndOfChunk())
			{
				tag = reader.PushChunk();
				if (tag == "SHAP")
				{
					for (int i = 0, count = reader.GetChunkSize(); i < count; i++)
						sprite.shape.push_back(reader.ReadUint8());
				}
				else if (tag == "LABL")
				{
					std::string text;
					for (int i = 0, count = reader.GetChunkSize(); i < count; i++)
					{
						char c = reader.ReadUint8();
						if (c == 0)
							break;
						text.push_back(c);
					}
					sprite.label = std::move(text);
				}
				else if (tag == "RECT")
				{
					sprite.x = reader.ReadUint16();
					sprite.y = reader.ReadUint16();
					sprite.width = reader.ReadUint16();
					sprite.height = reader.ReadUint16();
				}
				else if (tag == "INFO")
				{
					for (int i = 0, count = reader.GetChunkSize(); i < count; i++)
						sprite.info.push_back(reader.ReadUint8());
				}
				else if (tag == "SEQU")
				{
					for (int i = 0, count = reader.GetChunkSize(); i < count; i++)
						sprite.sequence.push_back(reader.ReadUint8());
				}
				else if (tag == "CLCK")
				{
					if (reader.GetChunkSize() != 0)
						throw std::runtime_error("Unexpected SPRT.CLCK size");

					sprite.clck = true;
				}
				else if (tag == "REGN")
				{
					if (reader.GetChunkSize() != 16)
						throw std::runtime_error("Unexpected SPRT.REGN size");
					sprite.region.x1 = reader.ReadUint16();
					sprite.region.y1 = reader.ReadUint16();
					sprite.region.x2 = reader.ReadUint16();
					sprite.region.y2 = reader.ReadUint16();
				}
				else
				{
					throw std::runtime_error("Unexpected tag");
				}
				reader.PopChunk();
			}
			fg.sprites.push_back(std::move(sprite));
		}
		else
		{
			throw std::runtime_error("Unexpected tag");
		}
		reader.PopChunk();
	}
	return fg;
}

WCSceneShip WCSceneList::ReadShip(FileEntryReader& reader, WCPalette* palette)
{
	WCSceneShip ship;
	while (!reader.IsEndOfChunk())
	{
		std::string tag = reader.PushChunk();
		if (tag == "INFO")
		{
			for (int i = 0, count = reader.GetChunkSize(); i < count; i++)
				ship.info.push_back(reader.ReadUint8());
		}
		else if (tag == "SHAP")
		{
			ship.shape = std::make_unique<WCImage>(reader, palette);
		}
		else
		{
			throw std::runtime_error("Unexpected tag");
		}
		reader.PopChunk();
	}
	return ship;
}

WCSceneLook WCSceneList::ReadLook(FileEntryReader& reader)
{
	WCSceneLook look;
	while (!reader.IsEndOfChunk())
	{
		std::string tag = reader.PushChunk();
		if (tag == "INFO")
		{
			for (int i = 0, count = reader.GetChunkSize(); i < count; i++)
				look.info.push_back(reader.ReadUint8());
		}
		else
		{
			throw std::runtime_error("Unexpected tag");
		}
		reader.PopChunk();
	}
	return look;
}
