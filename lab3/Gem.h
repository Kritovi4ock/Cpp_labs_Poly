#pragma once
#include <SDL.h>
#include <cstdlib>

enum class GemColor { RED, GREEN, BLUE, YELLOW, PURPLE, ORANGE, COUNT };

class Gem {
private:
	SDL_Rect rect;
	int row, col;
	bool selected;

public:
	GemColor color;
	Gem(int x = 0, int y = 0, int size = 40, int gridRow = 0, int gridCol = 0);

	void render(SDL_Renderer* renderer);

	void setPosition(int x, int y);

	void setGridPosition(int gridRow, int gridCol);

	void getGridPosition(int& gridRow, int& gridCol) const;

	bool contains(int x, int y) const;

	void setSelected(bool isSelected);

	GemColor getColor() const;

	void randomizeColor();

	SDL_Point getPosition() const;

	void setColor(GemColor newColor);
};