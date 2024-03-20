cmake_minimum_required(VERSION 3.25)
project(project1)

set(CMAKE_CXX_STANDARD 17)

add_executable(project1 main.cpp stropt.h stropt.cpp timeopt.h timeopt.cpp fileopt.h fileopt.cpp logbuild.h logbuild.cpp socket.h socket.cpp)
