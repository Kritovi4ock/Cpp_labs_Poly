#pragma once
#include "Gem.h"
#include <functional>
#include <utility>
#include <vector>

enum class BonusType { RECOLOR, BOMB, NONE };

class Bonus : public Gem {
private:
    BonusType type;
    GemColor originalColor;

public:
    Bonus(int x, int y, int size, BonusType bonusType, GemColor color)
        : Gem(x, y, size), type(bonusType), originalColor(color) {
        setVisuals();
    }

    BonusType getType() const { return type; }
    GemColor getOriginalColor() const { return originalColor; }

    using GridModifier = std::function<void(int, int, GemColor)>;
    void applyEffect(int targetRow, int targetCol, int rows, int cols,
        GridModifier modifier);

private:
    void setVisuals() {
        switch (type) {
        case BonusType::RECOLOR:
            color = GemColor::PURPLE;
            break;
        case BonusType::BOMB:
            color = GemColor::ORANGE;
            break;
        default:
            break;
        }
    }
};