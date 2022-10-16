cmake_minimum_required(VERSION 3.21)
project(proj4_avl)

set(CMAKE_CXX_STANDARD 14)

add_executable(proj4_avl driver-proj4.cpp avl-tree-prof-proj4.h
        avl-tree-student-proj4.h)
