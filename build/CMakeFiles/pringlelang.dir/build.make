# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jamesli/github_repos/pringlelang

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jamesli/github_repos/pringlelang/build

# Include any dependencies generated for this target.
include CMakeFiles/pringlelang.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/pringlelang.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pringlelang.dir/flags.make

CMakeFiles/pringlelang.dir/src/main.cpp.o: CMakeFiles/pringlelang.dir/flags.make
CMakeFiles/pringlelang.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jamesli/github_repos/pringlelang/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/pringlelang.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pringlelang.dir/src/main.cpp.o -c /home/jamesli/github_repos/pringlelang/src/main.cpp

CMakeFiles/pringlelang.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pringlelang.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jamesli/github_repos/pringlelang/src/main.cpp > CMakeFiles/pringlelang.dir/src/main.cpp.i

CMakeFiles/pringlelang.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pringlelang.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jamesli/github_repos/pringlelang/src/main.cpp -o CMakeFiles/pringlelang.dir/src/main.cpp.s

CMakeFiles/pringlelang.dir/src/parser.cpp.o: CMakeFiles/pringlelang.dir/flags.make
CMakeFiles/pringlelang.dir/src/parser.cpp.o: ../src/parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jamesli/github_repos/pringlelang/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/pringlelang.dir/src/parser.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pringlelang.dir/src/parser.cpp.o -c /home/jamesli/github_repos/pringlelang/src/parser.cpp

CMakeFiles/pringlelang.dir/src/parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pringlelang.dir/src/parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jamesli/github_repos/pringlelang/src/parser.cpp > CMakeFiles/pringlelang.dir/src/parser.cpp.i

CMakeFiles/pringlelang.dir/src/parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pringlelang.dir/src/parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jamesli/github_repos/pringlelang/src/parser.cpp -o CMakeFiles/pringlelang.dir/src/parser.cpp.s

CMakeFiles/pringlelang.dir/src/source_code.cpp.o: CMakeFiles/pringlelang.dir/flags.make
CMakeFiles/pringlelang.dir/src/source_code.cpp.o: ../src/source_code.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jamesli/github_repos/pringlelang/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/pringlelang.dir/src/source_code.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pringlelang.dir/src/source_code.cpp.o -c /home/jamesli/github_repos/pringlelang/src/source_code.cpp

CMakeFiles/pringlelang.dir/src/source_code.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pringlelang.dir/src/source_code.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jamesli/github_repos/pringlelang/src/source_code.cpp > CMakeFiles/pringlelang.dir/src/source_code.cpp.i

CMakeFiles/pringlelang.dir/src/source_code.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pringlelang.dir/src/source_code.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jamesli/github_repos/pringlelang/src/source_code.cpp -o CMakeFiles/pringlelang.dir/src/source_code.cpp.s

# Object files for target pringlelang
pringlelang_OBJECTS = \
"CMakeFiles/pringlelang.dir/src/main.cpp.o" \
"CMakeFiles/pringlelang.dir/src/parser.cpp.o" \
"CMakeFiles/pringlelang.dir/src/source_code.cpp.o"

# External object files for target pringlelang
pringlelang_EXTERNAL_OBJECTS =

pringlelang: CMakeFiles/pringlelang.dir/src/main.cpp.o
pringlelang: CMakeFiles/pringlelang.dir/src/parser.cpp.o
pringlelang: CMakeFiles/pringlelang.dir/src/source_code.cpp.o
pringlelang: CMakeFiles/pringlelang.dir/build.make
pringlelang: CMakeFiles/pringlelang.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jamesli/github_repos/pringlelang/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable pringlelang"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pringlelang.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pringlelang.dir/build: pringlelang

.PHONY : CMakeFiles/pringlelang.dir/build

CMakeFiles/pringlelang.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pringlelang.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pringlelang.dir/clean

CMakeFiles/pringlelang.dir/depend:
	cd /home/jamesli/github_repos/pringlelang/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jamesli/github_repos/pringlelang /home/jamesli/github_repos/pringlelang /home/jamesli/github_repos/pringlelang/build /home/jamesli/github_repos/pringlelang/build /home/jamesli/github_repos/pringlelang/build/CMakeFiles/pringlelang.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pringlelang.dir/depend
