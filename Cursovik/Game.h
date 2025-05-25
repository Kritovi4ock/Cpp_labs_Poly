#pragma once
#include <vector>
#include "Paddle.h"
#include "Ball.h"
#include "Block.h"

class Game {
public:
    Game(const char* title, int width, int height);
    ~Game();

    bool init();
    void run();
    void close();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    int windowWidth;
    int windowHeight;

    Paddle* paddle;
    Ball* ball;
    std::vector<Block*> blocks;

    void handleEvents();
    void update();
    void render();
    void createBlocks();
    void checkCollisions();

    void handleBallPaddle(const Uint8* keyboardState); // Добавили управление шариком
    const double fixedTimeStep = 1.0 / 60.0; // 30 обновлений в секунду (пример)
    double accumulator = 0.0;
};
