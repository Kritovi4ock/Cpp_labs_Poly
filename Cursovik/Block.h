#pragma once
#include <SDL.h>

class Block {
public:
    Block(int x, int y, int width, int height);
    ~Block();

    void render(SDL_Renderer* renderer);
    SDL_Rect getRect() const;
    bool isDestroyed() const;
    void destroy();

private:
    SDL_Rect rect;
    bool destroyed;
};