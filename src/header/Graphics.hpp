#pragma once

#include "Circle.hpp"
#include "Color.hpp"
#include <math.h>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <iostream>

class Graphics {
private:
	std::shared_ptr<SDL_Renderer> _renderer;
	std::shared_ptr<SDL_Texture> _background;
	int _bgWidth, _bgHeight;
	bool _solidDrawing;

public:
	Graphics(std::shared_ptr<SDL_Renderer> renderer) : _renderer(renderer), _bgWidth(0), _bgHeight(0), _solidDrawing(false) {}
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void Draw(const std::vector<Circle>& circles, std::vector<byte>& backgroundPixels, const Pixel& lastSumDot);
	void UpdateBackground(std::shared_ptr<SDL_Texture> background, int bgWidth, int bgHeight);

	~Graphics() = default;

private:
	void DrawLine_N(const Pixel& from, const Pixel& to);
	void DrawLine_B(const Pixel& from, const Pixel& to);
	void DrawLine_B_Background(std::vector<byte>& backgroundPixels, const Pixel& from, const Pixel& to, const Color& color);
	void DrawCircle(ushort radius, const Pixel& center);
	void DrawDot(ushort radius, const Pixel& center);
	void DrawSinWave(const Pixel& start, const Pixel& offset, float heightScaling = 1.0f, float frequency = 1.0f, float waveCount = 1.0f);

	inline void SetSolidDrawing(bool sd) { _solidDrawing = sd; }
	void SetColor(const Color& color);
	void SetPixel(const Pixel& pixel);
	void SetPixel(const ushort& x, const ushort& y);
	void SetBackgroundPixel(std::vector<byte>& backgroundPixels, ushort x, ushort y, const Color& color);
};
