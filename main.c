#include <emulator/lifecycle.h>
#include <emulator/display.h>

void emulator_lifecycle_init(void **appstate, int argc, char *argv[])
{
}

void emulator_lifecycle_iterate(void *appstate)
{
    emulator_display_clear();
    emulator_display_draw_pixel(100, 100, 0xFF, 0, 0);
    emulator_display_refresh();
}

void emulator_lifecycle_quit(void *appstate)
{
}