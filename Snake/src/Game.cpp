#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "Game.h"
#include "Player.h"
#include "core/Window.h"


Game::Game(int numCells) : m_numCells(numCells) {}
Game::~Game() {}

void Game::onInit(const Window& window) {
	m_windowWidth = window.getWidth();
	m_windowHeight = window.getHeight();

	double cellWidth = m_windowWidth / (double) m_numCells;
	double cellHeight = m_windowHeight / (double) m_numCells;
	
	int startX = m_numCells / 2;
	int startY = m_numCells / 2;

	m_player = new Player(startX, startY, cellWidth, cellHeight);
	grid = new bool[m_numCells * m_numCells];

	// initialize this grid (might not even need this block of code)
	for (int row = 0; row < m_numCells; row++)
		for (int column = 0; column < m_numCells; column++)
			grid[row * m_numCells + column] = false;

	grid[startY * m_numCells + startX] = true;
	
	//m_lastUpdateTime = 0;
	m_accumulatedTime = 0;

	m_state = GameState::PLAYING;

	srand(static_cast<unsigned int>(time(nullptr)));

	// gotta do a more graceful handle
	int x, y;
	if (!findRandomEmptySpot(&x, &y))
		exit(1);

	m_food.setCellWidth(cellWidth);
	m_food.setCellHeight(cellHeight);
	m_food.setPosition(x, y);
}

void Game::onInput(SDL_Event& event) {
	if (m_state == GameState::GAME_OVER) {
		// press any key to reset
		if (event.type == SDL_KEYDOWN)
			reset();
	}
	// Handle the escape key input always
	else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
		m_state = m_state == GameState::PAUSED ? GameState::PLAYING : GameState::PAUSED;
	
	if (m_state == GameState::PLAYING) {
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
				case SDLK_w:
				case SDLK_UP:
					m_player->setDirection(Player::Direction::UP);
					break;

				case SDLK_a:
				case SDLK_LEFT:
					m_player->setDirection(Player::Direction::LEFT);
					break;

				case SDLK_s:
				case SDLK_DOWN:
					m_player->setDirection(Player::Direction::DOWN);
					break;

				case SDLK_d:
				case SDLK_RIGHT:
					m_player->setDirection(Player::Direction::RIGHT);
					break;
			}
		}
	}
}


void Game::onUpdate(double dt) {
	// don't reeally wanna update anything if we're not playing
	if (m_state != GameState::PLAYING)
		return;

	m_accumulatedTime += (Uint64) (dt * 1000);
	// I want to simulate a "step by step" feeling here
	Uint64 now = SDL_GetTicks64();
	if (m_accumulatedTime >= 100) {
		m_accumulatedTime = 0;
		
		// update the snake's position
		// handle wrapping here too
		int prevTailX = m_player->getTailX();
		int prevTailY = m_player->getTailY();

		m_player->moveForward();

		for (unsigned int i = 0; i < m_player->getLength(); i++) {
			// weird 'auto' trick that lets me access private struct (bad code design by me?)
			// could just make Game a friend of Player
			auto& part = m_player->getBodyPart(i);

			if (part.m_x >= m_numCells)
				part.m_x = 0;
			else if (part.m_x < 0)
				part.m_x = m_numCells - 1;

			if (part.m_y >= m_numCells)
				part.m_y = 0;
			else if (part.m_y < 0)
				part.m_y = m_numCells - 1;
		}

		int headX = m_player->getHeadX();
		int headY = m_player->getHeadY();

		// update the grid
		grid[prevTailY * m_numCells + prevTailX] = false;

		// check if we hit ourself
		if (grid[headY * m_numCells + headX]) {
			std::cout << "Player hit themself!" << std::endl;
			m_state = GameState::GAME_OVER;
			return;
		}

		grid[headY * m_numCells + headX] = true;

		// snake eats the food
		if (headX == m_food.getX() && headY == m_food.getY()) {
			m_player->grow();
			grid[m_player->getTailY() * m_numCells + m_player->getTailX()] = true;

			// find a new spot for the food
			int x, y;
			if (!findRandomEmptySpot(&x, &y)) {
				std::cout << "Player won!" << std::endl;
				m_state = GameState::GAME_OVER;
			}
			else
				m_food.setPosition(x, y);
		}
	}
}

void Game::onRender(SDL_Renderer& renderer) {
	SDL_SetRenderDrawColor(&renderer, 0x2A, 0x2A, 0x2A, 0xFF);
	SDL_RenderClear(&renderer);

	m_food.render(renderer);
	m_player->render(renderer);


	if (m_state == GameState::PAUSED) {
		SDL_Rect rect = {	
			m_windowWidth / 2 - m_windowWidth / 20,
			m_windowHeight / 2 - m_windowHeight / 10,  
			m_windowWidth / 20, 
			m_windowHeight / 5
		};

		SDL_SetRenderDrawColor(&renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(&renderer, &rect);

		rect.x += m_windowWidth / 10;
		SDL_RenderFillRect(&renderer, &rect);
	}

	// add a small delay to reduce cpu load
	SDL_Delay(33);
}

void Game::onDestroy() {
	delete m_player;
	delete[] grid;
}

bool Game::findRandomEmptySpot(int* x, int* y) {
	int* possibleSlots = new int[m_numCells * m_numCells];
	int count = 0;

	for (int row = 0; row < m_numCells; row++) {
		for (int column = 0; column < m_numCells; column++) {
			if (!grid[row * m_numCells + column])
				possibleSlots[count++] = row * m_numCells + column;
		}
	}

	if (!count) {
		delete[] possibleSlots;
		return false;
	}

	int randIndex = rand() % count;
	int slot = possibleSlots[randIndex];

	*x = slot % m_numCells;
	*y = slot / m_numCells;

	delete[] possibleSlots;
	return true;
}

void Game::reset() {
	// reset player
	int startX = m_numCells / 2;
	int startY = m_numCells / 2;

	m_player->reset();
	m_player->getHead().m_x = startX;
	m_player->getHead().m_y = startY;

	// reset grid
	for (int row = 0; row < m_numCells; row++)
		for (int column = 0; column < m_numCells; column++)
			grid[row * m_numCells + column] = false;

	grid[startY * m_numCells + startX] = true;

	// reset food
	int x, y;
	if (!findRandomEmptySpot(&x, &y))
		exit(1);

	m_food.setPosition(x, y);
	m_state = GameState::PAUSED;
}