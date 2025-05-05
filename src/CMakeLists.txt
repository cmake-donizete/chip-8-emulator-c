subdirs(display lifecycle sound)

add_library(emulator INTERFACE)
target_link_libraries(emulator INTERFACE lifecycle display)