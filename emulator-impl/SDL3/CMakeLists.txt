add_library(emulator-sdl3 impl.c)

set(SDL_SHARED OFF)
set(SDL_STATIC ON)

target_link_libraries(emulator-sdl3 PUBLIC emulator SDL3::SDL3)
target_compile_definitions(
    emulator-sdl3
    PRIVATE
    WINDOW_WIDTH=320
    WINDOW_HEIGHT=240)