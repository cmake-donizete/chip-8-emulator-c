#include <stdio.h>

#include <fcntl.h>
#include <unistd.h>

#include <emulator/lifecycle.h>
#include <emulator/display.h>

#define CHIP_8_PROGRAM_START 0x200

struct chip_8_t
{
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
    int fd;
    size_t count;
    uint8_t *buffer = chip_8.memory + CHIP_8_PROGRAM_START;

    fd = open("../rom/ibmlogo.ch8", O_RDONLY);
    count = read(fd, buffer, sizeof(chip_8.memory) - CHIP_8_PROGRAM_START);

    for (size_t start = 0; start < count; start += 2)
    {
        if (start % 0x10 == 0)
        {
            printf("\n");
        }
        printf("%02x", buffer[start + 1]);
        printf("%02x", buffer[start]);
        printf(" ");
    }

    close(fd);
}

void emulator_lifecycle_iterate(void *appstate)
{
    emulator_display_clear();

    for (uint16_t x = 0; x < RENDER_WIDTH; x++)
    {
        for (uint16_t y = 0; y < RENDER_HEIGHT; y++)
        {
            emulator_display_draw_pixel(x, y, x + y, x * y, x - y);
        }
    }

    emulator_display_refresh();
}

void emulator_lifecycle_quit(void *appstate)
{
}