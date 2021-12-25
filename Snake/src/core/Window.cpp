#include "Window.h"
#include "Scene.h"

#include <iostream>

Window::Window(int width, int height, std::string& title) :
	m_Width(width), m_Height(height), m_Title(title), m_Window(nullptr),
	m_Renderer(nullptr), m_Scene(nullptr) {

}

Window::~Window() {
	if (m_Scene)
		m_Scene->onDestroy();

	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);

	SDL_Quit();
}

bool Window::init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		return false;
	}

	if (SDL_CreateWindowAndRenderer(m_Width, m_Height, 0, &m_Window, &m_Renderer)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
		return false;
	}

	SDL_SetWindowTitle(m_Window, m_Title.c_str());

	return true;
}

void Window::run() {
	if (!m_Scene)
		return;

	bool shouldRun = true;
	Uint64 lastRunTime = 0;
	SDL_Event event;

	while (true) {
		// input
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				shouldRun = false;
				break;
			}

			m_Scene->onInput(event);
		}

		if (!shouldRun)
			break;

		// updating
		Uint64 now = SDL_GetTicks64();
		double dt = (now - lastRunTime) / 1000.0;
		lastRunTime = now;

		m_Scene->onUpdate(dt);

		// rendering
		m_Scene->onRender(*m_Renderer);
		SDL_RenderPresent(m_Renderer);
	}
}

void Window::setScene(Scene* scene) {
	Scene* currentScene = m_Scene;
	if (currentScene)
		currentScene->onDestroy();

	m_Scene = scene;
	
	if (m_Scene)
		m_Scene->onInit(*this);
}

int Window::getWidth() const {
	return m_Width;
}

int Window::getHeight() const {
	return m_Height;
}