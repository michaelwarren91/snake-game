#pragma once

#include <SDL.h>

class Window;

class Scene {
public:
	virtual ~Scene() = 0 {}
	virtual void onInit(const Window& window) = 0;
	virtual void onUpdate(double dt) = 0;
	virtual void onRender(SDL_Renderer& renderer) = 0;
	virtual void onInput(SDL_Event& event) = 0;
	virtual void onDestroy() = 0;
private:

};