
#include "MissionComputerScreen.h"
#include "BaseScreen.h"
#include "Game/GameApp.h"

MissionComputerScreen::MissionComputerScreen(GameApp* app) : SceneScreen(app)
{
	nextScene = 58;
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

		smallFont = LoadPrivFont(palette.get());
	}

	renderdev->DrawImage(computerscreen->x, computerscreen->y, computerscreen->width, computerscreen->height, computerscreen.get());

	// DrawSprite(renderdev, 0, 100, 100);
}

void MissionComputerScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::MainConcourse)
	{
		ShowScreen(std::make_unique<BaseScreen>(app));
	}
}
