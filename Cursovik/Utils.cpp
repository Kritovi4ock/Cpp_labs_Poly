#include "Utils.h"

namespace Utils {
    bool checkCollision(SDL_Rect a, SDL_Rect b) {
        // Координаты
        int leftA = a.x;
        int rightA = a.x + a.w;
        int topA = a.y;
        int bottomA = a.y + a.h;

        int leftB = b.x;
        int rightB = b.x + b.w;
        int topB = b.y;
        int bottomB = b.y + b.h;

        // Если одна из сторон находится за другой
        if (bottomA <= topB) {
            return false;
        }

        if (topA >= bottomB) {
            return false;
        }

        if (rightA <= leftB) {
            return false;
        }

        if (leftA >= rightB) {
            return false;
        }

        // Столкновение обнаружено
        return true;
    }
}