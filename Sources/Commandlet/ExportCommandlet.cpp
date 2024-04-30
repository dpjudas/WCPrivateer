
#include "Precomp.h"
#include "ExportCommandlet.h"
#include "Tool/ToolApp.h"
#include "FileFormat/WCArchive.h"
#include "FileFormat/WCImage.h"
#include "FileFormat/WCPak.h"
#include "FileFormat/WCPalette.h"
#include "FileFormat/FileEntryReader.h"
#include <miniz/miniz.h>

ExportCommandlet::ExportCommandlet()
{
	SetLongFormName("export");
	SetShortDescription("Extract data from the packages");
}

void ExportCommandlet::OnCommand(ToolApp* console, const std::string& args)
{
	mArchiveFilename = "PRIV.TRE";

	if (args == "archive")
	{
		ExportArchive(console);
	}
	else if (args == "iffimages")
	{
		ExportIffImages(console);
	}
	else if (args == "pakimages")
	{
		ExportPakImages(console);
	}
	else if (args == "shpimages")
	{
		ExportShpImages(console);
	}
	else if (args == "vpk")
	{
		ExportVpk(console);
	}
	else if (args == "vpf")
	{
		ExportVpf(console);
	}
	else if (args == "iff")
	{
		WCArchive archive(mArchiveFilename);
		FileEntryReader reader = archive.openFile("DATA\\OPTIONS\\OPTIONS.IFF");
		if (reader.ReadTag() == "FORM")
		{
			PrintForm(console, reader, 0);
		}
	}
	else
	{
		console->WriteOutput("Unknown command " + args + NewLine());
	}
}

void ExportCommandlet::PrintForm(ToolApp* console, FileEntryReader& reader, int depth)
{
	uint32_t size = reader.ReadUint32BE();
	uint32_t end = reader.Tell() + size + (size & 1);
	std::string formName = reader.ReadTag();

	console->WriteOutput("", depth * 4);
	console->WriteOutput("FORM " + formName, 4);
	console->WriteOutput(NewLine());
	depth++;

	if (formName == "OPTS")
	{
		std::string tag = reader.ReadTag();

		console->WriteOutput("", depth * 4);
		console->WriteOutput(tag, 5);
		console->WriteOutput(NewLine());

		if (tag != "TABL")
			throw std::runtime_error("Expected TABL at beginning of OPTS form");

		uint32_t itemsize = reader.ReadUint32BE();
		uint32_t itemend = reader.Tell() + itemsize + (itemsize & 1);

		std::vector<uint32_t> table;
		int count = itemsize / 4;
		for (int i = 0; i < count; i++)
		{
			table.push_back(reader.ReadUint32());
		}

		for (int y = 0; y < (count + 15) / 16; y++)
		{
			console->WriteOutput("", (depth + 1) * 4);
			int start = y * 16;
			int end = std::min(start + 16, count);
			for (int i = start; i < end; i++)
			{
				console->WriteOutput(table[i], 8);
			}
			console->WriteOutput(NewLine());
		}
	}

	while (reader.Tell() < end)
	{
		if (formName == "OPTS")
		{
			// Size of the block following
			uint32_t size = reader.ReadUint32();
			console->WriteOutput("", depth * 4);
			console->WriteOutput(size, 8);
			console->WriteOutput(NewLine());
		}

		std::string tag = reader.ReadTag();
		if (tag == "FORM")
		{
			PrintForm(console, reader, depth);
		}
		else
		{
			uint32_t itemsize = reader.ReadUint32BE();
			uint32_t itemend = reader.Tell() + itemsize + (itemsize & 1);

			console->WriteOutput("", depth * 4);
			console->WriteOutput(tag, 5);

			if (formName == "BACK" && tag == "SHAP")
			{
				std::vector<uint16_t> table;
				int count = itemsize / 2;
				for (int i = 0; i < count; i++)
				{
					table.push_back(reader.ReadUint16());
				}

				console->WriteOutput(NewLine());
				for (int y = 0; y < (count + 15) / 16; y++)
				{
					console->WriteOutput("", (depth + 1) * 4);
					int start = y * 16;
					int end = std::min(start + 16, count);
					for (int i = start; i < end; i++)
					{
						console->WriteOutput(table[i], 8);
					}
					console->WriteOutput(NewLine());
				}
			}
			else if (tag == "LABL")
			{
				std::string text(itemsize, '\0');
				memcpy(text.data(), reader.CurrentPosData(), itemsize);
				for (size_t i = 0; i < text.size(); i++)
				{
					if (text[i] == 0)
					{
						text.resize(i);
						break;
					}
				}

				console->WriteOutput(text);
				console->WriteOutput(NewLine());
			}
			else if (tag == "INFO")
			{
				console->WriteOutput(reader.ReadUint16(), 8);
				console->WriteOutput(NewLine());
			}
			else if (tag == "PALT")
			{
				console->WriteOutput(reader.ReadUint16(), 8);
				console->WriteOutput(NewLine());
			}
			else if (tag == "CORD")
			{
				int x1 = reader.ReadUint16();
				int y1 = reader.ReadUint16();
				int x2 = reader.ReadUint16();
				int y2 = reader.ReadUint16();
				console->WriteOutput(x1, 4);
				console->WriteOutput(y1, 4);
				console->WriteOutput(x2, 4);
				console->WriteOutput(y2, 4);
				console->WriteOutput(NewLine());
			}
			else
			{
				console->WriteOutput(itemsize, 8);
				console->WriteOutput(NewLine());
			}

			reader.Seek(itemend);
		}
	}

	reader.Seek(end);
}

void ExportCommandlet::OnPrintHelp(ToolApp* console)
{
	console->WriteOutput("Syntax: export archive" + NewLine());
	console->WriteOutput("        export iffimages" + NewLine());
	console->WriteOutput("        export pakimages" + NewLine());
	console->WriteOutput("        export shpimages" + NewLine());
	console->WriteOutput("        export vpk" + NewLine());
	console->WriteOutput("        export vpf" + NewLine());
	console->WriteOutput("        export iff" + NewLine());
}

void ExportCommandlet::WriteFile(ToolApp* console, std::string filename, const void* data, size_t size)
{
	for (char& c : filename)
	{
		if (c == '\\' || c == '/')
			c = '.';
	}

	FILE* o = fopen(("export/" + filename).c_str(), "wb");
	if (o)
	{
		fwrite(data, size, 1, o);
		fclose(o);
	}
	else
	{
		console->WriteOutput("Could not write " + ColorEscape(96) + filename + ResetEscape() + NewLine());
	}
}

void ExportCommandlet::ExportArchive(ToolApp* console)
{
	console->WriteOutput("Exporting files from " + ColorEscape(96) + mArchiveFilename + ResetEscape() + NewLine());
	WCArchive archive(mArchiveFilename);

	for (int i = 0; i < archive.getFileCount(); i++)
	{
		std::string filename = archive.getFilename(i);
		std::string ext = filename.substr(filename.size() - 4);

		{
			FileEntryReader reader = archive.openFile(i);
			WriteFile(console, filename, reader.CurrentPosData(), reader.Size());
		}

		if (ext == ".PAK" && filename != "SPEECH.PAK")
		{
			WCPak pak(filename, &archive);
			int count = (int)pak.files.size();
			for (int i = 0; i < count; i++)
			{
				std::string filename2 = filename.substr(0, filename.size() - 4) + "-" + std::to_string(i) + ".BIN";
				WriteFile(console, filename2, pak.files[i].data(), pak.files[i].size());
			}
		}
	}
}

void ExportCommandlet::ExportIffImages(ToolApp* console)
{
	console->WriteOutput("Exporting IFF images from " + ColorEscape(96) + mArchiveFilename + ResetEscape() + NewLine());
	WCArchive archive(mArchiveFilename);

	auto spacepal = std::make_unique<WCPalette>("DATA\\PALETTE\\SPACE.PAL", &archive);

	for (int i = 0; i < archive.getFileCount(); i++)
	{
		std::string filename = archive.getFilename(i);
		std::string ext = filename.substr(filename.size() - 4);

		if (ext != ".IFF")
			continue;

		console->WriteOutput(NewLine());
		console->WriteOutput(filename, 40);

		FileEntryReader reader = archive.openFile(i);

		if (reader.ReadTag() != "FORM")
		{
			console->WriteOutput("Not IFF form", 20);
			continue;
		}
		reader.ReadUint32BE(); // chunk size

		std::string tag = reader.ReadTag();
		console->WriteOutput(tag, 6);
		if (tag == "APPR")
		{
			tag = reader.ReadTag();
			console->WriteOutput(tag, 6);
			if (tag != "FORM")
				continue;
			reader.ReadUint32BE(); // chunk size

			tag = reader.ReadTag();
			console->WriteOutput(tag, 6);
			if (tag == "BMAP")
			{
				tag = reader.ReadTag();
				console->WriteOutput(tag, 6);
				if (tag != "INFO")
					continue;
				if (reader.ReadUint32BE() != 6) // chunk size, expect 6
					continue;

				uint8_t unknown0 = reader.ReadUint8();
				uint8_t unknown1 = reader.ReadUint8();
				uint8_t unknown2 = reader.ReadUint8();
				uint8_t unknown3 = reader.ReadUint8();
				uint8_t unknown4 = reader.ReadUint8();
				uint8_t unknown5 = reader.ReadUint8();
			}
			else if (tag == "BM3D")
			{
				tag = reader.ReadTag();
				console->WriteOutput(tag, 6);
				if (tag != "INFO")
					continue;
				if (reader.ReadUint32BE() != 12) // chunk size, expect 12
					continue;

				uint8_t unknown0 = reader.ReadUint8();
				uint8_t unknown1 = reader.ReadUint8();
				uint8_t unknown2 = reader.ReadUint8();
				uint8_t unknown3 = reader.ReadUint8();
				uint8_t unknown4 = reader.ReadUint8();
				uint8_t unknown5 = reader.ReadUint8();
				uint8_t unknown6 = reader.ReadUint8();
				uint8_t unknown7 = reader.ReadUint8();
				uint8_t unknown8 = reader.ReadUint8();
				uint8_t unknown9 = reader.ReadUint8();
				uint8_t unknown10 = reader.ReadUint8();
				uint8_t unknown11 = reader.ReadUint8();
			}
			else
			{
				continue;
			}
		}
		else if (tag == "COCK")
		{
			tag = reader.ReadTag();
			console->WriteOutput(tag, 6);
			if (tag != "FORM")
				continue;
			reader.ReadUint32BE(); // chunk size

			tag = reader.ReadTag();
			console->WriteOutput(tag, 6);
			if (tag != "FRNT") // Other sides appear after first one?
				continue;
		}
		else if (tag == "MISC")
		{
			tag = reader.ReadTag();
			console->WriteOutput(tag, 6);
			if (tag != "TABL")
				continue;
			reader.ReadUint32BE(); // chunk size, is 4 and its an invalid size!

			uint8_t unknown0 = reader.ReadUint8();
			uint8_t unknown1 = reader.ReadUint8();
			uint8_t unknown2 = reader.ReadUint8();
			uint8_t unknown3 = reader.ReadUint8();
			uint8_t unknown4 = reader.ReadUint8();
			uint8_t unknown5 = reader.ReadUint8();
			uint8_t unknown6 = reader.ReadUint8();
			uint8_t unknown7 = reader.ReadUint8();

			tag = reader.ReadTag();
			console->WriteOutput(tag, 6);
			if (tag != "FORM")
				continue;
			reader.ReadUint32BE(); // chunk size

			tag = reader.ReadTag();
			console->WriteOutput(tag, 6);
			if (tag != "EXPL")
				continue;

			tag = reader.ReadTag();
			console->WriteOutput(tag, 8);
			if (tag != "INFO")
				continue;
			if (reader.ReadUint32BE() != 8) // chunk size, expect 8
				continue;

			uint8_t unknown8 = reader.ReadUint8();
			uint8_t unknown9 = reader.ReadUint8();
			uint8_t unknown10 = reader.ReadUint8();
			uint8_t unknown11 = reader.ReadUint8();
			uint8_t unknown12 = reader.ReadUint8();
			uint8_t unknown13 = reader.ReadUint8();
			uint8_t unknown14 = reader.ReadUint8();
			uint8_t unknown15 = reader.ReadUint8();
		}
		else if (tag == "ATTD") // DATA\AIDS\ATTITUDE.IFF
		{
			while (reader.Tell() != reader.Size())
			{
				tag = reader.ReadTag();
				console->WriteOutput(tag, 6);
				uint32_t chunksize = reader.ReadUint32BE();
				if (tag == "AROW" || tag == "DISP") chunksize++; // wtf?
				reader.Seek(reader.Tell() + chunksize);
			}
			continue;
		}
		else if (tag == "BUSR") // DATA\AIDS\BASE_AGR.IFF
		{
			tag = reader.ReadTag();
			console->WriteOutput(tag, 6);
			if (tag != "FNAM")
				continue;
			uint32_t chunksize = reader.ReadUint32BE();
			std::string basename;
			for (uint32_t i = 0; i < chunksize; i++)
				basename.push_back(reader.ReadUint8());
			basename.resize(std::min(basename.find_first_of('\0', 0), basename.size()));
			console->WriteOutput(basename, 20);

			tag = reader.ReadTag();
			console->WriteOutput(tag, 8);
			if (tag != "INFO")
				continue;
			if (reader.ReadUint32BE() != 7) // chunk size, expect 7
				continue;

			uint8_t unknown0 = reader.ReadUint8();
			uint8_t unknown1 = reader.ReadUint8();
			uint8_t unknown2 = reader.ReadUint8();
			uint8_t unknown3 = reader.ReadUint8();
			uint8_t unknown4 = reader.ReadUint8();
			uint8_t unknown5 = reader.ReadUint8();
			uint8_t unknown6 = reader.ReadUint8();

			uint8_t unknown7 = reader.ReadUint8(); // note: not part of the chunk

			tag = reader.ReadTag();
			console->WriteOutput(tag, 6);
			if (tag != "FORM")
				continue;
			reader.ReadUint32BE(); // chunk size

			tag = reader.ReadTag();
			console->WriteOutput(tag, 6);
			if (tag != "RECV")
				continue;

			// misc strings follows. "Entering automatic landing zone", "Greetings", "Salutations", etc.

			continue;
		}
		else if (tag == "AIDS") // DATA\AIDS\BOU_3C.IFF
		{
			// more string tables like "Welcome to the happy hunting grounds" etc
			continue;
		}
		else if (tag == "FACE") // DATA\AIDS\MFDFACES.IFF
		{
			tag = reader.ReadTag();
			console->WriteOutput(tag, 6);
			if (tag != "TABL")
				continue;
			uint32_t chunksize = reader.ReadUint32BE();
			reader.Seek(reader.Tell() + chunksize);

			size_t baseindex = 0;
			while (reader.Tell() < reader.Size())
			{
				uint32_t facesize = reader.ReadUint32();
				size_t faceoffset = reader.Tell();
				if (faceoffset + facesize > reader.Size())
					break;

				tag = reader.ReadTag();
				console->WriteOutput(tag, 6);
				if (tag != "INFO")
					break;
				if (reader.ReadUint32BE() != 2) // chunk size, expect 2
					break;

				uint8_t unknown0 = reader.ReadUint8();
				uint8_t unknown1 = reader.ReadUint8();

				tag = reader.ReadTag();
				console->WriteOutput(tag, 6);
				uint32_t chunksize = reader.ReadUint32BE(); // chunk size
				if (tag != "SHAP")
					break;

				WCImage image(reader, spacepal.get());
				SaveImage(console, filename, image, baseindex);
				baseindex += image.frames.size();

				reader.Seek(faceoffset + facesize);
			}
			continue;
		}
		else if (tag == "MOVI") // DATA\MIDGAMES\JUMP.IFF
		{
			tag = reader.ReadTag();
			console->WriteOutput(tag, 6);
			if (tag != "CLRC")
				continue;
			if (reader.ReadUint32BE() != 2) // chunk size, expect 2
				continue;
			uint8_t unknown0 = reader.ReadUint8();
			uint8_t unknown1 = reader.ReadUint8();

			tag = reader.ReadTag();
			console->WriteOutput(tag, 6);
			if (tag != "SPED")
				continue;
			if (reader.ReadUint32BE() != 2) // chunk size, expect 2
				continue;
			uint8_t unknown2 = reader.ReadUint8();
			uint8_t unknown3 = reader.ReadUint8();

			tag = reader.ReadTag();
			console->WriteOutput(tag, 6);
			if (tag != "FILE")
				continue;
			uint32_t chunksize = reader.ReadUint32BE();

			uint8_t unknown4 = reader.ReadUint8();
			uint8_t unknown5 = reader.ReadUint8();

			std::string filename;
			for (uint32_t i = 0; i < chunksize - 2; i++)
				filename.push_back(reader.ReadUint8());
			filename.resize(std::min(filename.find_first_of('\0', 0), filename.size()));
			console->WriteOutput(filename, 60);

			tag = reader.ReadTag();
			console->WriteOutput(tag, 8);
			if (tag != "FILD")
				continue;
			chunksize = reader.ReadUint32BE();

			// SPRI
			// FORM ACTSBFOR

			continue;
		}
		else if (tag == "MIDG") // DATA\MIDGAMES\MIDGAMES.IFF
		{
			// TABL and FNAM entries listing other .IFF and .PAK files
			continue;
		}
		else if (tag == "REAL") // DATA\APPEARNC\SKELETON.IFF
		{
			continue;
		}
		else if (tag == "ITTS") // DATA\COCKPITS\ITTS.IFF
		{
			continue;
		}
		else if (tag == "PLAQ") // DATA\COCKPITS\PLAQUES.IFF
		{
			continue;
		}
		else if (tag == "SOFT") // DATA\COCKPITS\SOFTWARE.IFF
		{
			continue;
		}
		else if (tag == "RUMR") // DATA\CONV\AGRIRUMR.IFF
		{
			continue;
		}
		else if (tag == "INFO") // DATA\CONV\MERCINFO.IFF
		{
			continue;
		}
		else
		{
			continue;
		}

		tag = reader.ReadTag();
		console->WriteOutput(tag, 6);

		uint32_t chunksize = reader.ReadUint32BE(); // chunk size

		if (tag == "SHAP")
		{
			WCImage image(reader, spacepal.get());
			SaveImage(console, filename, image, 0);
		}
		else if (tag == "VSHP")
		{
			size_t chunkoffset = reader.Tell();

			uint32_t sectionoffset = 0;
			size_t baseindex = 0;
			do
			{
				reader.Seek(chunkoffset + sectionoffset);
				uint32_t sectionsize = reader.ReadUint32();

				reader.Seek(chunkoffset + sectionoffset);
				WCImage image(reader, spacepal.get());

				SaveImage(console, filename, image, baseindex);
				baseindex += image.frames.size();

				sectionoffset += sectionsize;
			} while (sectionoffset + 4 <= chunksize);
		}
	}

	console->WriteOutput(NewLine());
}

void ExportCommandlet::ExportPakImages(ToolApp* console)
{
	console->WriteOutput("Exporting PAK images from " + ColorEscape(96) + mArchiveFilename + ResetEscape() + NewLine());
	WCArchive archive(mArchiveFilename);

	for (int i = 0; i < archive.getFileCount(); i++)
	{
		std::string filename = archive.getFilename(i);
		std::string ext = filename.substr(filename.size() - 4);

		if (ext != ".PAK" || filename == "SPEECH.PAK" || filename == "DATA\\SOUND\\SOUNDFX.PAK")
			continue;

		console->WriteOutput(NewLine());
		console->WriteOutput(filename, 40);

		std::unique_ptr<WCPalette> palette = std::make_unique<WCPalette>("DATA\\PALETTE\\PCMAIN.PAL", &archive);

		bool isOptshps = filename.find("OPTSHPS.PAK") != std::string::npos;

		WCPak pak(filename, &archive);

		for (int i = 0; i < (int)pak.files.size(); i++)
		{
			if (isOptshps)
			{
				WCPak palpak("DATA\\OPTIONS\\OPTPALS.PAK", &archive);
				palette = std::make_unique<WCPalette>(palpak.openFile(i % palpak.files.size()));
			}

			FileEntryReader reader = pak.openFile(i);
			if (reader.Size() == 3 * 256 + 4 && reader.ReadUint32BE() == 1) // palette
			{
				reader.Seek(0);
				palette = std::make_unique<WCPalette>(reader);
			}
			else if (palette && reader.Size() > 4)
			{
				reader.Seek(0);
				uint32_t size = reader.ReadUint32();
				reader.Seek(0);
				if (reader.Size() == size) // probably image
				{
					try
					{
						WCImage image(reader, palette.get());
						SaveImage(console, filename.substr(0, filename.size() - 4) + "-" + std::to_string(i) + ".png", image, 0);
					}
					catch (const std::exception& ex)
					{
						console->WriteOutput(ex.what(), 100);
						console->WriteOutput(NewLine());
					}
				}
			}
		}
	}

	console->WriteOutput(NewLine());
}

void ExportCommandlet::ExportShpImages(ToolApp* console)
{
	console->WriteOutput("Exporting SHP images from " + ColorEscape(96) + mArchiveFilename + ResetEscape() + NewLine());
	WCArchive archive(mArchiveFilename);

	auto palette = std::make_unique<WCPalette>("DATA\\PALETTE\\PREFMAIN.PAL", &archive);

	for (int i = 0; i < archive.getFileCount(); i++)
	{
		std::string filename = archive.getFilename(i);
		std::string ext = filename.substr(filename.size() - 4);

		if (ext != ".SHP" || filename.find("FONTS\\") != std::string::npos)
			continue;

		console->WriteOutput(NewLine());
		console->WriteOutput(filename, 40);

		auto reader = archive.openFile(i);
		try
		{
			WCImage image(reader, palette.get());
			SaveImage(console, filename, image, 0);
		}
		catch (const std::exception& ex)
		{
			console->WriteOutput(ex.what(), 100);
			console->WriteOutput(NewLine());
		}
	}

	console->WriteOutput(NewLine());
}

void ExportCommandlet::ExportVpk(ToolApp* console)
{
	console->WriteOutput("Exporting VPK audio from " + ColorEscape(96) + mArchiveFilename + ResetEscape() + NewLine());
	WCArchive archive(mArchiveFilename);

	// Note DATA/CONV/AGRIRUMR.IFF points to the text conversation (PFC file) and the audio conversation (VPK) AGRRUM1.PFC and AGRRUM1.VPK
	// The animation is not in those three files

	for (int i = 0; i < archive.getFileCount(); i++)
	{
		std::string filename = archive.getFilename(i);
		std::string ext = filename.substr(filename.size() - 4);

		if (ext != ".VPK")
			continue;

		console->WriteOutput(NewLine());
		console->WriteOutput(filename, 40);

		/*
		A header starting with the file size then read 4 bytes at a time, which points to a bunch of entries:

		first begins at offset 0x1c,
		second at 0x602a,
		third at 0xdbd4, etc (in an example file, the offsets vary in each file.)

		Each of these entries is LZW compressed.
		Start reading 9 bits at a time, expect symbol 257 as a stop symbol (although only some games honour that),
		and dictionary entries start on symbol 258. As dictionary expands towards entry 512, read 10 bytes at a
		time, and so on.

		You read at most 12 bytes at time, the dictionary doesn't grow beyond that (4096 entries... with the first
		256 entries being the ASCII table itself) and symbol 256 resets the dictionary.

		Unpack these and you get a Creative VOC file, which you'll be able to play in VLC, or a similar player.
		The format is PCM, which means it's not compressed by any predictive audio compression algorithm. It's just
		plain 8-bit (unsigned), 11025Hz, mono audio.
		*/
	}

	console->WriteOutput(NewLine());
}

void ExportCommandlet::ExportVpf(ToolApp* console)
{
	// Unknown file in CONV folder
}

void ExportCommandlet::SaveImage(ToolApp* console, std::string filename, const WCImage& image, size_t baseindex)
{
	for (size_t imageindex = 0; imageindex < image.frames.size(); imageindex++)
	{
		size_t pngSize = 0;
		void* pngData = tdefl_write_image_to_png_file_in_memory(image.frames[imageindex].pixels.data(), image.frames[imageindex].width, image.frames[imageindex].height, 4, &pngSize);
		if (pngData)
		{
			std::string pngfilename = filename;
			pngfilename = pngfilename.substr(0, pngfilename.size() - 4);
			pngfilename += "-";
			pngfilename += std::to_string(baseindex + imageindex);
			pngfilename += ".png";
			WriteFile(console, pngfilename, pngData, pngSize);
			mz_free(pngData);
		}
	}
}

/*

By the way, this is not very different from the way Wing Commander 2 sequences are encoded,
with WC2's base RLE algorithm being used to encode image segments corresponding to frame differences.
WC2 animation frames, by the way, are grouped together into ~64k-long blocks which are preferentially
loaded into EMS memory, and optionally compressed with a form of LZW. As a curiosity, most WC2 assets
are LZW-compressed, but can be decompressed at install-time optionally (the "Save Time or Save Space"
option available on the floppy install).

CUBICLE.pak
	unknown

jump.pak
	1 pal + shp


mid1.pak
	13 pal + shp stuff

mid1txt.pak also contains bunch of strings

midtext.pak
	"2669, GEMINI SECTOR, TROY SYSTEM..."
	"THE TERRAN FRONTIER..."
	"BETWEEN THE KILRATHI EMPIRE..."
	"...AND THE UNKNOWN."
	"And what is it, hmmm?"
	"Is it a smuggler far off the normal space lanes?"
	"Maybe a bad smuggler with some nice contraband?"
	"Sorry, boys, I'm just a tourist with a fragged nav console."
	"Could you tell me where I am?"
	"Deep in trouble, little tourist."
	"Prepare to be boarded."
	"Not a chance, pal."
	"Prepare to go to Hell."
	"What is it, that flies so good?"
	"Is it insane?"
	"No, it's just got a load of cargo in the hold and a load of bills to pay at home."
	"And I the same!"
	"It shouldn't kill me just for attacking it!"
	"I don't mind that you tried to kill me."
	"But protecting myself against your type gets expensive..."
	"...and I'm on a budget."
	"Hold your calls, I think we have a winner!"
	"Rest in pieces, you bastard."
	"MEANWHILE... SECTOR  DELTA  PRIME..."

opening.pak
	"mid1a"
	"mid1b"
	"mid1c1"
	"mid1c2"
	"mid1c3"
	"mid1c4"
	"mid1d"
	"mid1e1"
	"mid1e2"
	"mid1e3"
	"mid1e4"
	"mid1f"

takoffs.pak
	0 pal, 1-5 shp?

vctlist.pak
	"victory1"
	"victory3"
	"victory4"
	"victory5"

victory.pak
	0 pal, 1-11 shp?
	12 pal, 13-17 shp?
	18 pal, 19-23 shp?
	24 pal, 25-26 shp?

vcttxt.pak =
	"Rest in pieces, you bastard."
	"Meanwhile...sector Delta Prime.
	Aboard the Steltek Scout, an officer notifies his
	captain of the drone's destruction."
	"Vessel recovered. Drone no longer."
	"To be good."
	"However, weapons still in alien control. Wise?"
	"No. Choice none. Damage acceptable."
	"How much can single Privateer do?"


*/
