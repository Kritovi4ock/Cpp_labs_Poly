#include "Bonus.h"
#include "Ball.h"
#include "Paddle.h"
#include <SDL.h>

Bonus::Bonus(int x, int y) : rect({ x, y, 20, 20 }), collected(false) {}

Bonus::~Bonus() {}

void Bonus::render(SDL_Renderer* renderer) {
    if (!collected) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

void Bonus::update() { rect.y += 2; }

SDL_Rect Bonus::getRect() const { return rect; }

bool Bonus::isCollected() const { return collected; }

WidenPaddleBonus::WidenPaddleBonus(int x, int y) : Bonus(x, y) {}

WidenPaddleBonus::~WidenPaddleBonus() {}

void WidenPaddleBonus::apply(Paddle& paddle, Ball& ball) { paddle.widen(20); }

void WidenPaddleBonus::render(SDL_Renderer* renderer) {
    if (!collected) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

ChangeBallSpeedBonus::ChangeBallSpeedBonus(int x, int y, int speedChange)
    : Bonus(x, y), speedChange(speedChange) {}

ChangeBallSpeedBonus::~ChangeBallSpeedBonus() {}

void ChangeBallSpeedBonus::apply(Paddle& paddle, Ball& ball) {
    int currentXVelocity = ball.getXVelocity();
    int currentYVelocity = ball.getYVelocity();

    if (currentXVelocity > 0) {
        ball.setXVelocity(currentXVelocity + speedChange);
    }
    else {
        ball.setXVelocity(currentXVelocity - speedChange);
    }

    if (currentYVelocity > 0) {
        ball.setYVelocity(currentYVelocity + speedChange);
    }
    else {
        ball.setYVelocity(currentYVelocity - speedChange);
    }
}

void ChangeBallSpeedBonus::render(SDL_Renderer* renderer) {
    if (!collected) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

StickyPaddleBonus::StickyPaddleBonus(int x, int y) : Bonus(x, y) {}

StickyPaddleBonus::~StickyPaddleBonus() {}

void StickyPaddleBonus::apply(Paddle& paddle, Ball& ball) {
    ball.setStuckToPaddle(true);
}

void StickyPaddleBonus::render(SDL_Renderer* renderer) {
    if (!collected) {
        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

BottomWallBonus::BottomWallBonus(int x, int y) : Bonus(x, y) {}

BottomWallBonus::~BottomWallBonus() {}

void BottomWallBonus::apply(Paddle& paddle, Ball& ball) {
    ball.invertYVelocity();
}

void BottomWallBonus::render(SDL_Renderer* renderer) {
    if (!collected) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

ChangeTrajectoryBonus::ChangeTrajectoryBonus(int x, int y) : Bonus(x, y) {}

ChangeTrajectoryBonus::~ChangeTrajectoryBonus() {}

void ChangeTrajectoryBonus::apply(Paddle& paddle, Ball& ball) {
    ball.startRandomTrajectoryChange();
}

void ChangeTrajectoryBonus::render(SDL_Renderer* renderer) {
    if (!collected) {
        SDL_SetRenderDrawColor(renderer, 50, 168, 82, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}