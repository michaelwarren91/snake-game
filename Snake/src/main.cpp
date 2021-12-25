#define SDL_MAIN_HANDLED
#include <iostream>

#include "core/Window.h"
#include "Game.h"
#include <string>

int main(int argc, char** argv)
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
