cmake_minimum_required(VERSION 3.10.0)
project(chip-8-emulator-c VERSION 0.1.0 LANGUAGES C)

subdirs(external impl src)

add_executable(chip-8-emulator-c main.c)
target_link_libraries(chip-8-emulator-c emulator emulator-sdl3)