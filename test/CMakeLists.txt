#set(CATCH_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rd_party ${CMAKE_SOURCE_DIR}/test)
#add_library(Catch INTERFACE)
#target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Add test executable
add_executable (unit_tests catch_config.cpp test_1.cpp)
target_link_libraries(unit_tests PRIVATE Catch coreLib)
target_compile_options(unit_tests PUBLIC -std=c++11 -g)

# Enable unit test.
include(CTest)

# Catch (v2.13.2) automatic test registration
# this is pretty old and not really recommended anymore but it works for now
# see: https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#parseandaddcatchtestscmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../3rd_party/catch_cmake_modules")
include(Catch2)
catch_discover_tests(unit_tests)


