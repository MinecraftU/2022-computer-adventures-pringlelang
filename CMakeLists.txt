cmake_minimum_required (VERSION 3.16)
project (pringlelang)

add_executable(pringlelang src/main.cpp)

target_include_directories(pringlelang PUBLIC inc)
target_link_libraries(pringlelang coreLib)

add_subdirectory(src)
add_subdirectory(test)

target_compile_options(pringlelang PUBLIC -std=c++11 -g)

include(CTest)
enable_testing()
