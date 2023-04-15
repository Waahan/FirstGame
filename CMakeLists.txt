cmake_minimum_required(VERSION 3.8)

project(first)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

message(${SDL2_DIR})

add_executable(main main.cpp player.cpp visuals.cpp )
target_link_libraries(main ${SDL2_LIBRARIES})
target_link_libraries(main /usr/lib/x86_64-linux-gnu/libSDL2_image-2.0.so.0)

#If you are on a different computer than the raspberry pi uncomment and add these lines and comment the target link libararies above 
#There is also sudo apt-file list libsdl2-image-2.0-0 to get the path 
#find_package(SDL2_image REQUIRED)
#target_link_libraries(main ${SDL2_LIBRARIES} SDL2_image::SDL2_image)
