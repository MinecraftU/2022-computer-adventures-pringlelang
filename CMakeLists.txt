cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial)

# add the executable
add_executable(pringlelang src/main.cpp src/parser.cpp src/source_code.cpp)
