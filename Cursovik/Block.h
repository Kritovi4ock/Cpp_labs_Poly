#pragma once
#include "Ball.h"
#include <SDL.h>
#include <memory>
class Bonus;
class Block {
public:
	Block(int x, int y, int width, int height);
	virtual ~Block();

	virtual void render(SDL_Renderer* renderer);
	const SDL_Rect& getRect() const;
	virtual bool isDestroyed() const;
	virtual void destroy();
	virtual void takeDamage();

protected:
	SDL_Rect rect;
	bool destroyed;
};

class SpecialBlock : public Block {
public:
	SpecialBlock(int x, int y, int width, int height);
	~SpecialBlock() override;

	void render(SDL_Renderer* renderer) override;
	bool isDestroyed() const override;
	void destroy() override;
};

class SpeedUpBlock : public Block {
public:
	SpeedUpBlock(int x, int y, int width, int height, int speedIncrease);
	~SpeedUpBlock() override;

	void render(SDL_Renderer* renderer) override;
	void onCollision(Ball& ball);
	bool isDestroyed() const override;
	void destroy() override;

private:
	int speedIncrease;
};

class HealthBlock : public Block {
public:
	HealthBlock(int x, int y, int width, int height, int initialHealth);
	~HealthBlock() override;

	void render(SDL_Renderer* renderer) override;
	void takeDamage();
	bool isDestroyed() const override;

private:
	int health;
	int initialHealth;
};

class BonusBlock : public Block {
public:
	BonusBlock(int x, int y, int width, int height, std::unique_ptr<Bonus> bonus);
	~BonusBlock() override;
	void render(SDL_Renderer* renderer) override;
	bool isDestroyed() const override;
	void destroy() override;
	Bonus* getBonus() const;

private:
	std::unique_ptr<Bonus> bonus;
};