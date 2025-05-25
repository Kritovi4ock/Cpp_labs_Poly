#include "Ball.h"
#include <cmath>

Ball::Ball(int x, int y, int radius)
    : x(x), y(y), radius(radius), xVelocity(0), yVelocity(0),
    stuckToPaddle(true) {
    rect = { x - radius, y - radius, 2 * radius, 2 * radius };
}

Ball::~Ball() {}

void Ball::update() {
    if (!stuckToPaddle) {
        x += xVelocity;
        y += yVelocity;
    }
    rect.x = x - radius;
    rect.y = y - radius;
    if (isChangingTrajectory) {
        trajectoryChangeTimer++;

        if (trajectoryChangeTimer >= trajectoryChangeInterval) {
            int newXVelocity = (rand() % 5) - 2;
            int newYVelocity = (rand() % 5) - 2;

            xVelocity = newXVelocity;
            yVelocity = newYVelocity;

            trajectoryChangeTimer = 0;
            trajectoryChangeInterval = 0;
            isChangingTrajectory = false;
        }
    }
}

void Ball::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int w = -radius; w <= radius; w++) {
        for (int h = -radius; h <= radius; h++) {
            if (w * w + h * h <= radius * radius) {
                SDL_RenderDrawPoint(renderer, x + w, y + h);
            }
        }
    }
}

void Ball::setXVelocity(int xVel) { xVelocity = xVel; }

void Ball::setYVelocity(int yVel) { yVelocity = yVel; }

int Ball::getXVelocity() { return xVelocity; }

int Ball::getYVelocity() { return yVelocity; }

SDL_Rect Ball::getRect() const { return rect; }

int Ball::getRadius() const { return radius; }

void Ball::invertXVelocity() { xVelocity = -xVelocity; }

void Ball::invertYVelocity() { yVelocity = -yVelocity; }

void Ball::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
    rect.x = x - radius;
    rect.y = y - radius;
}

void Ball::setStuckToPaddle(bool stuck) { stuckToPaddle = stuck; }

bool Ball::isStuckToPaddle() const { return stuckToPaddle; }

void Ball::startRandomTrajectoryChange() {
    isChangingTrajectory = true;
    trajectoryChangeTimer = 0;
    trajectoryChangeInterval = (rand() % 180) + 60;
}