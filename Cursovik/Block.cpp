#include "Block.h"
#include "Bonus.h"
Block::Block(int x, int y, int width, int height)
    : rect({ x, y, width, height }), destroyed(false) {}

Block::~Block() {}

void Block::render(SDL_Renderer* renderer) {
    if (!destroyed) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

const SDL_Rect& Block::getRect() const { return rect; }

bool Block::isDestroyed() const { return destroyed; }

void Block::destroy() { destroyed = true; }

void Block::takeDamage() { destroy(); }

SpecialBlock::SpecialBlock(int x, int y, int width, int height)
    : Block(x, y, width, height) {}

SpecialBlock::~SpecialBlock() {}

void SpecialBlock::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderFillRect(renderer, &rect);
}

bool SpecialBlock::isDestroyed() const { return false; }

void SpecialBlock::destroy() {}

SpeedUpBlock::SpeedUpBlock(int x, int y, int width, int height,
    int speedIncrease)
    : Block(x, y, width, height), speedIncrease(speedIncrease) {}

SpeedUpBlock::~SpeedUpBlock() {}

void SpeedUpBlock::render(SDL_Renderer* renderer) {
    if (!isDestroyed()) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

void SpeedUpBlock::onCollision(Ball& ball) {
    int currentXVelocity = ball.getXVelocity();
    int currentYVelocity = ball.getYVelocity();

    if (!isDestroyed()) {
        if (currentXVelocity > 0) {
            ball.setXVelocity(currentXVelocity + speedIncrease);
        }
        else {
            ball.setXVelocity(currentXVelocity - speedIncrease);
        }

        if (currentYVelocity > 0) {
            ball.setYVelocity(currentYVelocity + speedIncrease);
        }
        else {
            ball.setYVelocity(currentYVelocity - speedIncrease);
        }
    }

    destroy();
}

bool SpeedUpBlock::isDestroyed() const { return Block::isDestroyed(); }

void SpeedUpBlock::destroy() { Block::destroy(); }

HealthBlock::HealthBlock(int x, int y, int width, int height, int initialHealth)
    : Block(x, y, width, height), health(initialHealth),
    initialHealth(initialHealth) {}

HealthBlock::~HealthBlock() {}

void HealthBlock::render(SDL_Renderer* renderer) {
    if (!isDestroyed()) {
        Uint8 colorIntensity =
            static_cast<Uint8>((static_cast<float>(health) / initialHealth) * 255);
        SDL_SetRenderDrawColor(renderer, 255, 255 - colorIntensity,
            255 - colorIntensity, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

void HealthBlock::takeDamage() {
    health--;
    if (health <= 0) {
        destroy();
    }
}

bool HealthBlock::isDestroyed() const { return health <= 0; }

BonusBlock::BonusBlock(int x, int y, int width, int height,
    std::unique_ptr<Bonus> bonus)
    : Block(x, y, width, height), bonus(std::move(bonus)) {}

BonusBlock::~BonusBlock() {}
void BonusBlock::render(SDL_Renderer* renderer) {
    if (!isDestroyed()) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}
void BonusBlock::destroy() { Block::destroy(); }

bool BonusBlock::isDestroyed() const { return Block::isDestroyed(); }

Bonus* BonusBlock::getBonus() const { return bonus.get(); }