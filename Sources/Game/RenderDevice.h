#pragma once

#include <cmath>

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

	virtual void DrawImageBox(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, GameTexture* gameTexture, float r, float g, float b, float a) = 0;

	void DrawImage(int x, int y, int width, int height, GameTexture* gameTexture, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
	{
		x += gameTexture->x;
		y += gameTexture->y;

		float x1 = (float)x;
		float x2 = (float)(x + width);
		float y1 = (float)y;
		float y2 = (float)(y + height);

		DrawImageBox(x1, y1, x2, y1, x2, y2, x1, y2, gameTexture, r, g, b, a);
	}

	void DrawImage(int x, int y, GameTexture* gameTexture, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
	{
		DrawImage(x, y, gameTexture->width, gameTexture->height, gameTexture, r, g, b, a);
	}

	void Draw3DImage(float x, float y, float scale, float rotation, GameTexture* gameTexture, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
	{
		float dirXx = std::cos(rotation * (3.14159265359f / 180.0f));
		float dirXy = std::sin(rotation * (3.14159265359f / 180.0f));
		float dirYy = -dirXx;
		float dirYx = dirXy;

		float x0 = gameTexture->x * scale;
		float y0 = gameTexture->y * scale;
		float x1 = (gameTexture->x + gameTexture->width) * scale;
		float y1 = (gameTexture->y + gameTexture->height) * scale;

		DrawImageBox(
			x + dirXx * x0 + dirYx * y0,
			y + dirXy * x0 + dirYy * y0,
			x + dirXx * x1 + dirYx * y0,
			y + dirXy * x1 + dirYy * y0,
			x + dirXx * x1 + dirYx * y1,
			y + dirXy * x1 + dirYy * y1,
			x + dirXx * x0 + dirYx * y1,
			y + dirXy * x0 + dirYy * y1,
			gameTexture,
			r, g, b, a);

	}

	virtual void End() = 0;
};
