#pragma once
#include "Ball.h"
#include "Block.h"
#include "Bonus.h"
#include "Paddle.h"
#include <memory>
#include <vector>

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
	bool hasBottomWall;
	SDL_Rect bottomWallRect;
	Paddle* paddle;
	Ball* ball;
	std::vector<std::unique_ptr<Block>> blocks;
	std::vector<std::unique_ptr<Bonus>> bonuses;
	void handleEvents();
	void update();
	void render();
	void createBlocks();
	void checkCollisions();
	void checkBonusCollisions();
	void handleBallPaddle(const Uint8* keyboardState);
	void checkBottomWallCollision();
	const double fixedTimeStep = 1.0 / 60.0;
	double accumulator = 0.0;
};