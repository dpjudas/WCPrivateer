#pragma once

#include "Commandlet/Commandlet.h"

class ToolApp
{
public:
	int Main(std::vector<std::string> args);

	void WriteOutput(const std::string& text);

	void WriteOutput(std::string value, size_t len)
	{
		WriteOutput(value);
		if (value.length() < len)
		{
			std::string spaces(len - value.length(), ' ');
			WriteOutput(spaces);
		}
	}

	void WriteOutput(int value, size_t len)
	{
		WriteOutput(std::to_string(value), len);
	}

	void WriteOutputUI8(void* value, size_t len)
	{
		WriteOutput(std::to_string(*(uint8_t*)value), len);
	}

	void Exit() { ExitRequested = true; }
	//void RunGame();

	static std::string ResetEscape();
	static std::string ColorEscape(int color);
	static std::string DeleteCharacterEscape(int count);
	static std::string EraseCharacterEscape(int count);
	static std::string CursorBackward(int count);
	static std::string NewLine();
	static std::string InputNewLine();

	bool ResumeProgram = false;

private:
	void CreateCommandlets();
	void Tick();

	//void PrintLog(const LogMessageLine& line);
	static std::string ToFixed(float time);

	void OnCommandEntered(const std::string& line);
	void WritePrompt();
	void EndPrompt();

	void WaitForInput();
	std::string ReadInput();

	void FrameDebugBreak();

	std::vector<std::unique_ptr<Commandlet>> Commandlets;

	std::string promptline;
	bool ExitRequested = false;

	bool PromptLineActive = false;
	bool GameRunning = false;

	friend class Commandlet;
};
