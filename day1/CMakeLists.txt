cmake_minimum_required(VERSION 3.00)
project(aoc)

add_executable(aoc main.cpp)
set_property(TARGET aoc PROPERTY CXX_STANDARD 23)

add_executable(aoc_td main_td.cpp)
set_property(TARGET aoc_td PROPERTY CXX_STANDARD 23)
