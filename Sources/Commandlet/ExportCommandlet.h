#pragma once

#include "Commandlet/Commandlet.h"

class WCImage;
class FileEntryReader;

class ExportCommandlet : public Commandlet
{
public:
	ExportCommandlet();

	void OnCommand(ToolApp* console, const std::string& args) override;
	void OnPrintHelp(ToolApp* console) override;

private:
	void ExportArchive(ToolApp* console);
	void ExportPak(ToolApp* console);
	void ExportMovie(ToolApp* console);
	void ExportIffImages(ToolApp* console);
	void ExportPakImages(ToolApp* console);
	void ExportShpImages(ToolApp* console);
	void ExportVpk(ToolApp* console);
	void ExportVpf(ToolApp* console);
	void ExportScene(ToolApp* console);
	void ExportMusic(ToolApp* console);
	void ExportConversation(ToolApp* console);
	void SaveImage(ToolApp* console, std::string filename, const WCImage& image, size_t baseindex);
	void WriteFile(ToolApp* console, std::string filename, const void* data, size_t size);
	void PrintForm(ToolApp* console, FileEntryReader& reader, int depth);

	std::string mArchiveFilename;
};
