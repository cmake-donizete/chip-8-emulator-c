cmake_minimum_required(VERSION 3.10.0)
project(chip-8-emulator-c VERSION 0.1.0 LANGUAGES C)

add_executable(chip-8-emulator-c main.c)

include(CTest)
enable_testing()

