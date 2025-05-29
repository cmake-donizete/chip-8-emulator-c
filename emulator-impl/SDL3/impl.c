#define SDL_MAIN_USE_CALLBACKS 1

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH 1920
#endif

#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT 1080
#endif

#define RENDERER_WIDTH 320
#define RENDERER_HEIGHT 240

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <emulator/lifecycle.h>
#include <emulator/display.h>

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
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_RESIZABLE,
            &window,
            &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(
        renderer,
        RENDERER_WIDTH,
        RENDERER_HEIGHT,
        SDL_LOGICAL_PRESENTATION_LETTERBOX);

    emulator_lifecycle_init(appstate, argc, argv);

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
    emulator_lifecycle_iterate(appstate);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    emulator_lifecycle_quit(appstate);
}

void emulator_display_clear()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
}

void emulator_display_refresh()
{
    SDL_RenderPresent(renderer);
}

void emulator_display_draw_pixel(
    uint16_t x,
    uint16_t y,
    uint8_t r,
    uint8_t g,
    uint8_t b)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
    SDL_RenderPoint(renderer, x, y);
}