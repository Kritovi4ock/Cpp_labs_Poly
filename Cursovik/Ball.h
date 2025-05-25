#pragma once
#include <SDL.h>

class Ball {
public:
    Ball(int x, int y, int radius);
    ~Ball();

    void update();
    void render(SDL_Renderer* renderer);
    void setXVelocity(int xVel);
    void setYVelocity(int yVel);
    int getXVelocity();
    int getYVelocity();
    SDL_Rect getRect() const;
    int getRadius() const;

    void invertXVelocity();
    void invertYVelocity();

    void setPosition(int x, int y); // Добавили метод для установки позиции
    void setStuckToPaddle(bool stuck);
    bool isStuckToPaddle() const;

private:
    int x, y;
    int radius;
    int xVelocity;
    int yVelocity;
    SDL_Rect rect;
    bool stuckToPaddle; // Добавили флаг
};