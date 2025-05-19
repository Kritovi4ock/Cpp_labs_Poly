#pragma once
#include "Bonus.h"
#include <memory>

class GameBoard {
private:
	std::vector<std::vector<Gem>> grid;
	int rows;
	int cols;
	int gemSize;
	int selectedRow;
	int selectedCol;
	bool hasSelection;
	std::vector<std::unique_ptr<Bonus>> activeBonuses;
	void initializeGrid();
	void swapGems(int row1, int col1, int row2, int col2);
	bool checkMatches();
	void removeMatches();

public:
	GameBoard(int rows = 8, int cols = 8, int gemSize = 60);
	void render(SDL_Renderer* renderer);
	void handleClick(int x, int y);
	void update();
	void dropGems();
	void refillEmpty();
	bool needsRefill();
	void printGrid() const;
	void tryCreateBonus(int row, int col, GemColor color);
	void processBonuses();
};