add_library(emulator-sdl3 impl.c)

target_link_libraries(emulator-sdl3 PUBLIC emulator SDL3::SDL3)

target_compile_definitions(
    emulator-sdl3
    PRIVATE
    WINDOW_WIDTH=320
    WINDOW_HEIGHT=240)

target_compile_definitions(
    emulator-sdl3
    PUBLIC
    RENDER_WIDTH=64
    RENDER_HEIGHT=32)