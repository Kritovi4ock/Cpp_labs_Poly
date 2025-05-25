#pragma once
#include <SDL.h>

class Paddle {
public:
	Paddle(int x, int y, int width, int height, int windowWidth);
	~Paddle();

	void handleInput(const Uint8* keyboardState);
	void render(SDL_Renderer* renderer);
	void moveLeft();
	void moveRight();
	SDL_Rect getRect() const;
	void shrink(int amount);
	void widen(int amount);

private:
	SDL_Rect rect;
	int velocity;
	int windowWidth;
};