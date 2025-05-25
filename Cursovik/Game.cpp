#include "Game.h"
#include "Utils.h"
#include <iostream>

Game::Game(const char* title, int width, int height) : window(nullptr), renderer(nullptr), running(false), windowWidth(width), windowHeight(height), paddle(nullptr), ball(nullptr) {}

Game::~Game() {
    if (paddle) delete paddle;
    if (ball) delete ball;
    for (Block* block : blocks) {
        delete block;
    }
    blocks.clear();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Arkanoid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }

    paddle = new Paddle(windowWidth / 2 - 50, windowHeight - 50, 100, 20, windowWidth);
    ball = new Ball(windowWidth / 2, windowHeight - 50 - 10, 10); // Начальное положение над ракеткой
    createBlocks();

    running = true;
    return true;
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        double frameTime = (currentTime - lastTime) / 1000.0; // Время кадра в секундах
        lastTime = currentTime;

        accumulator += frameTime;

        handleEvents(); // Обрабатываем события как можно чаще

        while (accumulator >= fixedTimeStep) {
            update(); // Обновляем игру с фиксированным шагом
            accumulator -= fixedTimeStep;
        }

        render();
    }
}

void Game::close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }

    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
    paddle->handleInput(keyboardState);

    handleBallPaddle(keyboardState);
}

void Game::update() {
    if (ball->isStuckToPaddle()) {
        ball->setPosition(paddle->getRect().x + paddle->getRect().w / 2, paddle->getRect().y - ball->getRadius()); // Обновляем позицию на ракетке
    }

    ball->update();
    checkCollisions();

    if (ball->getRect().x <= 0 || ball->getRect().x + 2 * ball->getRadius() >= windowWidth) {
        ball->invertXVelocity();
    }
    if (ball->getRect().y <= 0) {
        ball->invertYVelocity();
    }

    if (ball->getRect().y + 2 * ball->getRadius() >= windowHeight) {
        paddle->shrink(10);
        ball->setStuckToPaddle(true); // Возвращаем на ракетку
        ball->setPosition(paddle->getRect().x + paddle->getRect().w / 2, paddle->getRect().y - ball->getRadius());
        ball->setXVelocity(0);
        ball->setYVelocity(0);
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    paddle->render(renderer);
    ball->render(renderer);

    for (Block* block : blocks) {
        block->render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::createBlocks() {
    int blockWidth = 50;
    int blockHeight = 20;
    int padding = 5;

    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 14; ++col) {
            int x = col * (blockWidth + padding) + padding;
            int y = row * (blockHeight + padding) + padding;
            blocks.push_back(new Block(x, y, blockWidth, blockHeight));
        }
    }
}

void Game::checkCollisions() {
    if (Utils::checkCollision(ball->getRect(), paddle->getRect()) && !ball->isStuckToPaddle()) {
        ball->invertYVelocity();
    }

    for (size_t i = 0; i < blocks.size(); ++i) {
        if (!blocks[i]->isDestroyed()) {
            SDL_Rect blockRect = blocks[i]->getRect();
            SDL_Rect ballRect = ball->getRect();

            if (Utils::checkCollision(ballRect, blockRect)) {
                blocks[i]->destroy();

                // Определяем сторону столкновения
                int topCollision = ballRect.y + ballRect.h - blockRect.y;
                int bottomCollision = blockRect.y + blockRect.h - ballRect.y;
                int leftCollision = ballRect.x + ballRect.w - blockRect.x;
                int rightCollision = blockRect.x + blockRect.w - ballRect.x;

                // Находим наименьшее "проникновение" - это и будет сторона столкновения
                if (topCollision < bottomCollision && topCollision < leftCollision && topCollision < rightCollision) {
                    // Столкновение сверху
                    ball->invertYVelocity();
                }
                else if (bottomCollision < topCollision && bottomCollision < leftCollision && bottomCollision < rightCollision) {
                    // Столкновение снизу
                    ball->invertYVelocity();
                }
                else if (leftCollision < topCollision && leftCollision < bottomCollision && leftCollision < rightCollision) {
                    // Столкновение слева
                    ball->invertXVelocity();
                }
                else {
                    // Столкновение справа
                    ball->invertXVelocity();
                }

                break;
            }
        }
    }
}

void Game::handleBallPaddle(const Uint8* keyboardState) {
    if (keyboardState[SDL_SCANCODE_SPACE] && ball->isStuckToPaddle()) {
        ball->setStuckToPaddle(false);
        ball->setXVelocity(3); // Начальная скорость
        ball->setYVelocity(-3); // Начальная скорость
    }
}