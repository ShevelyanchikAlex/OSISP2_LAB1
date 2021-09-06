cmake_minimum_required(VERSION 3.20)
project(OSISP_LAB1)

set(CMAKE_CXX_STANDARD 14)

add_executable(OSISP_LAB1 main.cpp window/Window.cpp window/Window.h rect/Rect.cpp rect/Rect.h)
