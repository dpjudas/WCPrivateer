
#include "Precomp.h"
#include "GameApp.h"
#include "RenderDevice.h"
#include "Game/Screens/GameScreen.h"
#include "Game/Screens/MainMenuScreen.h"
#include "Game/Screens/LoadSaveScreen.h"
#include "Game/Screens/OptionsScreen.h"
#include "Game/Screens/JoystickScreen.h"
#include "Game/Screens/Scene/SceneScreen.h"
#include "Game/Screens/Scene/MissionComputerScreen.h"
#include "Game/Screens/Scene/BarScreen.h"
#include "Game/Screens/Scene/BaseScreen.h"
#include "Game/Screens/Scene/HangarScreen.h"
#include "Game/Screens/Movie/MovieScreen.h"
#include "Game/Screens/Movie/IntroScreen.h"
#include "Game/Screens/Movie/JumpScreen.h"
#include "Game/Screens/Movie/LandingScreen.h"
#include "Game/Screens/Movie/TakeoffScreen.h"
#include "Game/Screens/Movie/VictoryScreen.h"
#include "FileFormat/WCArchive.h"
#include "FileFormat/WCMusic.h"
#include "FileFormat/WCImage.h"
#include "FileFormat/WCPalette.h"
#include "FileFormat/FileEntryReader.h"
#include "Audio/AudioPlayer.h"
#include "Audio/AudioSource.h"
#include "zwidget/core/theme.h"
#include <zwidget/core/widget.h>
#include <zwidget/window/window.h>
#include <zwidget/core/resourcedata.h>

struct GameInputEvent
{
	enum Type
	{
		keyDown,
		keyUp,
		mouseDown,
		mouseUp,
		mouseMove
	};
	Type type = {};
	Point pos;
	InputKey key = {};
};

bool exitFlag;
std::vector<GameInputEvent> gameEvents;

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

	void OnKeyDown(InputKey key) override
	{
		GameInputEvent e;
		e.type = GameInputEvent::keyDown;
		e.key = key;
		gameEvents.push_back(e);
	}

	void OnClose() override
	{
		exitFlag = true;
	}

	bool OnMouseDown(const Point& pos, InputKey key) override
	{
		GameInputEvent e;
		e.type = GameInputEvent::mouseDown;
		e.key = key;
		e.pos = pos;
		gameEvents.push_back(e);
		return true;
	}

	bool OnMouseDoubleclick(const Point& pos, InputKey key) override
	{
		return OnMouseDown(pos, key);
	}

	bool OnMouseUp(const Point& pos, InputKey key) override
	{
		GameInputEvent e;
		e.type = GameInputEvent::mouseUp;
		e.key = key;
		e.pos = pos;
		gameEvents.push_back(e);
		return true;
	}

	void OnMouseMove(const Point& pos)
	{
		GameInputEvent e;
		e.type = GameInputEvent::mouseMove;
		e.pos = pos;
		gameEvents.push_back(e);
	}
};

static HCURSOR CreateAlphaCursor(const WCImageFrame& source)
{
	// Find closest integer scale factor for the monitor DPI
	HDC screenDC = GetDC(0);
	int dpi = GetDeviceCaps(screenDC, LOGPIXELSX);
	int scale = std::max((dpi + 96 / 2 - 1) / 96, 1) * 3;
	ReleaseDC(0, screenDC);

	int w = source.width;
	int h = source.height;

	BITMAPV5HEADER bi = {};
	bi.bV5Size = sizeof(bi);
	bi.bV5Width = w * scale;
	bi.bV5Height = h * scale;
	bi.bV5Planes = 1;
	bi.bV5BitCount = 32;
	bi.bV5Compression = BI_BITFIELDS;
	bi.bV5RedMask = 0x00FF0000;
	bi.bV5GreenMask = 0x0000FF00;
	bi.bV5BlueMask = 0x000000FF;
	bi.bV5AlphaMask = 0xFF000000;

	HDC dc = GetDC(0);
	if (dc == 0) return 0;

	void* bits = nullptr;
	HBITMAP color = CreateDIBSection(dc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &bits, 0, 0);
	ReleaseDC(0, dc);
	if (!color)
		return 0;

	HBITMAP mono = CreateBitmap(w * scale, h * scale, 1, 1, 0);
	if (!mono)
	{
		DeleteObject(color);
		return 0;
	}

	const uint32_t* unscaled = source.pixels.data();
	uint32_t* scaled = (uint32_t*)bits;
	for (int y = 0; y < h * scale; y++)
	{
		for (int x = 0; x < w * scale; x++)
		{
			scaled[x + y * w * scale] = unscaled[x / scale + y / scale * w];
		}
	}

	ICONINFO iconinfo = { FALSE, (DWORD)(-source.x * scale), (DWORD)(-source.y * scale), mono, color };
	HCURSOR cursor = CreateIconIndirect(&iconinfo);

	DeleteObject(mono);
	DeleteObject(color);

	return cursor;
}

#define PLAY_MUSIC

int GameApp::main(std::vector<std::string> args)
{
	try
	{
		WidgetTheme::SetTheme(std::make_unique<DarkWidgetTheme>());

		archive = std::make_unique<WCArchive>("PRIV.TRE");

		GameWindow window;
		window.SetFocus();
		window.ShowMaximized();

		auto renderdev = RenderDevice::Create(&window);

		auto palette = std::make_unique<WCPalette>("DATA\\PALETTE\\PCMAIN.PAL", archive.get());
		FileEntryReader cursorShp = archive->openFile("DATA\\MOUSE\\PNT.SHP");
		WCImage cursor(cursorShp, palette.get());
		HCURSOR win32cursor = CreateAlphaCursor(cursor.frames[0]);

#ifdef PLAY_MUSIC
		WCMusic music("DATA\\SOUND\\BASETUNE.GEN", archive.get());

		ZMusic_MusicStream song = AudioSource::OpenSong(music.songs[8]);
		int subsong = 0;
		bool loop = true;
		bool result = ZMusic_Start(song, subsong, loop);

		std::unique_ptr<AudioPlayer> audioPlayer;
		SoundStreamInfo fmt;
		ZMusic_GetStreamInfo(song, &fmt);
		if (fmt.mBufferSize != 0)
			audioPlayer = AudioPlayer::Create(AudioSource::CreateZMusic(song));
#endif

		auto screen = std::make_unique<SceneScreen>(this);

		while (!exitFlag)
		{
			DisplayWindow::ProcessEvents();
#ifdef PLAY_MUSIC
			ZMusic_Update(song);
#endif
			SetCursor(win32cursor);

			double letterboxwidth = 320 * window.GetHeight() / 240;
			double width = window.GetWidth();
			for (const GameInputEvent& e : gameEvents)
			{
				if (e.type == GameInputEvent::mouseMove || e.type == GameInputEvent::mouseDown || e.type == GameInputEvent::mouseUp)
				{
					double mx = (e.pos.x - (width - letterboxwidth) * 0.5) * 320.0 / letterboxwidth;
					double my = e.pos.y * 200 / window.GetHeight();
					screen->SetMousePos((int)std::round(mx), (int)std::round(my));
				}
				
				if (e.type == GameInputEvent::mouseDown)
				{
					screen->OnKeyDown(e.key);
				}
				else if (e.type == GameInputEvent::mouseUp)
				{
					screen->OnKeyUp(e.key);
				}
				else if (e.type == GameInputEvent::keyDown)
				{
					screen->OnKeyDown(e.key);
				}
				else if (e.type == GameInputEvent::keyUp)
				{
					screen->OnKeyUp(e.key);
				}
			}
			gameEvents.clear();

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
