add_library(coreLib parser.cpp source_code.cpp)
target_include_directories(coreLib PUBLIC ${CMAKE_SOURCE_DIR}/inc)
target_compile_options(coreLib PUBLIC -std=c++11 -g)