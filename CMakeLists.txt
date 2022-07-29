cmake_minimum_required(VERSION 3.16)
project(pringlelang)

add_executable(pringlelang main.cpp)

target_compile_options(pringlelang PUBLIC -std=c++11 -g)