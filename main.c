#include <stdio.h>

#define SDL_MAIN_USE_CALLBACKS 1
#define WIDTH 320
#define HEIGHT 240

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(
            "Chip 8 Emulator",
            WIDTH,
            HEIGHT,
            SDL_WINDOW_RESIZABLE,
            &window,
            &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(
        renderer,
        WIDTH,
        HEIGHT,
        SDL_LOGICAL_PRESENTATION_LETTERBOX);

    if (!(texture = SDL_CreateTexture(
              renderer,
              SDL_PIXELFORMAT_ARGB32,
              SDL_TEXTUREACCESS_STREAMING,
              WIDTH,
              HEIGHT)))
    {
        SDL_Log("Couldn't create texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0);
    SDL_FPoint points1[] = {
        {0, 0},
        {WIDTH, HEIGHT},
    };
    SDL_RenderLines(renderer, points1, 2);

    SDL_FPoint points2[] = {
        {0, 0},
        {WIDTH, 0},
    };
    SDL_RenderLines(renderer, points2, 2);

    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}