#include "Player.h"

Player::Player(char x, char y, double cellWidth, double cellHeight) : 
	m_cellWidth(cellWidth), m_cellHeight(cellHeight)
{
	// we really allocating 400 right here? alright
	m_snakeBodyParts = new SnakeBodyPart[20 * 20];
	m_snakeBodyParts[0] = SnakeBodyPart{ x, y, Direction::RIGHT };

	m_head = &m_snakeBodyParts[0];
	m_tail = m_head;
	m_length = 1;
}

Player::~Player() {
	delete[] m_snakeBodyParts;
}

void Player::setDirection(Direction direction) {
	m_head->direction = direction;
}

void Player::moveForward() {
	// I have to advance each snake part in the direction it is going
	// Furthermore, after moving, I need to update the direction to the direction of the part in front
	Direction prevDirection;
	for (unsigned int i = 0; i < m_length; i++) {
		SnakeBodyPart& part = m_snakeBodyParts[i];
		Direction direction = part.direction;
		char dx = 0, dy = 0;

		switch (direction) {
			case Direction::RIGHT:
				dx = 1;
				break;

			case Direction::DOWN:
				dy = 1;
				break;

			case Direction::LEFT:
				dx = -1;
				break;

			case Direction::UP:
				dy = -1;
				break;
		}

		// dx = m_direction != Direction::RIGHT ? m_direction != Direction::LEFT ? 0 : -1 : 1;
		// dy = m_direction != Direction::UP ? m_direction != Direction::DOWN ? 0 : 1 : -1;

		part.m_x += dx;
		part.m_y += dy;

		// change direction
		if (&part != m_head)
			part.direction = prevDirection;

		prevDirection = direction;
	}

	m_prevTailDirection = prevDirection;
}

void Player::render(SDL_Renderer& renderer) const {
	// save the previous draw color
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(&renderer, &r, &g, &b, &a);

	// draw the head a different color
	SDL_Rect rect = {
		(int) (m_head->m_x * m_cellWidth),
		(int) (m_head->m_y * m_cellHeight),
		(int) m_cellWidth,
		(int) m_cellHeight
	};
	SDL_SetRenderDrawColor(&renderer, 0x00, 0x88, 0x00, 0xFF);
	SDL_RenderFillRect(&renderer, &rect);

	// draw the rest of the body
	SDL_SetRenderDrawColor(&renderer, 0x00, 0xCD, 0x00, 0xFF);
	for (unsigned int i = 1; i < m_length; i++) {
		const SnakeBodyPart& part = m_snakeBodyParts[i];

		rect.x = (int) (part.m_x * m_cellWidth);
		rect.y = (int) (part.m_y * m_cellHeight);
		rect.w = (int) m_cellWidth;
		rect.h = (int) m_cellHeight;

		SDL_RenderFillRect(&renderer, &rect);
	}

	// restore the previous color
	SDL_SetRenderDrawColor(&renderer, r, g, b, a);
}

int Player::getHeadX() const {
	return m_head->m_x;
}

int Player::getHeadY() const {
	return m_head->m_y;
}

int Player::getTailX() const {
	return m_tail->m_x;
}

int Player::getTailY() const {
	return m_tail->m_y;
}

Player::SnakeBodyPart& Player::getHead() {
	return *m_head;
}

Player::SnakeBodyPart& Player::getTail() {
	return *m_tail;
}

Player::SnakeBodyPart& Player::getBodyPart(int index) {
	return m_snakeBodyParts[index];
}

unsigned int Player::getLength() const {
	return m_length;
}

void Player::grow() {
	unsigned char m_x = m_tail->m_x;
	unsigned char m_y = m_tail->m_y;
	Direction direction = m_prevTailDirection;

	char dx = 0, dy = 0;
	switch (direction) {
		case Direction::RIGHT:
			dx = -1;
			break;

		case Direction::DOWN:
			dy = -1;
			break;

		case Direction::LEFT:
			dx = 1;
			break;

		case Direction::UP:
			dy = 1;
			break;
	}

	m_snakeBodyParts[m_length++] = SnakeBodyPart{m_x + dx, m_y + dy, direction };
	m_tail = &m_snakeBodyParts[m_length - 1];
}


void Player::reset() {
	m_length = 1;
	m_head->m_x = 9;
	m_head->m_y = 9;
	m_head->direction = Direction::RIGHT;
	m_tail = m_head;
}
