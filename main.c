#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include <emulator/lifecycle.h>
#include <emulator/display.h>

#define CHIP_8_MEMORY_SIZE      0x1000
#define CHIP_8_FONT_START       0x0000
#define CHIP_8_PROGRAM_START    0x0200
#define CHIP_8_DISPLAY_START    0x0F00
#define CHIP_8_DISPLAY_SIZE     0x00FF

#define CHIP_8_OPCODE_CLEAR     0x00E0  // 0x00E0
#define CHIP_8_OPCODE_RET       0x00EE  // 0x00EE
#define CHIP_8_OPCODE_JUMP      0x1000  // 0x1NNN
#define CHIP_8_OPCODE_CALL      0x2000  // 0x2NNN
#define CHIP_8_OPCODE_EQNN      0x3000  // 0x3XNN
#define CHIP_8_OPCODE_NENN      0x4000  // 0x4XNN
#define CHIP_8_OPCODE_EQXY      0x5000  // 0x5XN0
#define CHIP_8_OPCODE_MOVX      0x6000  // 0x6XNN
#define CHIP_8_OPCODE_INCX      0x7000  // 0x7XNN
#define CHIP_8_OPCODE_MOVR      0x8000  // 0x8XY0
#define CHIP_8_OPCODE_MOVI      0xA000  // 0xANNN
#define CHIP_8_OPCODE_DISP      0xD000  // 0xDXYN

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
    uint8_t memory[CHIP_8_MEMORY_SIZE];
    uint16_t pc;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t rg[16];
    uint16_t I;
    uint8_t *program;
    uint8_t *display;
    uint8_t *font;
};

static struct chip_8_t chip_8;

static inline void chip_8_memory_init()
{
    memset(&chip_8, 0, sizeof(chip_8));

    chip_8.pc = CHIP_8_PROGRAM_START;
    chip_8.program = chip_8.memory + CHIP_8_PROGRAM_START;
    chip_8.display = chip_8.memory + CHIP_8_DISPLAY_START;
    chip_8.font = chip_8.memory + CHIP_8_FONT_START;

    memcpy(chip_8.font, FONT, sizeof(FONT));
}

void emulator_lifecycle_init(void **appstate, int argc, char *argv[])
{
    chip_8_memory_init();

    int fd;
    size_t readed;

    fd = open("../rom/ibmlogo.ch8", O_RDONLY);
    readed = read(fd, chip_8.program, CHIP_8_MEMORY_SIZE - CHIP_8_PROGRAM_START);

    for (size_t offset = 0; offset < readed; offset += 2)
    {
        if (offset % 0x10 == 0)
        {
            printf("\n");
        }
        printf("%02x", chip_8.program[offset]);
        printf("%02x", chip_8.program[offset + 1]);
        printf(" ");
    }

    close(fd);
}

void emulator_lifecycle_iterate(void *appstate)
{
    uint8_t byte_2 = chip_8.memory[chip_8.pc++];
    uint8_t byte_1 = chip_8.memory[chip_8.pc++];
    uint16_t opcode = (byte_2 << 8) | byte_1;

    switch (opcode)
    {
        case CHIP_8_OPCODE_CLEAR: {
            memset(chip_8.display, 0, CHIP_8_DISPLAY_SIZE);
            break;
        }

        case CHIP_8_OPCODE_RET: {
            chip_8.pc = chip_8.stack[--chip_8.sp];
            break;
        }
    }

    switch ((opcode & 0xF000))
    {
        case CHIP_8_OPCODE_JUMP: {
            uint16_t NNN = opcode & 0x0FFF;
            chip_8.pc = NNN;
            break;
        }

        case CHIP_8_OPCODE_CALL: {
            uint16_t NNN = opcode & 0x0FFF;
            chip_8.stack[chip_8.sp++] = chip_8.pc;
            chip_8.pc = NNN;
            break;
        }

        case CHIP_8_OPCODE_EQNN: {
            uint8_t x = byte_2 & 0x0F;
            if (chip_8.rg[x] == byte_1) chip_8.pc += 2;
            break;
        }

        case CHIP_8_OPCODE_NENN: {
            uint8_t x = byte_2 & 0x0F;
            if (chip_8.rg[x] != byte_1) chip_8.pc += 2;
            break;
        }

        case CHIP_8_OPCODE_EQXY: {
            uint8_t x = byte_2 & 0x0F;
            uint8_t y = byte_1 >> 4;
            if (chip_8.rg[x] == chip_8.rg[y]) chip_8.pc += 2;
            break;
        }

        case CHIP_8_OPCODE_MOVX: {
            uint8_t x = byte_2 & 0x0F;
            chip_8.rg[x] = byte_1;
            break;
        }

        case CHIP_8_OPCODE_INCX: {
            uint8_t x = byte_2 & 0x0F;
            chip_8.rg[x] += byte_1;
            break;
        }

        case CHIP_8_OPCODE_MOVR: {
            uint8_t x = byte_2 & 0x0F;
            uint8_t y = byte_1 >> 4;
            chip_8.rg[x] = chip_8.rg[y];
            break;
        }

        case CHIP_8_OPCODE_MOVI: {
            chip_8.I = opcode & 0x0FFF;
            break;
        }

        case CHIP_8_OPCODE_DISP: {
            uint8_t x = byte_2 & 0x0F;
            uint8_t y = byte_1 >> 4;
            uint8_t n = byte_1 & 0x0F;

            uint8_t xv = chip_8.rg[x] % RENDER_WIDTH;
            uint8_t yv = chip_8.rg[y] % RENDER_HEIGHT;
 
            for (uint8_t i = 0; i < n; i++)
            {
                uint8_t byte = chip_8.memory[chip_8.I + i];
                uint8_t *row = chip_8.display + ((yv + i) * (RENDER_WIDTH / 8));
                uint8_t div = xv / 8;
                uint8_t rem = xv - (div * 8);
                row[div] ^= byte >> rem;
                row[div + 1] ^= byte << (8 - rem);
            }
            break;
        }
    }

    emulator_display_clear();

    for (uint8_t y = 0; y < RENDER_HEIGHT; y++)
    {
        uint8_t index = y * (RENDER_WIDTH / 8);
        uint8_t *row = chip_8.display + index;

        for (uint8_t x = 0; x < RENDER_WIDTH; x++)
        {
            uint8_t div = x / 8;
            uint8_t mod = 0x80 >> (x % 8);
            uint8_t res = row[div] & mod;
            if (res)
            {
                emulator_display_draw_pixel(x, y, 0xFF, 0xFF, 0xFF);
            } else {
                emulator_display_draw_pixel(x, y, 0, 0, 0);
            }
        }
    }

    emulator_display_refresh();
}

void emulator_lifecycle_quit(void *appstate)
{
}