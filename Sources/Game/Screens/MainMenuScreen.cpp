
#include "MainMenuScreen.h"
#include "LoadSaveScreen.h"
#include "OptionsScreen.h"
#include "FileFormat/WCScene.h"
#include "Game/GameApp.h"

MainMenuScreen::MainMenuScreen(GameApp* app) : GameScreen(app)
{
}

MainMenuScreen::~MainMenuScreen()
{
}

void MainMenuScreen::Render(RenderDevice* renderdev)
{
	if (background.empty())
	{
		auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", 39);
		background = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", 181, palette.get());
		buttons = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", 182, palette.get());
	}

	renderdev->DrawImage(background[0]->x, background[0]->y, background[0]->width, background[0]->height, background[0].get());

	if (buttonAtMouseDown != 0)
	{
		int index = GetHotButton();
		if (index == buttonAtMouseDown)
			renderdev->DrawImage(buttons[index]->x, buttons[index]->y, buttons[index]->width, buttons[index]->height, buttons[index].get());
	}
}

void MainMenuScreen::OnKeyDown(InputKey key)
{
	if (key == InputKey::LeftMouse)
	{
		buttonAtMouseDown = GetHotButton();
	}
}

void MainMenuScreen::OnKeyUp(InputKey key)
{
	if (key == InputKey::LeftMouse)
	{
		buttonAtMouseDown = 0;
		int action = GetHotButton();
		if (action == 1)
		{
			ShowScreen(std::make_unique<LoadSaveScreen>(app, LoadSaveScreenState::New));
		}
		else if (action == 2)
		{
			ShowScreen(std::make_unique<LoadSaveScreen>(app, LoadSaveScreenState::Load));
		}
		else if (action == 3)
		{
			PushScreen(std::make_unique<OptionsScreen>(app));
		}
		else if (action == 4)
		{
			PopScreen();
		}
	}
}

int MainMenuScreen::GetHotButton()
{
	if (mouseY >= buttonTop && mouseY <= buttonBottom)
	{
		if (mouseX >= newLeft && mouseX <= newRight)
		{
			return 1;
		}
		else if (mouseX >= loadLeft && mouseX <= loadRight)
		{
			return 2;
		}
		else if (mouseX >= optionsLeft && mouseX <= optionsRight)
		{
			return 3;
		}
		else if (mouseX >= quitLeft && mouseX <= quitRight)
		{
			return 4;
		}
	}
	return 0;
}
