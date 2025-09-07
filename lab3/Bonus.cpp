#include "Bonus.h"
#include <algorithm>
#include <random>

void RecolorBonus::applyEffect(int targetRow, int targetCol,
    int rows, int cols,
    GridModifier modifier) {
    modifier(targetRow, targetCol, originalColor);

    std::vector<std::pair<int, int>> candidates;
    for (int i = std::max(0, targetRow - 3);
        i <= std::min(rows - 1, targetRow + 3); ++i) {
        for (int j = std::max(0, targetCol - 3);
            j <= std::min(cols - 1, targetCol + 3); ++j) {
            if ((i != targetRow || j != targetCol) &&
                !(abs(i - targetRow) <= 1 && abs(j - targetCol) <= 1)) {
                candidates.emplace_back(i, j);
            }
        }
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(candidates.begin(), candidates.end(), g);

    for (int k = 0; k < std::min(2, (int)candidates.size()); ++k) {
        modifier(candidates[k].first, candidates[k].second, originalColor);
    }
}

void BombBonus::applyEffect(int targetRow, int targetCol,
    int rows, int cols,
    GridModifier modifier) {
    modifier(targetRow, targetCol, GemColor::COUNT);

    std::vector<std::pair<int, int>> allGems;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i != targetRow || j != targetCol) {
                allGems.emplace_back(i, j);
            }
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(allGems.begin(), allGems.end(), g);

    for (int k = 0; k < std::min(4, (int)allGems.size()); ++k) {
        modifier(allGems[k].first, allGems[k].second, GemColor::COUNT);
    }
}
