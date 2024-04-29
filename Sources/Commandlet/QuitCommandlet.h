#pragma once

#include "Commandlet/Commandlet.h"

class QuitCommandlet : public Commandlet
{
public:
	QuitCommandlet();

	void OnCommand(ToolApp* console, const std::string& args) override;
	void OnPrintHelp(ToolApp* console) override;
};
