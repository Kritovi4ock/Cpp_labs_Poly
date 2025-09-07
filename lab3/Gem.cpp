#include "Gem.h"
Gem::Gem(int x, int y, int size, int gridRow, int gridCol)
    : row(gridRow), col(gridCol), selected(false) {
    rect.x = x;
    rect.y = y;
    rect.w = size;
    rect.h = size;

    color = static_cast<GemColor>(rand() % static_cast<int>(GemColor::COUNT));
}

void Gem::render(SDL_Renderer* renderer) {
    if (color == GemColor::COUNT)
        return;
    switch (color) {
    case GemColor::RED:
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        break;
    case GemColor::GREEN:
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        break;
    case GemColor::BLUE:
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        break;
    case GemColor::YELLOW:
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        break;
    case GemColor::PURPLE:
        SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);
        break;
    case GemColor::ORANGE:
        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
        break;
    default:
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }

    SDL_RenderFillRect(renderer, &rect);

    if (selected) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);
    }
}

void Gem::setPosition(int x, int y) {
    rect.x = x;
    rect.y = y;
}

void Gem::setGridPosition(int gridRow, int gridCol) {
    row = gridRow;
    col = gridCol;
}

void Gem::getGridPosition(int& gridRow, int& gridCol) const {
    gridRow = row;
    gridCol = col;
}

bool Gem::contains(int x, int y) const {
    return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y &&
        y <= rect.y + rect.h);
}

void Gem::setSelected(bool isSelected) { selected = isSelected; }

GemColor Gem::getColor() const { return color; }

void Gem::randomizeColor() {
    color = static_cast<GemColor>(rand() % static_cast<int>(GemColor::COUNT));
}

SDL_Point Gem::getPosition() const { return { rect.x, rect.y }; }

void Gem::setColor(GemColor newColor) { color = newColor; }