#pragma once

class Widget;

class CachedTexture
{
public:
	virtual ~CachedTexture() = default;
};

class GameTexture
{
public:
	std::unique_ptr<CachedTexture> CacheEntry;

	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
	std::vector<uint32_t> pixels;
};

class RenderDevice
{
public:
	static std::unique_ptr<RenderDevice> Create(Widget* viewport);

	virtual ~RenderDevice() = default;

	virtual bool Begin() = 0;
	virtual void DrawImage(int x, int y, int width, int height, GameTexture* gameTexture, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) = 0;
	virtual void End() = 0;
};
