
#include "Precomp.h"
#include "QuitCommandlet.h"
#include "Tool/ToolApp.h"

QuitCommandlet::QuitCommandlet()
{
	SetShortFormName("q");
	SetLongFormName("quit");
	SetShortDescription("Exit the debugger");
}

void QuitCommandlet::OnCommand(ToolApp* console, const std::string& args)
{
	console->Exit();
}

void QuitCommandlet::OnPrintHelp(ToolApp* console)
{
}
