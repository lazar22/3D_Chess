cmake_minimum_required(VERSION 3.14)

project(3D_Chess)

find_package(SDL2 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)

add_executable(chess src/main.cpp
        include/shaders.h)

target_include_directories(chess PRIVATE include)

target_link_libraries(chess
        PRIVATE
        SDL2::SDL2main
        SDL2::SDL2
        GLEW::GLEW
)
