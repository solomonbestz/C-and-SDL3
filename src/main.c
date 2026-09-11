#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

#define WIDTH 600
#define HEIGHT 800


uint32_t frame_buffer[WIDTH * HEIGHT];

typedef struct Circle {
    double x;
    double y;
} Circle;


void clear_buffer(uint32_t *frame_buffer, uint32_t width, uint32_t height, uint32_t color)
{
    for(uint32_t i = 0; i < width * height; i++)
    {
        frame_buffer[i] = color;
    }
}


void create_circle_object(uint32_t *frame_buffer, uint32_t radius, uint32_t color, uint32_t *speed, Circle circle)
{

    for (uint32_t y = 0; y < (int)(circle.y + radius); y++)
    {
        for(uint32_t x = 0; x < (int)(circle.x + radius); x++)
        {
            int32_t change_in_x = x - circle.x;
            int32_t change_in_y = y - circle.y;

            if((change_in_x * change_in_x) + (change_in_y * change_in_y) <= pow(radius, 2))
            {
                frame_buffer[y * WIDTH + x] = color;
            }
        }
    }
    
}

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
    
    Circle c1 = {.x = 50, .y = 10};
    Circle c2 = {.x = 300, .y = 50};


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

        clear_buffer(frame_buffer, WIDTH, HEIGHT, 0x000000);
        create_circle_object(frame_buffer, 50, 0x0000FF, &speed, c1);
        create_circle_object(frame_buffer, 50, 0xFF0000, &speed, c2);


        SDL_UpdateTexture(texture, NULL, frame_buffer, WIDTH * sizeof(uint32_t));
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