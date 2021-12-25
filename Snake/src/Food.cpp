#include "Food.h"

void Food::setPosition(int x, int y) {
	m_x = x;
	m_y = y;
}

int Food::getX() const {
	return m_x;
}

int Food::getY() const {
	return m_y;
}

void Food::setCellWidth(double width) {
	m_cellWidth = width;
}

void Food::setCellHeight(double height) {
	m_cellHeight = height;
}

void Food::render(SDL_Renderer& renderer) const {
	// save the previous draw color
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(&renderer, &r, &g, &b, &a);

	double width = m_cellWidth / 2;
	double height = m_cellHeight / 2;

	SDL_Rect rect = {
		(int) (m_x * m_cellWidth + m_cellWidth / 2 - width / 2), // x
		(int) (m_y * m_cellHeight + m_cellHeight / 2 - height / 2), // y
		(int) width,
		(int) height
	};

	SDL_SetRenderDrawColor(&renderer, 0xBD, 0x30, 0x39, 0xFF);
	SDL_RenderFillRect(&renderer, &rect);

	// restore the previous color
	SDL_SetRenderDrawColor(&renderer, r, g, b, a);
}