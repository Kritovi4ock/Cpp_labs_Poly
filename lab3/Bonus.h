#pragma once
#include "Gem.h"
#include <functional>
#include <utility>
#include <vector>

class Bonus : public Gem {
protected:
    GemColor originalColor;

public:
    using GridModifier = std::function<void(int, int, GemColor)>;

    Bonus(int x, int y, int size, GemColor color)
        : Gem(x, y, size), originalColor(color) {}

    virtual ~Bonus() = default;
    virtual void applyEffect(int targetRow, int targetCol,
        int rows, int cols,
        GridModifier modifier) = 0;

    GemColor getOriginalColor() const { return originalColor; }
};

class RecolorBonus : public Bonus {
public:
    RecolorBonus(int x, int y, int size, GemColor color)
        : Bonus(x, y, size, color) {
        color = GemColor::PURPLE;
    }

    void applyEffect(int targetRow, int targetCol,
        int rows, int cols,
        GridModifier modifier) override;
};

class BombBonus : public Bonus {
public:
    BombBonus(int x, int y, int size, GemColor color)
        : Bonus(x, y, size, color) {
        color = GemColor::ORANGE;
    }

    void applyEffect(int targetRow, int targetCol,
        int rows, int cols,
        GridModifier modifier) override;
};