#pragma once

#include "SDL.h"

class Food {
public:
	void setPosition(int x, int y);
	int getX() const;
	int getY() const;

	void setCellWidth(double width);
	void setCellHeight(double height);
	
	void render(SDL_Renderer& renderer) const;

private:
	int m_x, m_y;
	double m_cellWidth, m_cellHeight;
};