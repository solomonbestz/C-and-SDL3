#include "renderer.h"


static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;

int rendererInit()
{
    

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
}

int rendererUpdate(uint32_t *frameBuffer)
{
    SDL_UpdateTexture(texture, NULL, frameBuffer, WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int rendererDestroy()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void clearBuffer(uint32_t *frameBuffer, size_t width, size_t height, uint32_t color)
{
    for(uint32_t i = 0; i < width * height; i++)
    {
        frameBuffer[i] = color;
    }
}

void drawCircle(uint32_t *frameBuffer, uint32_t color, Circle *circle)
{
    int startX = (int)(circle->x - circle->radius);
    int startY = (int)(circle->y - circle->radius);
    int endX = (int)(circle->x + circle->radius);
    int endY = (int)(circle->y + circle->radius);

    if(startX < 0) startX = 0;
    if(startY < 0) startY = 0;
    if (endX >= WIDTH) endX = WIDTH - 1;
    if(endY >= HEIGHT) endY = HEIGHT - 1;

    for (int y = startY; y < endY; y++)
    {
        for(int x = startX; x < endX; x++)
        {
            double dX = x - circle->x;
            double dY = y - circle->y;

            if((dX * dX) + (dY * dY) <= (circle->radius * circle->radius))
            {
                frameBuffer[(y * WIDTH) + x] = color;
            }
        }
    }
    
}

void drawRect(uint32_t *frameBuffer, uint32_t color, Rect *rect)
{   
    // (x - centerX) - halfX(w), (y - centerY) - halfY(h)

    int rectX = (int)rect->x;
    int rectY = (int)rect->y;

    int halfX = rectX / 2;
    int halfY = rectY / 2;
    
    int centerX = rectX + halfX;
    int centerY = rectY + halfY;

    int startX = rectX;
    int startY = rectY;
    int endX = rectX + rectX;
    int endY = rectY + rectY;

    

    for(int y = startY; y < endY; y++ )
    {
        for(int x = startX; x < endX; x++)
        {
            int result = SDL_max((x - centerX) - halfX, (y - centerY) - halfY);

            if(result <= 0)
            {
                frameBuffer[(y * WIDTH) + x] = color;
            }
        }
    }
}
    