
#include "Precomp.h"
#include "GameApp.h"
#include "RenderDevice.h"
#include "FileFormat/WCArchive.h"
#include "FileFormat/WCPak.h"
#include "FileFormat/WCImage.h"
#include "FileFormat/WCPalette.h"
#include "FileFormat/FileEntryReader.h"
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

		GameWindow window;
		window.Show();

		WCArchive archive("PRIV.TRE");

		std::unique_ptr<WCPalette> palette;// = std::make_unique<WCPalette>("DATA\\PALETTE\\SPACE.PAL", archive);
		{
			WCPak palpak("DATA\\OPTIONS\\OPTPALS.PAK", archive);
			palette = std::make_unique<WCPalette>(palpak.openFile(39));
		}

		WCPak pak("DATA\\OPTIONS\\OPTSHPS.PAK", archive);
		FileEntryReader reader = pak.openFile(181);
		WCImage image(reader, palette.get());

		GameTexture gameTexture;
		gameTexture.x = image.frames[0].x;
		gameTexture.y = image.frames[0].y;
		gameTexture.width = image.frames[0].width;
		gameTexture.height = image.frames[0].height;
		gameTexture.pixels = image.frames[0].pixels;

		auto renderdev = RenderDevice::Create(&window);
		while (!exitFlag)
		{
			DisplayWindow::ProcessEvents();

			if (!renderdev->Begin())
				continue;

			renderdev->DrawImage(0, 0, 320, 200, &gameTexture);

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
