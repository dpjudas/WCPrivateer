#pragma once

#include "GameScreen.h"

enum class LoadSaveScreenState
{
	New,
	Load,
	Save
};

class LoadSaveScreen : public GameScreen
{
public:
	LoadSaveScreen(GameApp* app, LoadSaveScreenState state);

	void OnKeyDown(InputKey key) override;
	void OnKeyUp(InputKey key) override;
	void Render(RenderDevice* renderdev) override;

	enum ButtonName
	{
		None = -1,
		Save,
		Load,
		Missions,
		Finances,
		Manifest,
		UpArrow,
		DownArrow,
		LeftArrow,
		RightArrow,
		Power
	};

	ButtonName GetHotButton();

	LoadSaveScreenState state = {};

	std::vector<std::unique_ptr<GameTexture>> background;
	std::vector<std::unique_ptr<GameTexture>> font;


	int buttonAtMouseDown = 0;

	int buttonRects[10 * 4] =
	{
		218, 59, 254, 73,   // save
		258, 59, 290, 73,   // load
		218, 78, 293, 94,   // missions
		220, 97, 258, 114,  // finances
		261, 96, 294, 114,  // manifest
		254, 120, 271, 139, // up arrow
		255, 141, 272, 162, // down arrow
		226, 133, 251, 147, // left arrow
		274, 133, 298, 147, // right arrow
		224, 168, 258, 181  // power
	};
};
