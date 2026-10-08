#ifndef RENDERER_H
#define RENDERER_H

#include "globals.h"



typedef struct Circle {
    float x;
    float y;
    float radius;
    float restitution;
    bool hasGravity; 
} Circle;

typedef struct Rect {
    float x;
    float y;
    float width;
    float height;
    float restitution;
} Rect;


int rendererInit();

int rendererUpdate(uint32_t *frameBuffer);

int rendererDestroy();

void frameRatePerSeconds(float deltaTime, float targetFraame);

void clearBuffer(uint32_t *frameBuffer, size_t width, size_t height, uint32_t color);

void drawCircle(uint32_t *frameBuffer, uint32_t color, Circle *circle);

void drawRect(uint32_t *frameBuffer, uint32_t color, Rect *rect);

#endif