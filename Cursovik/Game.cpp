#include "Game.h"
#include "Utils.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <memory>

Game::Game(const char* title, int width, int height) : window(nullptr), renderer(nullptr), running(false), windowWidth(width), windowHeight(height), paddle(nullptr), ball(nullptr), hasBottomWall(false), bottomWallRect({ 0, 0, 0, 0 }) { srand(time(0)); }

Game::~Game() {
    if (paddle) delete paddle;
    if (ball) delete ball;
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
    ball = new Ball(windowWidth / 2, windowHeight - 50 - 10, 10);
    createBlocks();

    running = true;
    return true;
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        double frameTime = (currentTime - lastTime) / 1000.0;
        lastTime = currentTime;

        accumulator += frameTime;

        handleEvents();

        while (accumulator >= fixedTimeStep) {
            update();
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
        ball->setPosition(paddle->getRect().x + paddle->getRect().w / 2, paddle->getRect().y - ball->getRadius());
    }

    ball->update();
    checkCollisions();
    checkBonusCollisions();

    for (auto& bonusPtr : bonuses) {
        bonusPtr->update();
    }

    if (ball->getRect().x <= 0 || ball->getRect().x + 2 * ball->getRadius() >= windowWidth) {
        ball->invertXVelocity();
    }
    if (ball->getRect().y <= 0) {
        ball->invertYVelocity();
    }

    if (ball->getRect().y + 2 * ball->getRadius() >= windowHeight) {
        if (hasBottomWall) {
            checkBottomWallCollision();
        }
        else {
            paddle->shrink(10);
            ball->setStuckToPaddle(true);
            ball->setPosition(paddle->getRect().x + paddle->getRect().w / 2, paddle->getRect().y - ball->getRadius());
            ball->setXVelocity(0);
            ball->setYVelocity(0);
        }


    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    paddle->render(renderer);
    ball->render(renderer);

    for (const auto& blockPtr : blocks) {
        blockPtr->render(renderer);
    }
    for (const auto& bonusPtr : bonuses) {
        bonusPtr->render(renderer);
    }
    if (hasBottomWall) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &bottomWallRect);
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

            int randomNumber = rand() % 10;

            if (randomNumber == 0) {
                blocks.push_back(std::make_unique<HealthBlock>(x, y, blockWidth, blockHeight, 2));
            }
            else if (randomNumber == 1) {
                blocks.push_back(std::make_unique<SpecialBlock>(x, y, blockWidth, blockHeight));
            }
            else if (randomNumber == 2) {
                blocks.push_back(std::make_unique<SpeedUpBlock>(x, y, blockWidth, blockHeight, 2));
            }
            else if (randomNumber == 3) {
                int bonusType = rand() % 4;
                std::unique_ptr<Bonus> bonus;
                switch (bonusType) {
                case 0:
                    bonus = std::make_unique<WidenPaddleBonus>(x, y);
                    break;
                case 1:
                    bonus = std::make_unique<ChangeBallSpeedBonus>(x, y, 1);
                    break;
                case 2:
                    bonus = std::make_unique<StickyPaddleBonus>(x, y);
                    break;
                case 3:
                    bonus = std::make_unique<BottomWallBonus>(x, y);
                    break;
                }
                blocks.push_back(std::make_unique<BonusBlock>(x, y, blockWidth, blockHeight, std::move(bonus)));
            }

            else {
                blocks.push_back(std::make_unique<Block>(x, y, blockWidth, blockHeight));
            }
        }
    }
}

void Game::checkCollisions() {
    if (Utils::checkCollision(ball->getRect(), paddle->getRect()) && !ball->isStuckToPaddle()) {
        ball->invertYVelocity();
    }

    for (size_t i = 0; i < blocks.size(); ++i) {
        SDL_Rect blockRect = blocks[i]->getRect();
        SDL_Rect ballRect = ball->getRect();

        if (Utils::checkCollision(ballRect, blockRect)) {
            SpeedUpBlock* speedUpBlock = dynamic_cast<SpeedUpBlock*>(blocks[i].get());
            HealthBlock* healthBlock = dynamic_cast<HealthBlock*>(blocks[i].get());
            BonusBlock* bonusBlock = dynamic_cast<BonusBlock*>(blocks[i].get());
            if (!blocks[i]->isDestroyed())
            {
                int topCollision = ballRect.y + ballRect.h - blockRect.y;
                int bottomCollision = blockRect.y + blockRect.h - ballRect.y;
                int leftCollision = ballRect.x + ballRect.w - blockRect.x;
                int rightCollision = blockRect.x + blockRect.w - ballRect.x;
                if (speedUpBlock) {
                    speedUpBlock->onCollision(*ball);
                    if (topCollision < bottomCollision && topCollision < leftCollision && topCollision < rightCollision) {
                        ball->invertYVelocity();
                    }
                    else if (bottomCollision < topCollision && bottomCollision < leftCollision && bottomCollision < rightCollision) {

                        ball->invertYVelocity();
                    }
                    else if (leftCollision < topCollision && leftCollision < bottomCollision && leftCollision < rightCollision) {

                        ball->invertXVelocity();
                    }
                    else {

                        ball->invertXVelocity();
                    }

                    blocks[i]->destroy();
                    break;
                }
                else if (healthBlock) {

                    if (topCollision < bottomCollision && topCollision < leftCollision && topCollision < rightCollision) {

                        ball->invertYVelocity();
                    }
                    else if (bottomCollision < topCollision && bottomCollision < leftCollision && bottomCollision < rightCollision) {

                        ball->invertYVelocity();
                    }
                    else if (leftCollision < topCollision && leftCollision < bottomCollision && leftCollision < rightCollision) {
                        ball->invertXVelocity();
                    }
                    else {
                        ball->invertXVelocity();
                    }
                    healthBlock->takeDamage();
                    break;
                }
                else if (bonusBlock) {
                    Bonus* bonus = bonusBlock->getBonus();
                    if (bonus) {
                        bonuses.push_back(std::unique_ptr<Bonus>(bonus));
                    }
                    else {
                        std::cerr << "Error: Bonus pointer is null!" << std::endl;
                    }
                    if (topCollision < bottomCollision && topCollision < leftCollision && topCollision < rightCollision) {
                        ball->invertYVelocity();
                    }
                    else if (bottomCollision < topCollision && bottomCollision < leftCollision && bottomCollision < rightCollision) {
                        ball->invertYVelocity();
                    }
                    else if (leftCollision < topCollision && leftCollision < bottomCollision && leftCollision < rightCollision) {
                        ball->invertXVelocity();
                    }
                    else {
                        ball->invertXVelocity();
                    }
                    blocks[i]->destroy();
                    break;
                }
                else
                {
                    if (topCollision < bottomCollision && topCollision < leftCollision && topCollision < rightCollision) {
                        ball->invertYVelocity();
                    }
                    else if (bottomCollision < topCollision && bottomCollision < leftCollision && bottomCollision < rightCollision) {
                        ball->invertYVelocity();
                    }
                    else if (leftCollision < topCollision && leftCollision < bottomCollision && leftCollision < rightCollision) {
                        ball->invertXVelocity();
                    }
                    else {
                        ball->invertXVelocity();
                    }

                    blocks[i]->destroy();
                    break;
                }
            }
        }
    }
}
void Game::checkBonusCollisions() {
    for (size_t i = 0; i < bonuses.size(); ++i) {
        BottomWallBonus* bottomWallBonus = dynamic_cast<BottomWallBonus*>(bonuses[i].get());
        if (Utils::checkCollision(paddle->getRect(), bonuses[i]->getRect())) {

            if (bottomWallBonus) {
                hasBottomWall = true;
                bottomWallRect.x = 0;
                bottomWallRect.y = windowHeight - 20;
                bottomWallRect.w = windowWidth;
                bottomWallRect.h = 10;
            }
            bonuses[i]->apply(*paddle, *ball);
            bonuses.erase(bonuses.begin() + i);
            i--;
        }
        else if (!bonuses[i]->isCollected() && Utils::checkCollision(paddle->getRect(), bonuses[i]->getRect())) {
            bonuses[i]->apply(*paddle, *ball);
            bonuses.erase(bonuses.begin() + i);
            i--;
        }
    }
}
void Game::handleBallPaddle(const Uint8* keyboardState) {
    if (keyboardState[SDL_SCANCODE_SPACE] && ball->isStuckToPaddle()) {
        ball->setStuckToPaddle(false);
        ball->setXVelocity(3);
        ball->setYVelocity(-3);
    }
}

void Game::checkBottomWallCollision() {
    if (Utils::checkCollision(ball->getRect(), bottomWallRect)) {
        ball->invertYVelocity();
        hasBottomWall = false;
        bottomWallRect.x = 0;
        bottomWallRect.y = 0;
        bottomWallRect.w = 0;
        bottomWallRect.h = 0;
    }
}