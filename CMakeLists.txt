cmake_minimum_required(VERSION 3.15)
project(project)

set(CMAKE_CXX_STANDARD 14)

add_executable(project main.cpp footballField.h footballField.cpp UAV.cpp UAV.h Bitmap.h)
FIND_LIBRARY(GLUT_LIBRARY GLUT)
FIND_LIBRARY(OpenGL_LIBRARY OpenGL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -framework GLUT -framework OpenGL") # compiler version and compiler flags.
