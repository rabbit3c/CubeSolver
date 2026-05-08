cmake_minimum_required(VERSION 3.10.0)
project(CubeSolver VERSION 0.1.0 LANGUAGES C CXX)

add_executable(CubeSolver main.cpp side.cpp colors.cpp cube.cpp scrambler.cpp)

include(CTest)
enable_testing()

