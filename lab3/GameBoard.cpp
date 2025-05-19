#include "GameBoard.h"
#include <SDL.h>
#include <algorithm>
#include <iostream>

GameBoard::GameBoard(int rows, int cols, int gemSize)
    : rows(rows), cols(cols), gemSize(gemSize), selectedRow(-1),
    selectedCol(-1), hasSelection(false) {
    initializeGrid();
}

void GameBoard::initializeGrid() {
    grid.resize(rows, std::vector<Gem>(cols));

    int offsetX = (800 - cols * gemSize) / 2;
    int offsetY = (600 - rows * gemSize) / 2;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int x = offsetX + j * gemSize;
            int y = offsetY + i * gemSize;
            grid[i][j] = Gem(x, y, gemSize - 2, i, j);
        }
    }

    while (checkMatches()) {
        removeMatches();
        refillEmpty();
    }
}

void GameBoard::render(SDL_Renderer* renderer) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j].render(renderer);
        }
    }
}

void GameBoard::handleClick(int x, int y) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j].contains(x, y)) {
                if (!hasSelection) {

                    selectedRow = i;
                    selectedCol = j;
                    grid[i][j].setSelected(true);
                    hasSelection = true;
                }
                else {

                    if ((abs(i - selectedRow) == 1 && j == selectedCol) ||
                        (abs(j - selectedCol) == 1 && i == selectedRow)) {

                        grid[selectedRow][selectedCol].setSelected(false);

                        swapGems(selectedRow, selectedCol, i, j);

                        if (!checkMatches()) {

                            swapGems(selectedRow, selectedCol, i, j);
                        }
                        else {
                            removeMatches();
                            refillEmpty();
                        }
                    }
                    else {
                        grid[selectedRow][selectedCol].setSelected(false);
                        selectedRow = i;
                        selectedCol = j;
                        grid[i][j].setSelected(true);
                    }
                    hasSelection = false;
                }
                return;
            }
        }
    }
}

void GameBoard::swapGems(int row1, int col1, int row2, int col2) {
    std::swap(grid[row1][col1], grid[row2][col2]);

    int tempX = grid[row1][col1].getPosition().x;
    int tempY = grid[row1][col1].getPosition().y;

    grid[row1][col1].setPosition(grid[row2][col2].getPosition().x,
        grid[row2][col2].getPosition().y);
    grid[row2][col2].setPosition(tempX, tempY);

    grid[row1][col1].setGridPosition(row1, col1);
    grid[row2][col2].setGridPosition(row2, col2);
}

bool GameBoard::checkMatches() {
    bool foundMatches = false;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols - 2; ++j) {
            if (grid[i][j].getColor() == grid[i][j + 1].getColor() &&
                grid[i][j].getColor() == grid[i][j + 2].getColor()) {
                foundMatches = true;
            }
        }
    }

    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows - 2; ++i) {
            if (grid[i][j].getColor() == grid[i + 1][j].getColor() &&
                grid[i][j].getColor() == grid[i + 2][j].getColor()) {
                foundMatches = true;
            }
        }
    }

    return foundMatches;
}

void GameBoard::removeMatches() {
    std::vector<std::pair<int, int>> toRemove;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols - 2; ++j) {
            if (grid[i][j].getColor() == grid[i][j + 1].getColor() &&
                grid[i][j].getColor() == grid[i][j + 2].getColor()) {
                toRemove.emplace_back(i, j);
                toRemove.emplace_back(i, j + 1);
                toRemove.emplace_back(i, j + 2);
            }
        }
    }

    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows - 2; ++i) {
            if (grid[i][j].getColor() == grid[i + 1][j].getColor() &&
                grid[i][j].getColor() == grid[i + 2][j].getColor()) {
                toRemove.emplace_back(i, j);
                toRemove.emplace_back(i + 1, j);
                toRemove.emplace_back(i + 2, j);
            }
        }
    }

    std::sort(toRemove.begin(), toRemove.end());
    toRemove.erase(std::unique(toRemove.begin(), toRemove.end()), toRemove.end());

    for (auto& pos : toRemove) {
        tryCreateBonus(pos.first, pos.second,
            grid[pos.first][pos.second].getColor());
        grid[pos.first][pos.second].setColor(GemColor::COUNT);
    }

    dropGems();

    refillEmpty();
    processBonuses();
}

void GameBoard::dropGems() {
    for (int j = 0; j < cols; ++j) {
        std::vector<Gem> aliveGems;
        for (int i = rows - 1; i >= 0; --i) {
            if (grid[i][j].getColor() != GemColor::COUNT) {
                aliveGems.push_back(grid[i][j]);
            }
        }

        for (int i = rows - 1, k = 0; i >= 0; --i) {
            if (k < aliveGems.size()) {
                grid[i][j] = aliveGems[k++];
                grid[i][j].setGridPosition(i, j);
                int x = grid[i][j].getPosition().x;
                int y = (600 - rows * gemSize) / 2 + i * gemSize;
                grid[i][j].setPosition(x, y);
            }
            else {
                grid[i][j].setColor(GemColor::COUNT);
            }
        }
    }
}

void GameBoard::refillEmpty() {
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows; ++i) {
            if (grid[i][j].getColor() == GemColor::COUNT) {
                grid[i][j].randomizeColor();
                int x = (800 - cols * gemSize) / 2 + j * gemSize;
                int y = (600 - rows * gemSize) / 2 + i * gemSize;
                grid[i][j].setPosition(x, y);
                grid[i][j].setGridPosition(i, j);
            }
        }
    }
}

bool GameBoard::needsRefill() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j].getColor() == GemColor::COUNT) {
                return true;
            }
        }
    }
    return false;
}

void GameBoard::update() {

    static int frames = 0;
    if (++frames % 10 == 0) {
        for (auto& row : grid) {
            for (auto& gem : row) {
                if (gem.getColor() == GemColor::COUNT) {
                    gem.randomizeColor();
                }
            }
        }
    }
}

void GameBoard::printGrid() const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << static_cast<int>(grid[i][j].getColor()) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void GameBoard::tryCreateBonus(int row, int col, GemColor color) {
    if (rand() % 100 < 15) {
        BonusType type = (rand() % 2 == 0) ? BonusType::RECOLOR : BonusType::BOMB;

        std::vector<std::pair<int, int>> candidates;
        for (int i = std::max(0, row - 3); i <= std::min(rows - 1, row + 3); ++i) {
            for (int j = std::max(0, col - 3); j <= std::min(cols - 1, col + 3);
                ++j) {
                if (i != row || j != col) {
                    candidates.emplace_back(i, j);
                }
            }
        }

        if (!candidates.empty()) {
            auto& pos = candidates[rand() % candidates.size()];
            int x = grid[pos.first][pos.second].getPosition().x;
            int y = grid[pos.first][pos.second].getPosition().y;

            auto bonus = std::make_unique<Bonus>(x, y, gemSize - 2, type, color);
            grid[pos.first][pos.second] = *bonus;
            activeBonuses.push_back(std::move(bonus));
        }
    }
}

void GameBoard::processBonuses() {
    auto modifier = [this](int row, int col, GemColor color) {
        grid[row][col].setColor(color);
        };

    for (auto& bonus : activeBonuses) {
        int row, col;
        bonus->getGridPosition(row, col);

        if (grid[row][col].getColor() == GemColor::COUNT) {
            bonus->applyEffect(row, col, rows, cols, modifier);
        }
    }
    activeBonuses.clear();
}