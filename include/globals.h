#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdint.h>
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 1280
#define HEIGHT 800


extern uint32_t gameFrameBuffer[WIDTH * HEIGHT];


typedef struct Circle {
    double x;
    double y;
    double radius;
} Circle;



#endif