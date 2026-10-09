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

void frameRatePerSeconds(float deltaTime, float targetFraame)
{
    if(deltaTime < targetFraame)
    {
        SDL_Delay((targetFraame - deltaTime) * 1000.0);
    }
}

void clearBuffer(uint32_t *frameBuffer, size_t width, size_t height, uint32_t color)
{
    for(uint32_t i = 0; i < width * height; i++)
    {
        frameBuffer[i] = color;
    }
}

void clearZBuffer(float *zBuffer, size_t width, size_t height)
{
    for(size_t i = 0; i < width * height; i++)
    {
        zBuffer[i] = 10000.0f;
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

    int startX = (int)rect->x;
    int startY = (int)rect->y;
    int endX = startX + (int)rect->width;
    int endY = startY + (int)rect->height;

    

    for(int y = startY; y < endY; y++ )
    {
        for(int x = startX; x < endX; x++)
        {
            frameBuffer[(y * WIDTH) + x] = color;
        }
    }
}

void load_image_pixels(const char *filePath, Texture *tex)
{
    int width, height, channels;

    int desired_channels = 4;

    unsigned char *pixels_data = stbi_load(filePath, &width, &height, &channels, desired_channels);

    if(pixels_data == NULL)
    {
        SDL_Log("Failed to load image at %s\n", filePath);
        return;
    }

    // SDL_Log("Image Loaded: %dx%d (%d channels)\n", width, height, channels);


    tex->width = width;
    tex->height = height;

    tex->pixels = (uint32_t*)pixels_data;

    
}


void drawTexturedRect(uint32_t *frameBuffer, Rect *rect, Texture *tex)
{
    int minX = (int)rect->x;
    int minY = (int)rect->y;

    int maxX = minX + rect->width;
    int maxY = minY + rect->height;

    if (minX < 0) minX = 0;
    if (minY < 0) minY = 0;
    if (maxX >= WIDTH) maxX = WIDTH - 1;
    if (maxY >= HEIGHT) maxY = HEIGHT - 1;

    for(int y = minY; y <= maxY; y++)
    {
        float v = (float)(y - rect->y) / rect->height;
        int texY = (int)(v * tex->height);

        for (int x = minX; x <= maxX; x++)
        {
            float u = (float)(x - rect->x) / rect->width;
            int texX = (int)(u * tex->width);

            if(texX >= tex->width) texX = tex->width - 1;
            if(texY >= tex->height) texY = tex->height - 1;

            uint32_t color = tex->pixels[(texY * tex->width) + texX];

            if((color & 0xFF000000) != 0)
            {
                frameBuffer[(y * WIDTH) + x] = color;
            }
        }
    }

    stbi_image_free(tex->pixels);
    tex->pixels = NULL;

}