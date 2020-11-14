cmake_minimum_required(VERSION 3.17)
project(generic_topo_gen)

set(CMAKE_CXX_STANDARD 17)

add_executable(generic_topo_gen src/main.cpp src/generic_topo_gen.cpp src/generic_topo_gen.h
        src/commandline_parser.cpp src/commandline_parser.h src/file_handler.cpp src/file_handler.h
        src/Topology.cpp src/Topology.h src/Node.cpp src/Node.h src/Link.cpp src/Link.h)