#pragma once

#ifdef _WIN32
#include <SDL.h>
#undef main
#elif
#include <SDL2/SDL.h>
#endif

#include "KW_gui.h"
#include "KW_frame.h"
#include "KW_editbox.h"
#include "KW_label.h"
#include "KW_button.h"
#include "KW_renderdriver_sdl2.h"

#ifdef _WIN32
#define BASE_PATH "C:/Users/PeterUser/Documents/GitHub/Fourier/Fourier/resources/"
#elif
#define BASE_PATH "/Users/peter/Documents/github/C++/Fourier/libs/KiWi/resources/"
#endif

#define FPS 60

typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char byte;
