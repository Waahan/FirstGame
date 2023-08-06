cmake_minimum_required(VERSION 3.8)

project(first CXX)

set(CMAKE_CXX_STANDARD 20)

option(DEBUGGING "Compile with debug flags" OFF)

if(DEBUGGING)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -Wextra")
else()
    set(CMAKE_BUILD_TYPE Release)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_library(
    SDL_IMAGE_LIB
    
    NAMES libsdl2-image-2.0-0 libSDL2_image-2.0.so.0 libSDL2_image-2.0.so.0.2.3

    PATHS 
    /usr/lib
    /usr/lib64
    /usr/local/lib
    /usr/local/lib64
)

find_library(
    SDL_TTF_LIB

    NAMES libsdl2-ttf-2.0-0 libSDL2_ttf-2.0.so.0 libSDL2_ttf-2.0.so.0.14.1

    PATHS 
    /usr/lib
    /usr/lib64
    /usr/local/lib
    /usr/local/lib64
)

find_library(
    SDL_MIX_LIB

    NAMES libsdl2-mixer-2.0-0 libSDL2_mixer-2.0.so.0 libSDL2_mixer-2.0.so.0.2.2

    PATHS 
    /usr/lib
    /usr/lib64
    /usr/local/lib
    /usr/local/lib64
)

add_executable(main main.cpp player.cpp visuals.cpp)
target_link_libraries(main ${SDL2_LIBRARIES} ${SDL_IMAGE_LIB} ${SDL_TTF_LIB} ${SDL_MIX_LIB} Threads::Threads )
