#include "Paddle.h"
#include <algorithm>

Paddle::Paddle(int x, int y, int width, int height, int windowWidth) : rect({ x, y, width, height }), velocity(2), windowWidth(windowWidth) {}

Paddle::~Paddle() {}

void Paddle::handleInput(const Uint8* keyboardState) {
    if (keyboardState[SDL_SCANCODE_LEFT]) {
        moveLeft();
    }
    if (keyboardState[SDL_SCANCODE_RIGHT]) {
        moveRight();
    }
}


void Paddle::moveLeft() {
    rect.x -= velocity;
    if (rect.x < 0) {
        rect.x = 0;
    }
}

void Paddle::moveRight() {
    rect.x += velocity;
    if (rect.x > windowWidth - rect.w) {
        rect.x = windowWidth - rect.w;
    }
}


void Paddle::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Белый
    SDL_RenderFillRect(renderer, &rect);
}

SDL_Rect Paddle::getRect() const {
    return rect;
}

void Paddle::shrink(int amount) {
    rect.w -= amount; // Уменьшаем ширину
    rect.x += amount / 2;

    // Ограничиваем минимальную ширину
    rect.w = std::max(rect.w, 20);

    // Проверяем, чтобы ракетка не вышла за границы окна
    if (rect.x < 0) {
        rect.x = 0;
    }
    else if (rect.x + rect.w > windowWidth) {
        rect.x = windowWidth - rect.w;
    }
}