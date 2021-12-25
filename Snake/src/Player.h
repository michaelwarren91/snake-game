#pragma once

#include <SDL.h>

class Player {
public:
	enum class Direction : unsigned char {
		UP = 0,
		LEFT,
		DOWN,
		RIGHT
	};

private:
	struct SnakeBodyPart {
		char m_x, m_y;
		Direction direction;
	};

private:
	double m_cellWidth;
	double m_cellHeight;

	unsigned int m_length;
	SnakeBodyPart* m_snakeBodyParts;
	SnakeBodyPart* m_head, *m_tail;
	Direction m_prevTailDirection;

public:
	Player(char x, char y, double cellWidth, double cellHeight);
	~Player();

	void setDirection(Direction direction);
	void moveForward();

	int getHeadX() const;
	int getHeadY() const;
	int getTailX() const;
	int getTailY() const;

	SnakeBodyPart& getHead();
	SnakeBodyPart& getTail();
	SnakeBodyPart& getBodyPart(int index);
	unsigned int getLength() const;

	void grow();
	void reset();

	void render(SDL_Renderer& renderer) const;
};