
#include "Precomp.h"
#include "WCMusic.h"
#include "WCArchive.h"
#include "WCPak.h"
#include "FileEntryReader.h"

WCMusic::WCMusic(const std::string& filename, WCArchive* archive)
{
	FileEntryReader reader = archive->openFile(filename);

	if (reader.ReadTag() == "FORM")
	{
		reader.Seek(0);
		std::vector<uint8_t> data(reader.Size());
		reader.Read(data.data(), data.size());
		songs.push_back(std::move(data));
	}
	else
	{
		// What is all this pak in pak in pak in pak stuff about?

		reader.Seek(0);
		WCPak pak1(reader);
		for (auto& file : pak1.files)
		{
			FileEntryReader reader2(file);
			if (reader2.ReadTag() == "FORM")
			{
				reader2.Seek(0);
				std::vector<uint8_t> data(reader2.Size());
				reader2.Read(data.data(), data.size());
				songs.push_back(std::move(data));
			}
			else
			{
				reader2.Seek(0);
				WCPak pak2(reader2);
				for (auto& file2 : pak2.files)
				{
					FileEntryReader reader3(file2);
					if (reader3.ReadTag() == "FORM")
					{
						reader3.Seek(0);
						std::vector<uint8_t> data(reader3.Size());
						reader3.Read(data.data(), data.size());
						songs.push_back(std::move(data));
					}
					else
					{
						reader3.Seek(0);
						WCPak pak3(reader3);
						for (auto& file3 : pak3.files)
						{
							FileEntryReader reader4(file3);
							if (reader4.ReadTag() == "FORM")
							{
								reader4.Seek(0);
								std::vector<uint8_t> data(reader3.Size());
								reader4.Read(data.data(), data.size());
								songs.push_back(std::move(data));
							}
							else
							{
								throw std::runtime_error("Invalid music header");
							}
						}
					}
				}
			}
		}
	}
}
