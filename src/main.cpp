//
// Created by Lazar on 11/14/2025.
//

#include <SDL2/SDL.h>

#include <platform.h>

static SDL_DisplayMode window_size;

static const char* window_title{platform::windows::TITLE};

static constexpr int window_width{platform::windows::WIDTH};
static constexpr int window_height{platform::windows::HEIGHT};

int SDL_main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_GetCurrentDisplayMode(0, &window_size);

    const int position_x = (window_size.w - window_width) / 2;
    const int position_y = (window_size.h - window_height) / 2;

    SDL_Window* window = SDL_CreateWindow(window_title,
                                          position_x, position_y,
                                          window_width, window_height,
                                          SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return 1;
    }


    SDL_GL_SetSwapInterval(1);
    SDL_Event event;

    bool running = true;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
