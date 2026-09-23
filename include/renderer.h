#ifndef RENDERER_H
#define RENDERER_H

#include "globals.h"


typedef struct Circle {
    double x;
    double y;
    double radius;
} Circle;

typedef struct Rect {
    double x;
    double y;
} Rect;

int rendererInit();

int rendererUpdate(uint32_t *frameBuffer);

int rendererDestroy();

void clearBuffer(uint32_t *frameBuffer, size_t width, size_t height, uint32_t color);

void drawCircle(uint32_t *frameBuffer, uint32_t color, Circle *circle);

void drawRect(uint32_t *frameBuffer, uint32_t color, Rect *rect);


#endif