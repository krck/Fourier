
#include "../header/Graphics.hpp"

using namespace Fourier;

void Graphics::Draw(const std::vector<Circle>& circles, std::vector<byte>& backgroundPixels, const Pixel& lastSumDot) {
	// Clear the Frame (white)
	SDL_SetRenderDrawColor(_renderer.get(), 255, 255, 255, 255);
	SDL_RenderClear(_renderer.get());

	// Draw the background Texture
	SDL_UpdateTexture(_background.get(), NULL, &backgroundPixels[0], _bgWidth * 4);
	SDL_RenderCopy(_renderer.get(), _background.get(), NULL, NULL);

	// Draw all Circles
	if (circles.size()) {
		// Draw the Grid-Lines
		const Pixel& center = circles.front().Center;
		SetSolidDrawing(false);
		SetColor(COLOR_GRAY_LIGHT);
		DrawLine_B(Pixel(center.X - 300, center.Y), Pixel(center.X + 300, center.Y));
		DrawLine_B(Pixel(center.X, center.Y - 300), Pixel(center.X, center.Y + 300));

		// And for each Circle draw: 
		// The Circle itself, a Dot on the circumference and a Line (Center to Dot)
		for (const Circle& c : circles) {
			SetColor(COLOR_GRAY_MEDIUM);
			SetSolidDrawing(false);
			DrawCircle(c.Radius, c.Center);
			DrawLine_B(c.Center, c.CycleDot);

			SetSolidDrawing(true);
			SetColor(COLOR_GREEN);
			DrawDot(4, c.CycleDot);
		}

		// Draw the SUM 
		// CycleDot on outer Circle + a Line on the Background Texture (Sum will be very dotted, with only Pixels)
		const Pixel& sumDot = circles.back().CycleDot;
		SetColor(COLOR_BLUE);
		DrawDot(4, sumDot);
		DrawLine_B_Background(backgroundPixels, lastSumDot, sumDot, COLOR_BLUE);
	}

	// Render to the Window
	SDL_RenderPresent(_renderer.get());
}

//
// Set a updated Background Texture (not the acutal Pixels tho)
// Should be called on every Window-Resize Event
//
void Graphics::UpdateBackground(std::shared_ptr<SDL_Texture> background, int bgWidth, int bgHeight) {
	_background = background;
	_bgWidth = bgWidth;
	_bgHeight = bgHeight;
}

//
// Naive Line-Drawing Algorithm
// By ChiliTomatoNoodle: https://github.com/planetchili/HUGS/blob/master/Engine/D3DGraphics.cpp
// Based on the "Cartesian slope-intercept equation": y = mx + b
//
// This implementation covers all cases (except 1-Pixel-Line) but is inefficient
// due to the number of operations and the need for floating-point types
//
void Graphics::DrawLine_N(const Pixel& from, const Pixel& to) {
	// Create copy of the Start and End Pixel for swapping. Then get the deltas
	float x0 = from.X, y0 = from.Y, x1 = to.X, y1 = to.Y;
	const float dx = x1 - x0;
	const float dy = y1 - y0;

	if (fabs(dx) > fabs(dy)) {
		// Calculation with X as the main/longer Axis (and swap, to always draw from left to right)
		if (dx < 0) { std::swap(x0, x1); std::swap(y0, y1); }
		const float m = dy / dx;
		const float b = y0 - m * x0;
		for (int x = x0; x <= x1; x++) {
			int y = ((m * x) + b + 0.5f);
			SetPixel(x, y);
		}
	}
	else {
		// Calculation with Y as the main/longer Axis (and swap, to always draw from top to bottom)
		if (dy < 0) { std::swap(y0, y1); std::swap(x0, x1); }
		const float m = dx / dy;
		const float b = x0 - (m * y0);
		for (int y = y0; y <= y1; y++) {
			int x = ((m * y) + b + 0.5f);
			SetPixel(x, y);
		}
	}
}

//
// Bresenham's Line-Drawing Algorithm
// Based on: https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
// Optimized Version of Bresenham's using "Error correction" instead of swaps
//
// Fun Fact: DrawLine_N is 1.4 times faster without compiler-optimization
//           DrawLine_B is ~27 times faster with compiler-optimization
//
void Graphics::DrawLine_B(const Pixel& from, const Pixel& to) {
	int x0 = from.X, y0 = from.Y, x1 = to.X, y1 = to.Y;
	const int deltaX = abs(x1 - x0);
	const int deltaY = abs(y1 - y0);
	// Check Slope direction (X Axis: [Left -] to [Right +], Y Axis: [Top -] to [Bottom +])
	const int sx = (x0 < x1) ? 1 : -1;
	const int sy = (y0 < y1) ? 1 : -1;
	int err = (deltaX > deltaY ? deltaX : -deltaY) / 2;
	int errTmp = err;

	for (;;) {
		// Draw until Line reached the last Pixel
		SetPixel(x0, y0);
		if (x0 == x1 && y0 == y1)
			break;

		errTmp = err;
		// Step the X Axis
		if (errTmp > -deltaX) {
			err -= deltaY;
			x0 += sx;
		}
		// Step the Y Axis
		if (errTmp < deltaY) {
			err += deltaX;
			y0 += sy;
		}
	}
}

//
// Same as DrawLine_B but Pixels are added on the Background Texture, not directly to the Renderer
// 
void Graphics::DrawLine_B_Background(std::vector<byte>& backgroundPixels, const Pixel& from, const Pixel& to, const Color& color) {
	int x0 = from.X, y0 = from.Y, x1 = to.X, y1 = to.Y;
	const int deltaX = abs(x1 - x0);
	const int deltaY = abs(y1 - y0);
	// Check Slope direction (X Axis: [Left -] to [Right +], Y Axis: [Top -] to [Bottom +])
	const int sx = (x0 < x1) ? 1 : -1;
	const int sy = (y0 < y1) ? 1 : -1;
	int err = (deltaX > deltaY ? deltaX : -deltaY) / 2;
	int errTmp = err;

	for (;;) {
		// Draw until Line reached the last Pixel
		SetBackgroundPixel(backgroundPixels, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break;

		errTmp = err;
		// Step the X Axis
		if (errTmp > -deltaX) {
			err -= deltaY;
			x0 += sx;
		}
		// Step the Y Axis
		if (errTmp < deltaY) {
			err += deltaX;
			y0 += sy;
		}
	}
}

//
// Circle-Drawing Algorithm
// By ChiliTomatoNoodle: https://github.com/planetchili/HUGS/blob/master/Engine/D3DGraphics.cpp
//
// Get any Point on the Circle via the right-angled triangle (Radius == Hypotenuse)
// Convert Pythagoras (r^2 = x^2 + y^2) to get Y based on R and X: y = √(r^2 - x^2)
//
void Graphics::DrawCircle(ushort radius, const Pixel& center) {
	const int cX = center.X, cY = center.Y;
	const int rs = (radius * radius);
	// Only loop X for 1/8 of the circumference and calculate the according Y values
	// (Speeds up the Algorithm and deals with dotted lines, when Y gets to "steap")
	const int xRange = (radius * 0.70710678118f + 0.5f);
	for (int x = 0; x <= xRange; x++)
	{
		// Draw the calculated Pixel and mirror it to the other 7 parts of the Circle
		// Also translate the Circle position form 0,0 to the actual Center (+cX, +cY)
		const int y = (sqrt(rs - (x * x)) + 0.5f);
		SetPixel(cX + x, cY + y);
		SetPixel(cX - x, cY + y);
		SetPixel(cX + x, cY - y);
		SetPixel(cX - x, cY - y);
		SetPixel(cX + y, cY + x);
		SetPixel(cX - y, cY + x);
		SetPixel(cX + y, cY - x);
		SetPixel(cX - y, cY - x);
	}
}

//
// Filled-Circle-Drawing Algorithm
// Based on: https://stackoverflow.com/a/1237519
//
// Simple (brute-force) way of drawing a Dot, only using integer multiplications and additions
// For a very basic Version use:					if (x * x + y * y <= radius * radius)
// For "Close to" Midpoint-Algorithm result use:	if (x * x + y * y <= radius * radius + radius * 0.8f)
//
void Graphics::DrawDot(ushort radius, const Pixel& center) {
	const int cX = center.X, cY = center.Y;
	for (int y = -radius; y <= radius; y++)
		for (int x = -radius; x <= radius; x++)
			if (x * x + y * y <= radius * radius + radius * 0.8f)
				SetPixel(cX + x, cY + y);
}

//
// Sin-Wave drawing Algorithm
//
// Updated sin() calculation with variable frequency, height and intersection
// y = scaling * sin((angle - offsetX) / frequency) + offsetY
// Using adjusted step-speeds on the X Axis to draw a smoother curve (less pixelated)
//
void Graphics::DrawSinWave(const Pixel& start, const Pixel& offset, float heightScaling, float frequency, float waveCount) {
	const float a = heightScaling; // Default heightScaling = 1.0f
	const float f = frequency; // Default frequency = 1.0f
	const int waves = (waveCount * 360) * frequency;

	int xPos = 0, yPos = 0;
	float angleRad = 0.0f, y = 0.0f, yAbs = 0.0f, steps = 0.5f;
	for (float x = 0; x <= waves; x += steps) {
		// Convert degree Angle to radians and get the sin() as Y coordinate
		angleRad = (x * M_PI) / 180.0f;
		y = a * sin(angleRad / f);
		yAbs = fabs(y);

		// Adjust steps to draw more Pixels on the steap parts or on scaled-up waves
		// Start of wave: Step X with 0.5 (half Pixelz!!1), then 0.7 and on top with 1.0
		steps = ((yAbs > 0.8) ? 1.0f : (yAbs > 0.5) ? 0.75f : 0.5f) / heightScaling;

		// Translate x and y to the actual drawing Positions
		xPos = x + start.X + offset.X;
		yPos = (y * 100) + start.Y + offset.Y;
		SetPixel(xPos, yPos);
	}
}

//
// Set the Drawing Color for all Pixels rendered after this call
//
void Graphics::SetColor(const Color& color) {
	SDL_SetRenderDrawColor(_renderer.get(), color.r, color.g, color.b, color.a);
}

//
// Draw a Pixel
// (In case Solid-Drawing is active: Draw 9 Pixels instead of just one for better visibility)
// 
void Graphics::SetPixel(const Pixel& pixel) { SetPixel(pixel.X, pixel.Y); }
void Graphics::SetPixel(const ushort& x, const ushort& y) {
	SDL_RenderDrawPoint(_renderer.get(), x, y);
	if (_solidDrawing) {
		SDL_RenderDrawPoint(_renderer.get(), x - 1, y);
		SDL_RenderDrawPoint(_renderer.get(), x + 1, y);
		SDL_RenderDrawPoint(_renderer.get(), x, y - 1);
		SDL_RenderDrawPoint(_renderer.get(), x, y + 1);
		SDL_RenderDrawPoint(_renderer.get(), x - 1, y - 1);
		SDL_RenderDrawPoint(_renderer.get(), x + 1, y - 1);
		SDL_RenderDrawPoint(_renderer.get(), x - 1, y + 1);
		SDL_RenderDrawPoint(_renderer.get(), x + 1, y + 1);
	}
}

//
// Draw a Pixel onto the Background Texture
//
void Graphics::SetBackgroundPixel(std::vector<byte>& backgroundPixels, ushort x, ushort y, const Color& color) {
	// Get the offset to access a "two-dimensional" point in the one-dimensional array
	const uint offset = (_bgWidth * 4 * y) + x * 4;
	backgroundPixels[offset + 0] = color.b;
	backgroundPixels[offset + 1] = color.g;
	backgroundPixels[offset + 2] = color.r;
	backgroundPixels[offset + 3] = 0;
}
