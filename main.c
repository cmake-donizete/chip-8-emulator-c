#include <emulator/lifecycle.h>
#include <emulator/display.h>

#define CHIP_8_PROGRAM_START 0x200
#define CHIP_8_WIDTH 64
#define CHIP_8_HEIGHT 32

struct chip_8_t {
    uint8_t memory[4096];
    uint16_t pc;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t rg[16];
    uint16_t I;
};

static struct chip_8_t chip_8;

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