#include "globals.h"
#include "renderer.h"
#include "movesystem.h"

SDL_Event event;
uint8_t isRunning = 1;

int main(void)
{
    setbuf(stdout, NULL);

    uint32_t lastCounter = SDL_GetPerformanceCounter();
    double deltaTime;
    
    Circle c1 = {
        .x = 50, 
        .y = 10,
        .radius = 20,
        .hasGravity = false
    };
    Circle c2 = {
        .x = 300, 
        .y = 50,
        .radius = 40,
        .hasGravity = true
    };

    Rect r1 = {
        .x = 100,
        .y = 50
    };

    Rect r2 = {
        .x = 200,
        .y = 20
    };

    Velocity vel = {
        .dx = 0,
        .dy = 100
    };

    // Vector3D cube = {
    //     .x = 0.0f,
    //     .y = 0.0f,
    //     .z = 2.0f
    // };


    // Triangle3D triangle = {
    //     .vertices = {
    //         {.x = -100.0f, .y = 100.0f, .z = 300.f},
    //         {.x = 100.0f, .y = 100.0f, .z = 400.0f},
    //         {.x = 0.0f, .y = -100.0f, .z = 300.0f}
    //     }
    // };
    
    rendererInit();

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

        deltaTime = (double)(currentCounter - lastCounter) / (double)SDL_GetPerformanceFrequency();

        lastCounter = currentCounter;


        clearBuffer(gameFrameBuffer, WIDTH, HEIGHT, 0x000000);


        drawCircle(gameFrameBuffer, 0x0000FF, &c1);
        drawCircle(gameFrameBuffer, 0xFF0000, &c2);
        drawRect(gameFrameBuffer, 0x00FF00, &r1);
        moveObject(&vel, &c1, deltaTime);
        moveObject(&vel, &c2, deltaTime);

        // drawRect(gameFrameBuffer, 0x3F3F3F, &r2);
        rendererUpdate(gameFrameBuffer);
    }

    rendererDestroy();
    
    return EXIT_SUCCESS;
}