#pragma once

#include "core/Scene.h"
#include "Food.h"

class Player;

class Game : public Scene {
public:
	Game(int numCells);
	~Game() override;
	void onInit(const Window& window) override;
	void onUpdate(double dt) override;
	void onRender(SDL_Renderer& renderer) override;
	void onInput(SDL_Event& event) override;
	void onDestroy() override;

	bool findRandomEmptySpot(int* x, int* y);
	void reset();

private:
	enum class GameState : unsigned char {
		PAUSED = 0,
		PLAYING,
		GAME_OVER
	};

private:
	bool* grid;
	Player* m_player;
	Food m_food;

	//Uint64 m_lastUpdateTime;
	Uint64 m_accumulatedTime;

	GameState m_state;

	int m_windowWidth;
	int m_windowHeight;
	int m_numCells;

};