#include "../header/Application.hpp"

int main(int argc, const char* argv[]) {
	// Setup the actual App and start the main loop
	Fourier::Application app("Fourier", 1280, 720, BASE_PATH);
	return (app.InitApplication()) ? app.Run() : -1;
}
