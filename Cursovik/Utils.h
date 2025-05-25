#pragma once
#include <SDL.h>

namespace Utils {
    // Функция для проверки столкновения двух прямоугольников
    bool checkCollision(SDL_Rect a, SDL_Rect b);
}