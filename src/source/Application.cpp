
#include "../header/Application.hpp"

using namespace Fourier;

Application::Application(std::string&& appName, uint&& windowWidth, uint&& windowHeight, std::string&& resourcePath)
	: _appName(std::move(appName)), _windowWidth(std::move(windowWidth)), _windowHeight(std::move(windowHeight)),
	_resourcePath(std::move(resourcePath)), _surface(nullptr), _font(nullptr), _actualWidth(0), _actualHeight(0) {}

Application::Application(const std::string& appName, const uint& windowWidth, const uint& windowHeight, const std::string& resourcePath)
	: _appName(appName), _windowWidth(windowWidth), _windowHeight(windowHeight), _resourcePath(resourcePath),
	_surface(nullptr), _font(nullptr), _actualWidth(0), _actualHeight(0) {}

//
// Initializes the SDL and KiwiGUI Ressources
// and also creats/shows the Apps main Window
//
bool Application::InitApplication() {
	try {
		// Initialize all SDL2 subsystems
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
			throw FourierException("SDL Error on Init: " + std::string(SDL_GetError()));

		SetupWindow();
		SetupSDL();
		SetupKiwiGUI();

		return true;
	}
	catch (const std::exception& ex) {
		std::cout << ex.what() << std::endl;
		return false;
	}
}

//
// Run the Applications Main Loop
// Process SDL Events and Draw the Frames
//
int Application::Run() {
	try {
		Graphics graphics = Graphics(_renderer);
		Transformations transform = Transformations();
		OnWindowResize(graphics);

		// Manually define the Circles for now
		// ... this should be done via GUI input
		int angle = 0;
		Pixel circleCenter(800, 400);
		std::vector<Circle> circles{
			{circleCenter, 120, 0, 1},
			{circleCenter, 100, 45, 2},
			{circleCenter, 80, 20, 4},
			{circleCenter, 60, 0, 6},
			{circleCenter, 40, 90, 8},
			{circleCenter, 20, 0, 10}
		};
		// Workaround to get the Startpoint of the Sum-Line
		auto tmpCircles(circles);
		transform.Transform(tmpCircles, 1);
		Pixel lastSumDot = tmpCircles.back().CycleDot;

		// Main Loop
		SDL_Event event;
		uint timeStamp_new = SDL_GetTicks(), timeStamp_old = SDL_GetTicks();
		while (!SDL_QuitRequested()) {
			// FPS Limiter (Nice, because it works without WAIT)
			timeStamp_new = SDL_GetTicks();
			if ((timeStamp_new - timeStamp_old) > (1000.0f / FPS)) {
				timeStamp_old = timeStamp_new;

				// Handle all Events in the queue
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
						OnWindowResize(graphics);
				}

				// Update and draw the GUI (Bottom Layer)
				KW_ProcessEvents(_gui.get());
				KW_Paint(_gui.get());

				// Constant rotation of one Degree per Frame
				// Frequency 1 == "6 sec. for one full rotation"
				angle = (angle > 360) ? 1 : angle + 1;

				// Transform and draw all circles (Top Layer)
				transform.Transform(circles, angle);
				graphics.Draw(circles, _backgroundPixels, lastSumDot);
				lastSumDot = circles.back().CycleDot;
			}
		}

		return 0;
	}
	catch (const std::exception& ex) {
		std::cout << ex.what() << std::endl;
		return -1;
	}
}

//
// Create and Show the Application Window
//
void Application::SetupWindow() {
	// Create and Show the main Window
	const uint pos = SDL_WINDOWPOS_UNDEFINED;
	const uint flags = (SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
	_window = sdl_make_shared(SDL_CreateWindow(_appName.c_str(), pos, pos, 1280, 720, flags));
	
	// Set the min. Window size, if creation was successful 
	if (_window != nullptr)
		SDL_SetWindowMinimumSize(_window.get(), 1280, 720);
	else
		throw FourierException("SDL Error on Window creation: " + std::string(SDL_GetError()));
}

//
// Create the SDL2 Renderer and a background Texture
//
void Application::SetupSDL() {
	// Create the Renderer to draw within the Window (-1 for default Window driver)
	// Set only the SDL_RENDERER_ACCELERATED Flag and NO Vsync! (Managing Frame-Times is important)
	_renderer = sdl_make_shared(SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED));
	if (_renderer == nullptr)
		throw FourierException("SDL Error on Renderer creation: " + std::string(SDL_GetError()));
}

//
// Create all the Ressources needed to work with the KiwiGUI
//
void Application::SetupKiwiGUI() {
	// Initialize the KiWi SDL2 Render Driver
	// You can have multiple GUI instances in the same/different windows, based on the Render Driver
	_driver = kiwi_make_shared(KW_CreateSDL2RenderDriver(_renderer.get(), _window.get()));
	if (_driver == nullptr)
		throw FourierException("KiWi GUI Error creating GUI Driver: " + std::string(SDL_GetError()));

	_surface = KW_LoadSurface(_driver.get(), (_resourcePath + "tileset.png").c_str());
	if (_surface == nullptr)
		throw FourierException("KiWi GUI Error creating GUI Surface:" + std::string(SDL_GetError()));

	// Finally create the GUI
	_gui = kiwi_make_shared(KW_Init(_driver.get(), _surface));
	if (_gui == nullptr)
		throw FourierException("KiWi GUI Error setting up the GUI: " + std::string(SDL_GetError()));

	// Load the font and set it with Font-size 12
	_font = KW_LoadFont(_driver.get(), (_resourcePath + "sourcesans-pro-semibold.ttf").c_str(), 12);
	if (_font != nullptr)
		KW_SetFont(_gui.get(), _font);
	else
		throw FourierException("KiWi GUI Error loading Font: " + std::string(SDL_GetError()));
}

//
// Called on Window-Resize Event
//
void Application::OnWindowResize(Graphics& graphics) {
	// Update the Width and Height values based on the new Window size
	SDL_GetWindowSize(_window.get(), &_actualWidth, &_actualHeight);

	// Setup the background Pixel memory ("* 4" is for the rgba values of each Pixel)
	_backgroundPixels = std::vector<byte>(_actualWidth * _actualHeight * 4, 255);
	// Create (or refresh) the background Texture
	_background = sdl_make_shared(SDL_CreateTexture(_renderer.get(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, _actualWidth, _actualHeight));
	if (_background != nullptr)
		graphics.UpdateBackground(_background, _actualWidth, _actualHeight);
	else
		throw FourierException("SDL Error on Texture creation: " + std::string(SDL_GetError()));
}

//
// Cleanup all the SDL2 and KiwiGUI Ressources that
// couldn't be managed by smart-pointers automatically
// 
Application::~Application() {
	if (_surface != nullptr)
		KW_ReleaseSurface(_driver.get(), _surface);
	if (_font != nullptr)
		KW_ReleaseFont(_driver.get(), _font);

	SDL_Quit();
}
