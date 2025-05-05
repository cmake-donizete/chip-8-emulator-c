add_library(emulator-sdl3 impl.c)

set(SDL_SHARED OFF)
set(SDL_STATIC ON)

target_link_libraries(emulator-sdl3 PUBLIC emulator SDL3::SDL3)