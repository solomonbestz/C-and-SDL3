#ifndef RENDERER_H
#define RENDERER_H

#include "globals.h"



typedef struct Circle {
    double x;
    double y;
    double radius;
    bool hasGravity; 
} Circle;

typedef struct Rect {
    double x;
    double y;
} Rect;

typedef struct Vector3D {
    float x, y, z;
} Vector3D;

typedef struct Triangle3D {
    Vector3D vertices[3];
} Triangle3D;

int rendererInit();

int rendererUpdate(uint32_t *frameBuffer);

int rendererDestroy();

void clearBuffer(uint32_t *frameBuffer, size_t width, size_t height, uint32_t color);

void clearZBuffer(float *zBuffer, size_t width, size_t height);

void drawCircle(uint32_t *frameBuffer, uint32_t color, Circle *circle);

void drawRect(uint32_t *frameBuffer, uint32_t color, Rect *rect);

void drawPoint3D(uint32_t *framebuffer, float *zBuffer, uint32_t color, Vector3D *point);

void drawTriangle3D(uint32_t *framebuffer, float *zBuffer, uint32_t color, Triangle3D *tri);


#endif