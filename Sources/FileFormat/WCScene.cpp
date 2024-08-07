
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
	reader.PopChunk(false);

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
				scene.regions.push_back(ReadRegion(reader, false));
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
				scene.ship = ReadShip(reader);
			}
			else if (tag == "LOOK")
			{
				scene.look = ReadLook(reader);
			}
			else if (tag == "RECT")
			{
				scene.regions.push_back(ReadRegion(reader, true));
			}
			else
			{
				throw std::runtime_error("Unexpected tag");
			}

			reader.PopChunk();
		}

		reader.PopChunk();
		scenes.push_back(std::move(scene));
	}
}

WCRegion WCSceneList::ReadRegion(FileEntryReader& reader, bool isRect)
{
	WCRegion region;
	while (!reader.IsEndOfChunk())
	{
		std::string tag = reader.PushChunk();
		if (tag == "INFO")
		{
			if (reader.GetChunkSize() != 1)
				throw std::runtime_error("Unexpected REGN.INFO size");
			region.target = (WCTarget)reader.ReadUint8();
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
			if (isRect)
			{
				if (reader.GetChunkSize() != 8)
					throw std::runtime_error("Unexpected RECT.CORD size");
				int left = reader.ReadInt16();
				int top = reader.ReadInt16();
				int right = reader.ReadInt16();
				int bottom = reader.ReadInt16();
				region.coords.push_back({ left, top });
				region.coords.push_back({ right, top });
				region.coords.push_back({ right, bottom });
				region.coords.push_back({ left, bottom });
			}
			else
			{
				while (!reader.IsEndOfChunk())
				{
					WCRegionPoint p;
					p.x = reader.ReadInt16();
					p.y = reader.ReadInt16();
					region.coords.push_back(p);
				}
			}
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
			shape.unknown16 = reader.ReadUint16();
			shape.optpakIndex = reader.ReadUint16();
			shape.offsetX = reader.ReadUint16();
			shape.offsetY = reader.ReadUint16();
			for (int i = 0, count = reader.GetChunkSize() - 8; i < count; i++)
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
					sprite.target = (WCSpriteTarget)reader.ReadUint8();
					sprite.optpakIndex = reader.ReadUint8();
					for (int i = 0, count = reader.GetChunkSize() - 2; i < count; i++)
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
					sprite.x1 = reader.ReadUint16();
					sprite.y1 = reader.ReadUint16();
					sprite.x2 = reader.ReadUint16();
					sprite.y2 = reader.ReadUint16();
				}
				else if (tag == "INFO")
				{
					for (int i = 0, count = reader.GetChunkSize() / 2; i < count; i++)
						sprite.info.push_back(reader.ReadUint16());
				}
				else if (tag == "SEQU")
				{
					for (int i = 0, count = reader.GetChunkSize() / 2; i < count; i++)
						sprite.sequence.push_back(reader.ReadUint16());
				}
				else if (tag == "CLCK")
				{
					if (reader.GetChunkSize() != 0)
						throw std::runtime_error("Unexpected SPRT.CLCK size");

					sprite.clck = true;
				}
				else if (tag == "REGN")
				{
					while (!reader.IsEndOfChunk())
					{
						WCRegionPoint p;
						p.x = reader.ReadInt16();
						p.y = reader.ReadInt16();
						sprite.region.push_back(p);
					}
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

WCSceneShip WCSceneList::ReadShip(FileEntryReader& reader)
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
			ship.shape = std::make_unique<WCImage>(reader);
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
