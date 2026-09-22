#ifndef RENDERER_H
#define RENDERER_H

#include "globals.h"

int rendererInit();

void clearBuffer(uint32_t *frame_buffer, size_t width, size_t height, uint32_t color);

void createCircleObject(uint32_t *frame_buffer, uint32_t color, Circle *circle);

#endif