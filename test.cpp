#include <iostream>
#include <SDL3/SDL.h>


SDL_Window *window;
SDL_Event event;

int main()
{
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "SDL INITIALIZATION FAILED" << std::endl;
        return 0;
    }
    
    window = SDL_CreateWindow("Test", 600, 300, 0);

    if(window == NULL)
    {
        std::cout << "Window Failed To Create" << std::endl;
        SDL_Quit();
        return 0;
    }

    int isRunning = 1;
    
    while(isRunning)
    {
        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
            {
                isRunning = 0;
            }
        }
    }

    SDL_DestroyWindow(window);

    return 0;
}