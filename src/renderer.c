#include "globals.h"


void renderInit()
{

}

void clearBuffer(uint32_t *frame_buffer, size_t width, size_t height, uint32_t color)
{
    for(uint32_t i = 0; i < width * height; i++)
    {
        frame_buffer[i] = color;
    }
}

void createCircleObject(uint32_t *frame_buffer, uint32_t color, Circle *circle)
{
    int startX = (int)(circle->x - circle->radius);
    int startY = (int)(circle->y - circle->radius);
    int endX = (int)(circle->x + circle->radius);
    int endY = (int)(circle->y + circle->radius);

    if(startX < 0)
    {
        startX = 0;
    }
    if(startY < 0)
    {
        startY = 0;
    }
    if (endX >= WIDTH)
    {
        endX = WIDTH - 1;
    }
    if(endY >= HEIGHT)
    {
        endY = HEIGHT - 1;
    }


    for (int y = startY; y < endY; y++)
    {
        for(int x = startX; x < endX; x++)
        {
            double dX = x - circle->x;
            double dY = y - circle->y;

            if((dX * dX) + (dY * dY) <= (circle->radius * circle->radius))
            {
                frame_buffer[(y * WIDTH) + x] = color;
            }
        }
    }
    
}