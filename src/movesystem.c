
#include "movesystem.h"

static float gravity = 980.0f;


void moveObject(Velocity *vel, Circle *circleObject, float deltaTime)
{
    if(circleObject->hasGravity)
    {
        vel->dy += gravity * deltaTime;
        
    }
    
    circleObject->x += vel->dx * deltaTime;
    circleObject->y += vel->dy * deltaTime;
}

void CircleRectCollision(Circle *circleObject, Rect *rectObject, Velocity *vel)
{
    int circleBottom = circleObject->y + circleObject->radius;


    if(circleBottom >= rectObject->y)
    {
        circleObject->y = rectObject->y - circleObject->radius;
        vel->dy = -vel->dy * circleObject->restitution;
    }
}