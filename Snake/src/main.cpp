// To compile with gcc:  (tested on Ubuntu 14.04 64bit):
//	 g++ sdl2_opengl.cpp -lSDL2 -lGL
// To compile with msvc: (tested on Windows 7 64bit)
//   cl sdl2_opengl.cpp /I C:\sdl2path\include /link C:\path\SDL2.lib C:\path\SDL2main.lib /SUBSYSTEM:CONSOLE /NODEFAULTLIB:libcmtd.lib opengl32.lib

#define SDL_MAIN_HANDLED
#include <iostream>

#include "core/Window.h"
#include "Game.h"
#include <string>

int main(int ArgCount, char **Args)
{
	std::string title = "Snake";
	Window* w = new Window(800, 800, title);
	if (!w->init()) {
		std::cout << "Error initializing window!" << std::endl;
		return 1;
	}

	Scene* scene = new Game(16);
	w->setScene(scene);
	w->run();

	delete w;
	delete scene;

	return 0;
}
