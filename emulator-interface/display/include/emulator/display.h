#include <stdint.h>

void emulator_display_clear();

void emulator_display_draw_pixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b);

void emulator_display_refresh();