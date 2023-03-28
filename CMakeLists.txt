cmake_minimum_required(VERSION 3.8)

project(first)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(main window.cpp)
target_link_libraries(main ${SDL2_LIBRARIES})
