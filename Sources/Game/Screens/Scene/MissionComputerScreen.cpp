
#include "MissionComputerScreen.h"
#include "BaseScreen.h"
#include "Game/GameApp.h"
#include <cstdio>

MissionComputerScreen::MissionComputerScreen(GameApp* app, const WCGameFlowTarget* target) : SceneScreen(app)
{
	nextScene = 58;

	screenAlign = GameTextAlignment::Center;
	screenText = app->gamedata->missionComputerText["WELC"]; // Welcome to mission computer
	/*
	screenText = app->gamedata->missionComputerText["SCAN"]; // Scanning for missions
	screenText = app->gamedata->missionComputerText["UNAV"]; // No missions available
	screenText = app->gamedata->missionComputerText["ACPT"]; // Mission accepted
	screenText = app->gamedata->missionComputerText["NROM"]; // No room for more missions

	// Mission generation:

	screenAlign = GameTextAlignment::Left;

	// Mission status: 6 available missions, 1 current mission:
	screenText = FormatText(app->gamedata->missionComputerText["CMIS"], 6, 1);

	// Mission header (BOUN = bounty, ATAK = attack, CRGO = cargo, DFND = defend base, PTRL = patrol, SCOU = scout
	screenText += app->gamedata->missionComputerText["BOUN"];

	// Mission body text
	screenText += app->gamedata->missionText.summary["BNTY"];
	ReplaceString(screenText, "$EN", app->gamedata->missionText.enemies[6]);
	ReplaceString(screenText, "$DO", "New Detroit"); // To do: load system names from somewhere
	ReplaceString(screenText, "$CL", app->gamedata->missionText.corporations[25]);
	ReplaceString(screenText, "$PY", "10000");
	*/
}

void MissionComputerScreen::ReplaceString(std::string& text, std::string search, std::string replace)
{
	size_t lastpos = 0;
	while (true)
	{
		size_t pos = text.find(search, lastpos);
		if (pos == std::string::npos)
			break;
		text.replace(pos, search.size(), replace);
		pos += replace.size();
		lastpos = pos;
	}
}

std::string MissionComputerScreen::FormatText(std::string format, ...)
{
	va_list args1;
	va_start(args1, format);
	va_list args2;
	va_copy(args2, args1);
	std::vector<char> buf(1 + std::vsnprintf(nullptr, 0, format.c_str(), args1));
	va_end(args1);
	std::vsnprintf(buf.data(), buf.size(), format.c_str(), args2);
	va_end(args2);
	return buf.data();
}

void MissionComputerScreen::Render(RenderDevice* renderdev)
{
	SceneScreen::Render(renderdev);
	if (!computerscreen)
	{
		auto gameTexture = std::make_unique<GameTexture>();
		gameTexture->x = 54;
		gameTexture->y = 29;
		gameTexture->width = 160;
		gameTexture->height = 101;
		gameTexture->pixels.resize(160 * 101, 0xff000000);
		computerscreen = std::move(gameTexture);

		smallFont = LoadMssgFont(palette.get(), 46);
	}

	renderdev->DrawImage(0, 0, computerscreen.get());

	if (screenAlign == GameTextAlignment::Left)
		DrawText(renderdev, 56, 35, WordWrap(screenText, 156, smallFont), smallFont, screenAlign);
	else
		DrawText(renderdev, 56 + 156 / 2, 35, WordWrap(screenText, 156, smallFont), smallFont, screenAlign);

	// DrawSprite(renderdev, 0, 100, 100);
}

void MissionComputerScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::MainConcourse)
	{
		ShowScreen(std::make_unique<BaseScreen>(app, GetFlowTarget(target)));
	}
	else if (target == WCTarget::ActivateComputer)
	{
		screenAlign = GameTextAlignment::Left;
		//screenText = app->gamedata->missionComputerText["UNAV"]; // No missions available
		screenText = app->gamedata->missionComputerText["SCAN"]; // Scanning for missions
	}
}
