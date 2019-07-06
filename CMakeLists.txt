cmake_minimum_required(VERSION 3.13)
project(chip_8)

set(CMAKE_CXX_STANDARD 17)

add_executable(chip_8 main.cpp chip_8.cc chip_8.hh)

target_link_libraries(chip_8 sfml-graphics sfml-system sfml-window)
