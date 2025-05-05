#include <emulator/lifecycle.h>
#include <emulator/display.h>

void emulator_lifecycle_init(void **appstate, int argc, char *argv[])
{
}

void emulator_lifecycle_iterate(void *appstate)
{
    emulator_display_clear();

    for (uint16_t x = 0; x < 320; x++)
    {
        for (uint16_t y = 0; y < 240; y++)
        {
            emulator_display_draw_pixel(x, y, x + y, x * y, x - y);
        }
    }

    emulator_display_refresh();
}

void emulator_lifecycle_quit(void *appstate)
{
}