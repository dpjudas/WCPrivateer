
#include "ConversationScreen.h"
#include "FileFormat/WCConversation.h"
#include "Game/GameApp.h"

ConversationScreen::ConversationScreen(GameApp* app) : GameScreen(app)
{
	backgroundList = std::make_unique<WCConvBackgroundList>(app->archive.get());
	faceList = std::make_unique<WCConvFaceList>(app->archive.get());
}

void ConversationScreen::Render(RenderDevice* renderdev)
{
	if (curBackground != nextBackground || curFace != nextFace)
	{
		background.clear();
		curBackground = nextBackground;

		const WCConvBackground& bg = backgroundList->backgrounds[curBackground];

		// For patent-pending reasons the palette is split between the face and the background
		WCPalette palette;
		for (WCPalette* p : { faceList->palette.get(), bg.palette.get() })
		{
			for (int i = p->start, end = p->start + p->count; i < end; i++)
			{
				palette.palette[i] = p->palette[i];
			}
		}

		background = LoadPakImage("DATA\\OPTIONS\\CU.PAK", bg.pakindex, &palette);

		head.clear();
		eyes.clear();
		mouth.clear();
		uniform.clear();
		hand.clear();
		curFace = nextFace;

		const WCConvFace& face = faceList->faces[curFace];
		if (face.head != -1)
		{
			head = LoadPakImage("DATA\\OPTIONS\\STATFACE.PAK", face.head, &palette);
			if (face.eyes != -1)
				eyes = LoadPakImage("DATA\\OPTIONS\\STATFACE.PAK", face.eyes, &palette);
			if (face.mouth != -1)
				mouth = LoadPakImage("DATA\\OPTIONS\\STATFACE.PAK", face.mouth, &palette);
			if (face.uniform != -1)
				uniform = LoadPakImage("DATA\\OPTIONS\\STATFACE.PAK", face.uniform, &palette);
			if (face.hand != -1)
				hand = LoadPakImage("DATA\\OPTIONS\\STATFACE.PAK", face.hand, &palette);
		}
		/*else
		{
			head = LoadPakImage("DATA\\OPTIONS\\RANDMALE.PAK", 0, &palette);
			eyes = LoadPakImage("DATA\\OPTIONS\\RANDMALE.PAK", 1, &palette);
			mouth = LoadPakImage("DATA\\OPTIONS\\RANDMALE.PAK", 2, &palette);
			uniform = LoadPakImage("DATA\\OPTIONS\\RANDMALE.PAK", 13, &palette);
		}*/
	}

	static int framecounter = 0;
	framecounter++;

	GameTexture* bgimage = background[(framecounter / 20) % background.size()].get();
	renderdev->DrawImage(bgimage->x + 160 - bgimage->width / 2, bgimage->y + 100 - bgimage->height / 2, bgimage->width, bgimage->height, bgimage);

	const WCConvFace& face = faceList->faces[curFace];

	int offsetY = 0;
	if (!uniform.empty())
	{
		int y = bgimage->y + 100 + bgimage->height / 2;
		offsetY = y - uniform[0]->height - uniform[0]->y - face.uniformY;
	}

	if (!head.empty())
	{
		GameTexture* img = head[(framecounter / 20) % head.size()].get();
		renderdev->DrawImage(160 + img->x, img->y + offsetY, img->width, img->height, img);
	}
	if (!eyes.empty())
	{
		GameTexture* img = eyes[(framecounter / 20) % eyes.size()].get();
		renderdev->DrawImage(160 + img->x + face.eyesX, img->y + offsetY + face.eyesY, img->width, img->height, img);
	}
	if (!mouth.empty())
	{
		GameTexture* img = mouth[(framecounter / 20) % mouth.size()].get();
		renderdev->DrawImage(160 + img->x + face.mouthX, img->y + offsetY + face.mouthY, img->width, img->height, img);
	}
	if (!uniform.empty())
	{
		GameTexture* img = uniform[(framecounter / 20) % uniform.size()].get();
		renderdev->DrawImage(160 + img->x + face.uniformX, img->y + offsetY + face.uniformY, img->width, img->height, img);
	}
	if (!hand.empty())
	{
		GameTexture* img = hand[(framecounter / 20) % hand.size()].get();
		renderdev->DrawImage(160 + img->x + face.handX, img->y + offsetY + face.handY, img->width, img->height, img);
	}
}

void ConversationScreen::OnKeyDown(InputKey key)
{
	if (key == InputKey::Left)
	{
		nextBackground = std::max(curBackground - 1, 0);
	}
	else if (key == InputKey::Right)
	{
		nextBackground = std::min(curBackground + 1, (int)backgroundList->backgrounds.size() - 1);
	}
	else if (key == InputKey::Up)
	{
		nextFace = std::max(curFace - 1, 0);
	}
	else if (key == InputKey::Down)
	{
		nextFace = std::min(curFace + 1, (int)faceList->faces.size() - 1);
	}
}
