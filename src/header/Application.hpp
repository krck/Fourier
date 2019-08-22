#pragma once

#include "Exception.hpp"
#include "Graphics.hpp"
#include "Transformations.hpp"
#include <string>
#include <memory>
#include <iostream>

namespace Fourier {

	//
	// Application
	//
	// Managing all SDL2 and KiwiGUI Ressources
	// Creates a Window and runs the main Event loop
	//
	class Application {
	private:
		const std::string _appName;
		const std::string _resourcePath;
		const uint _windowWidth, _windowHeight;
		int _actualWidth, _actualHeight;
		std::vector<byte> _backgroundPixels;
		std::shared_ptr<SDL_Window> _window;
		std::shared_ptr<SDL_Renderer> _renderer;
		std::shared_ptr<SDL_Texture> _background;
		std::shared_ptr<KW_RenderDriver> _driver;
		std::shared_ptr<KW_GUI> _gui;
		KW_Surface* _surface;
		KW_Font* _font;

	public:
		Application(std::string&& appName, uint&& windowWidth, uint&& windowHeight, std::string&& resourcePath);
		Application(const std::string& appName, const uint& windowWidth, const uint& windowHeight, const std::string& resourcePath);
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		bool InitApplication();
		int Run();

		~Application();

	private:
		void SetupWindow();
		void SetupSDL();
		void SetupKiwiGUI();
		void OnWindowResize(Graphics& graphics);
	};

	// Custom Deleters called on shared_ptr destruction to cleanup SDL resources
	static void SDL_Deleter(SDL_Window* p) { if (p != nullptr) SDL_DestroyWindow(p); }
	static void SDL_Deleter(SDL_Renderer* p) { if (p != nullptr) SDL_DestroyRenderer(p); }
	static void SDL_Deleter(SDL_Texture* p) { if (p != nullptr) SDL_DestroyTexture(p); }
	// Custom Deleters called on shared_ptr destruction to cleanup KiWi resources
	static void KiWi_Deleter(KW_RenderDriver* p) { if (p != nullptr) KW_ReleaseRenderDriver(p); }
	static void KiWi_Deleter(KW_GUI* p) { if (p != nullptr) KW_Quit(p); }

	// SDL and KiwiGUI Shared Pointer creation functions globally in the Fourier namespace
	template <typename T>
	std::shared_ptr<T> sdl_make_shared(T* t) { return std::shared_ptr<T>(t, [](T* t) { SDL_Deleter(t); }); }
	template <typename T>
	std::shared_ptr<T> kiwi_make_shared(T* t) { return std::shared_ptr<T>(t, [](T* t) { KiWi_Deleter(t); }); }

}