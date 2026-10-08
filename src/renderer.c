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
    
void drawPoint3D(uint32_t *framebuffer, float *zBuffer, uint32_t color, Vector3D *point)
{
    if(point->z <= 0.1f) return;

    float fovScale = WIDTH * 0.7f;

    int screenX = (int)((point->x / point->z) * fovScale) + (WIDTH / 2);
    int screenY = (int)((point->y / point->z) * fovScale) + (HEIGHT / 2);

    int pointSize = 5;
    int halfSize = pointSize / 2;

    for(int yOffset = -halfSize; yOffset <= halfSize; yOffset++)
    {
        for(int xOffset = -halfSize; xOffset <= halfSize; xOffset++)
        {
            int drawX = screenX + xOffset;
            int drawY = screenY + yOffset;

            if(drawX >= 0 && drawX < WIDTH && drawY >= 0 && drawY < HEIGHT)
            {
                int pixelIndex = (drawY * WIDTH) + drawX;

                if(point->z < zBuffer[pixelIndex])
                {
                    zBuffer[pixelIndex] = point->z;

                    framebuffer[pixelIndex] = color;
                }
            }

        }
    }

    
}

float edgeFunction(float ax, float ay, float bx, float by, float cx, float cy)
{
    return (cx - ax) * (by - ay) - (cy - ay) * (bx - ax);
}

void drawTriangle3D(uint32_t *framebuffer, float *zBuffer, uint32_t color, Triangle3D *tri)
{
    float fovScale = WIDTH * 0.7f;
    float screenX[3];
    float screenY[3];
    float screenZ[3];

    for(int i = 0; i < 3; i++)
    {
        if(tri->vertices[i].z <= 0.1f) return;

        screenX[i] = ((tri->vertices[i].x / tri->vertices[i].z) * fovScale) + (WIDTH / 2.0f);
        screenY[i] = ((tri->vertices[i].y / tri->vertices[i].z) * fovScale) + (HEIGHT / 2.0f);
        screenZ[i] = tri->vertices[i].z;
    }

    int minX = SDL_max(0, (int)SDL_min(screenX[0], SDL_min(screenX[1], screenX[2])));
    int maxX = SDL_min(WIDTH - 1, (int)SDL_max(screenX[0], SDL_max(screenX[1], screenX[2])));
    int minY = SDL_max(0, (int)SDL_min(screenY[0], SDL_min(screenY[1], screenY[2])));
    int maxY = SDL_min(HEIGHT - 1, (int)SDL_max(screenY[0], SDL_max(screenY[1], screenY[2])));

    float area = edgeFunction(screenX[0], screenY[0], screenX[1], screenY[1], screenX[2], screenY[2]);
    if(area == 0) return;

    for(int y = minY; y <= maxY; y++)
    {
        for (int x = minX; x <= maxX; x++)
        {
            float w0 = edgeFunction(screenX[1], screenY[1], screenX[2], screenY[2], x, y);
            float w1 = edgeFunction(screenX[2], screenY[2], screenX[0], screenY[0], x, y);
            float w2 = edgeFunction(screenX[0], screenY[0], screenX[1], screenY[1], x, y);

            if((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w1 <= 0 && w2 <= 0))
            {
                w0 /= area;
                w1 /= area;
                w2 /= area;

                float pixelZ = 1.0f / (w0 * (1.0f / screenZ[0]) + w1 * (1.0f / screenZ[1]) + w2 * (1.0f / screenZ[2]));

                int pixelIndex = (y * WIDTH) + x;

                if (pixelZ < zBuffer[pixelIndex])
                {
                    zBuffer[pixelIndex] = pixelZ;
                    framebuffer[pixelIndex] = color;
                }
            }
        }
    }
}