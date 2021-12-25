#pragma once


#include "SDL.h"
#include <string>

class Scene;

class Window {

public:
	Window(int width, int height, std::string& title);
	~Window();

	bool init();
	void run();
	void setScene(Scene* scene);

	int getWidth() const;
	int getHeight() const;
private:
	int m_Width;
	int m_Height;
	std::string m_Title;

	SDL_Window* m_Window = nullptr;
	SDL_Renderer* m_Renderer = nullptr;
	Scene* m_Scene = nullptr;
};