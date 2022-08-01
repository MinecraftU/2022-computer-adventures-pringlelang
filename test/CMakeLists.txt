set(CATCH_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rd_party ${CMAKE_SOURCE_DIR}/test)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Add test executable
add_executable (unit_tests tests.cpp)
target_link_libraries(unit_tests PRIVATE Catch coreLib)
target_compile_options(unit_tests PUBLIC -std=c++11 -g)

# Enable unit test.
include(CTest)
add_test(NAME unit_tests COMMAND unit_tests)

