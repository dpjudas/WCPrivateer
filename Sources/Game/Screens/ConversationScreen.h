#pragma once

#include "GameScreen.h"
#include "FileFormat/WCConversation.h"

class ConversationScreen : public GameScreen
{
public:
	ConversationScreen(GameApp* app, const std::string& conversationName);

	void OnKeyDown(InputKey key) override;
	void Render(RenderDevice* renderdev) override;

	std::unique_ptr<WCConvBackgroundList> backgroundList;
	std::unique_ptr<WCConvFaceList> faceList;
	std::unique_ptr<WCConversation> conversation;

	std::vector<std::unique_ptr<GameTexture>> font;

	std::vector<std::unique_ptr<GameTexture>> background;
	std::vector<std::unique_ptr<GameTexture>> head;
	std::vector<std::unique_ptr<GameTexture>> eyes;
	std::vector<std::unique_ptr<GameTexture>> mouth;
	std::vector<std::unique_ptr<GameTexture>> uniform;
	std::vector<std::unique_ptr<GameTexture>> hand;

	int curBackground = -1;
	int nextBackground = 0;

	int curFace = -1;
	int nextFace = 0;
};
