#ifndef MOVESYSTEM_H
#define MOVESYSTEM_H

#include "renderer.h"


typedef struct Velocity {
    float dx, dy;
} Velocity;

void moveObject(Velocity *vel, Circle *circleObject, float deltaTime);

void CircleRectCollision(Circle *circleObject, Rect *rectObject, Velocity *vel);

#endif