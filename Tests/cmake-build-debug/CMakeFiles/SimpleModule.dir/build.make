# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /home/seb/bin/cmake-3.13.1-Linux-x86_64/bin/cmake

# The command to remove a file.
RM = /home/seb/bin/cmake-3.13.1-Linux-x86_64/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/seb/zia/api/Tests

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/seb/zia/api/Tests/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/SimpleModule.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SimpleModule.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SimpleModule.dir/flags.make

CMakeFiles/SimpleModule.dir/Loader/SimpleModule.cpp.o: CMakeFiles/SimpleModule.dir/flags.make
CMakeFiles/SimpleModule.dir/Loader/SimpleModule.cpp.o: ../Loader/SimpleModule.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/seb/zia/api/Tests/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SimpleModule.dir/Loader/SimpleModule.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SimpleModule.dir/Loader/SimpleModule.cpp.o -c /home/seb/zia/api/Tests/Loader/SimpleModule.cpp

CMakeFiles/SimpleModule.dir/Loader/SimpleModule.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimpleModule.dir/Loader/SimpleModule.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/seb/zia/api/Tests/Loader/SimpleModule.cpp > CMakeFiles/SimpleModule.dir/Loader/SimpleModule.cpp.i

CMakeFiles/SimpleModule.dir/Loader/SimpleModule.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimpleModule.dir/Loader/SimpleModule.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/seb/zia/api/Tests/Loader/SimpleModule.cpp -o CMakeFiles/SimpleModule.dir/Loader/SimpleModule.cpp.s

# Object files for target SimpleModule
SimpleModule_OBJECTS = \
"CMakeFiles/SimpleModule.dir/Loader/SimpleModule.cpp.o"

# External object files for target SimpleModule
SimpleModule_EXTERNAL_OBJECTS =

libSimpleModule.so: CMakeFiles/SimpleModule.dir/Loader/SimpleModule.cpp.o
libSimpleModule.so: CMakeFiles/SimpleModule.dir/build.make
libSimpleModule.so: CMakeFiles/SimpleModule.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/seb/zia/api/Tests/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libSimpleModule.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SimpleModule.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SimpleModule.dir/build: libSimpleModule.so

.PHONY : CMakeFiles/SimpleModule.dir/build

CMakeFiles/SimpleModule.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SimpleModule.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SimpleModule.dir/clean

CMakeFiles/SimpleModule.dir/depend:
	cd /home/seb/zia/api/Tests/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/seb/zia/api/Tests /home/seb/zia/api/Tests /home/seb/zia/api/Tests/cmake-build-debug /home/seb/zia/api/Tests/cmake-build-debug /home/seb/zia/api/Tests/cmake-build-debug/CMakeFiles/SimpleModule.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SimpleModule.dir/depend

