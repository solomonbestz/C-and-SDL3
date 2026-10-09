#define STB_IMAGE_IMPLEMENTATION
#include "globals.h"
#include "renderer.h"
#include "movesystem.h"

SDL_Event event;
uint8_t isRunning = 1;
const float targetFrame = 1.0/60.0;

char filePath[1024] = "assets/wall.jpg";


int main(void)
{
    rendererInit();
    uint32_t lastCounter = SDL_GetPerformanceCounter();
    float deltaTime;
    
    Circle ball1 = {
        .x = 50, 
        .y = 0,
        .radius = 20,
        .restitution = 0.5,
        .hasGravity = true
    };
    Circle ball2 = {
        .x = 300, 
        .y = 0,
        .radius = 40,
        .restitution = 0.5,
        .hasGravity = true
    };

    Rect floorWall = {
        .x = 0,
        .y = 750.0,
        .width = WIDTH,
        .height = 50
    };

    // Rect r2 = {
    //     .x = 200,
    //     .y = 20
    // };

    Velocity ballOneVel = {
        .dx = 0,
        .dy = 100
    };

    Velocity ballTwoVel = {
        .dx = 0,
        .dy = 100
    };

    Texture floorTex = {};

    load_image_pixels(filePath, &floorTex);

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


        drawCircle(gameFrameBuffer, 0x0000FF, &ball1);
        drawCircle(gameFrameBuffer, 0xFF0000, &ball2);
        drawRect(gameFrameBuffer, 0x00FF00, &floorWall);

        
        drawTexturedRect(gameFrameBuffer, &floorWall, &floorTex);
        
        moveObject(&ballOneVel, &ball1, deltaTime);
        moveObject(&ballTwoVel, &ball2, deltaTime);

        CircleRectCollision(&ball1, &floorWall, &ballOneVel);
        CircleRectCollision(&ball2, &floorWall, &ballTwoVel);

    
        rendererUpdate(gameFrameBuffer);
        frameRatePerSeconds(deltaTime, targetFrame);
    }

    rendererDestroy();
    
    return EXIT_SUCCESS;
}