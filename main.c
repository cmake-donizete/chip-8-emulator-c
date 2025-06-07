#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include <emulator/lifecycle.h>
#include <emulator/display.h>

#define CHIP_8_PROGRAM_START 0x200

const static uint8_t FONT[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

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

    memcpy(chip_8.memory, FONT, sizeof(FONT));
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