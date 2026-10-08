#include "globals.h"
#include "renderer.h"
#include "movesystem.h"

SDL_Event event;
uint8_t isRunning = 1;
const float targetFrame = 1.0/60.0;

int main(void)
{
    rendererInit();
    uint32_t lastCounter = SDL_GetPerformanceCounter();
    float deltaTime;
    
    Circle c1 = {
        .x = 50, 
        .y = 0,
        .radius = 20,
        .restitution = 0.5,
        .hasGravity = true
    };
    Circle c2 = {
        .x = 300, 
        .y = 0,
        .radius = 40,
        .restitution = 0.5,
        .hasGravity = true
    };

    Rect r1 = {
        .x = 0,
        .y = 750.0,
        .width = WIDTH,
        .height = 50
    };

    Rect r2 = {
        .x = 200,
        .y = 20
    };

    Velocity vel1 = {
        .dx = 0,
        .dy = 100
    };

    Velocity vel2 = {
        .dx = 0,
        .dy = 100
    };


    while (isRunning)
    {
        uint32_t currentCounter = SDL_GetPerformanceCounter();

        // Event polling
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
            {
                isRunning = 0;
            }
        }

        deltaTime = (float)(currentCounter - lastCounter) / (float)SDL_GetPerformanceFrequency();

        lastCounter = currentCounter;


        clearBuffer(gameFrameBuffer, WIDTH, HEIGHT, 0x000000);


        drawCircle(gameFrameBuffer, 0x0000FF, &c1);
        drawCircle(gameFrameBuffer, 0xFF0000, &c2);
        drawRect(gameFrameBuffer, 0x00FF00, &r1);
        
        moveObject(&vel1, &c1, deltaTime);
        moveObject(&vel2, &c2, deltaTime);

        CircleRectCollision(&c1, &r1, &vel1);
        CircleRectCollision(&c2, &r1, &vel2);

        // drawRect(gameFrameBuffer, 0x3F3F3F, &r2);
        rendererUpdate(gameFrameBuffer);
        frameRatePerSeconds(deltaTime, targetFrame);
    }

    rendererDestroy();
    
    return EXIT_SUCCESS;
}