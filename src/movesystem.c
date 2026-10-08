
#include "movesystem.h"

static float gravity = 980.0f;


void moveObject(Velocity *vel, Circle *circleObject, double deltaTime)
{
    if(circleObject->hasGravity)
    {
        vel->dy += vel->dy * deltaTime;
        circleObject->y += vel->dy * deltaTime;
    }
    
    circleObject->y += vel->dy * deltaTime;
}