#include "globals.h"
#include "renderer.h"


int main(void)
{
    setbuf(stdout, NULL);

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event event;
    uint8_t isRunning = 1;
    uint32_t speed = 0;

    uint32_t last_counter = SDL_GetPerformanceCounter();
    double delta_time;
    
    Circle c1 = {
        .x = 50, 
        .y = 10,
        .radius = 20
    };
    Circle c2 = {
        .x = 300, 
        .y = 50,
        .radius = 40
    };


    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL Init failed: %s \n", SDL_GetError());
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("MY WINDOW", WIDTH, HEIGHT, 0);

    if(window == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow failed: %s \n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    // printf("Renderer: %s\n", SDL_GetRendererName(renderer));

    if(renderer == NULL)
    {
        fprintf(stderr, "SDL_CreateRenderer failed: %s \n", SDL_GetError());
        SDL_Quit();
        SDL_DestroyWindow(window);
        return EXIT_FAILURE;
    }

    texture = SDL_CreateTexture(  
        renderer, 
        SDL_PIXELFORMAT_XRGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );

    if (texture == NULL)
    {
        fprintf(stderr, "SDL_CreateTexture failed: %s \n", SDL_GetError());
        SDL_Quit();
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        return EXIT_FAILURE;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    

    while (isRunning)
    {
        uint32_t current_counter = SDL_GetPerformanceCounter();

        // Event polling
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
            {
                isRunning = 0;
            }
        }

        delta_time = (double)(current_counter - last_counter) / (double)SDL_GetPerformanceFrequency();

        last_counter = current_counter;

        // SDL_Log("Delta time: %f", delta_time); 

        clearBuffer(gameFrameBuffer, WIDTH, HEIGHT, 0x000000);
        createCircleObject(gameFrameBuffer, 0x0000FF, &c1);
        createCircleObject(gameFrameBuffer, 0xFF0000, &c2);


        SDL_UpdateTexture(texture, NULL, gameFrameBuffer, WIDTH * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}