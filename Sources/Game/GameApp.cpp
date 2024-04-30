
#include "Precomp.h"
#include "GameApp.h"
#include "RenderDevice.h"
#include "Game/Screens/GameScreen.h"
#include "Game/Screens/MainMenuScreen.h"
#include "Game/Screens/LoadSaveScreen.h"
#include "Game/Screens/ShipDealerScreen.h"
#include "Game/Screens/MissionComputerScreen.h"
#include "Game/Screens/BarScreen.h"
#include "Game/Screens/BaseScreen.h"
#include "Game/Screens/HangarScreen.h"
#include "FileFormat/WCArchive.h"
#include "zwidget/core/theme.h"
#include <zwidget/core/widget.h>
#include <zwidget/window/window.h>
#include <zwidget/core/resourcedata.h>

bool exitFlag;

class GameWindow : public Widget
{
public:
	GameWindow() : Widget(nullptr, WidgetType::Window)
	{
		SetWindowTitle("Wing Commander: Privateer");
		Size screenSize = Widget::GetScreenSize();
		Size size(320.0 * 4.0, 200 * 4.0 * 1.2);
		SetFrameGeometry((screenSize.width - size.width) * 0.5, (screenSize.height - size.height) * 0.5, size.width, size.height);
	}

	void OnClose() override
	{
		exitFlag = true;
	}
};

int GameApp::main(std::vector<std::string> args)
{
	try
	{
		WidgetTheme::SetTheme(std::make_unique<DarkWidgetTheme>());

		archive = std::make_unique<WCArchive>("PRIV.TRE");

		GameWindow window;
		window.Show();

		auto screen = std::make_unique<MainMenuScreen>(this);

		auto renderdev = RenderDevice::Create(&window);
		while (!exitFlag)
		{
			DisplayWindow::ProcessEvents();

			if (!renderdev->Begin())
				continue;

			screen->Render(renderdev.get());

			renderdev->End();
		}

		return 0;
	}
	catch (const std::exception& e)
	{
		MessageBoxA(0, e.what(), "Unhandled Exception", MB_OK);
		return 0;
	}

	return 0;
}

void VulkanPrintLog(const char* typestr, const std::string& msg)
{
	// Log messages from the validation layer here
}

void VulkanError(const char* message)
{
	throw std::runtime_error(message);
}

std::vector<SingleFontData> LoadWidgetFontData(const std::string& name)
{
	return {};
}

std::vector<uint8_t> LoadWidgetData(const std::string& name)
{
	return {};
}
