#include "Block.h"

Block::Block(int x, int y, int width, int height) : rect({ x, y, width, height }), destroyed(false) {}

Block::~Block() {}

void Block::render(SDL_Renderer* renderer) {
    if (!destroyed) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Белый
        SDL_RenderFillRect(renderer, &rect);
    }
}

SDL_Rect Block::getRect() const {
    return rect;
}

bool Block::isDestroyed() const {
    return destroyed;
}

void Block::destroy() {
    destroyed = true;
}