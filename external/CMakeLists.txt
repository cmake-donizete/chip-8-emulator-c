add_library(external INTERFACE)

add_subdirectory(SDL)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
target_link_libraries(external INTERFACE SDL3::SDL3)