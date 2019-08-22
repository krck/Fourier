Fourier
=======

... 

About
-----

...

Dependencies
------------

- [SDL2](https://www.libsdl.org/download-2.0.php) : Simple DirectMedia Layer is a cross-platform graphics library
- [SDL_image](https://www.libsdl.org/projects/SDL_image/) : Image loading library used with the SDL
- [SDL_tff](https://www.libsdl.org/projects/SDL_ttf/) :  TrueType font rendering library used with the SDL
- [KiWi](https://github.com/mobius3/KiWi) : Widget-based GUI library

ToDo
----

- [X] Setup a basic 2D Drawing Application
- [X] Add Algorithms for Circle, Line, Wave, ... drawing
- [X] Add GUI Framework
- [X] Configure different Platforms (Win + VS / OSX + Xcode) 
- [ ] Implement Fourier Transform

Setup Visual Studio
-------------------

- Get all Dependencies (SDL2, image and tff come with downloadable .dlls, KiWi must be compiled)
- Put Lib .dlls and /includes in one place
- Open the VS Project file and open the Project Settings
- Set: C/C++ -> General -> AdditionalIncludeDirectories -> Path to "...\SDL2\include" (and to all other include folders)
- Set: Linker -> General -> AdditionalLibraryDirectories -> Path to "...\SDL2\lib\x64" (and to all other .dll folders)
- Set: Linker -> Input -> AdditionalDependencies -> Add "SDL2.lib" and "SDL2main.lib" (and all other Library files)
- And copy the SDL2.dll (and all other libs) into the Debug dir, where the .exe is

Other Settings
--------------

- Set: C/C++ -> General -> Multi Process Compiliation -> YES
- Set: C/C++ -> Language -> C++ Language Standard -> 17 or higher
