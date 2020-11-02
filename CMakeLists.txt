cmake_minimum_required(VERSION 3.17)
project(generic_topo_gen)

set(CMAKE_CXX_STANDARD 17)

add_executable(generic_topo_gen main.cpp generic_topo_gen.cpp generic_topo_gen.h)