add_library(coreLib token.cpp interpreter.cpp sourcecode.cpp)
target_include_directories(coreLib PUBLIC ${CMAKE_SOURCE_DIR}/inc)
target_compile_options(coreLib PUBLIC -std=c++11 -g)