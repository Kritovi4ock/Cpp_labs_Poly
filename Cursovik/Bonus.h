#pragma once
#include <SDL.h>

class Bonus {
public:
    Bonus(int x, int y);
    virtual ~Bonus();

    virtual void apply(class Paddle& paddle, class Ball& ball) = 0;
    virtual void render(SDL_Renderer* renderer);
    virtual void update();

    SDL_Rect getRect() const;
    bool isCollected() const;

protected:
    SDL_Rect rect;
    bool collected;
};

class WidenPaddleBonus : public Bonus {
public:
    WidenPaddleBonus(int x, int y);
    ~WidenPaddleBonus() override;

    void apply(class Paddle& paddle, class Ball& ball) override;
    void render(SDL_Renderer* renderer) override;
};

class ChangeBallSpeedBonus : public Bonus {
public:
    ChangeBallSpeedBonus(int x, int y, int speedChange);
    ~ChangeBallSpeedBonus() override;

    void apply(class Paddle& paddle, class Ball& ball) override;
    void render(SDL_Renderer* renderer) override;

private:
    int speedChange;
};

class StickyPaddleBonus : public Bonus {
public:
    StickyPaddleBonus(int x, int y);
    ~StickyPaddleBonus() override;

    void apply(class Paddle& paddle, class Ball& ball) override;
    void render(SDL_Renderer* renderer) override;
};

class BottomWallBonus : public Bonus {
public:
    BottomWallBonus(int x, int y);
    ~BottomWallBonus() override;

    void apply(class Paddle& paddle, class Ball& ball) override;
    void render(SDL_Renderer* renderer) override;
};