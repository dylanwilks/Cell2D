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
CMAKE_SOURCE_DIR = "/home/altar/Desktop/Projects/Cell Library"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/altar/Desktop/Projects/Cell Library"

# Include any dependencies generated for this target.
include CMakeFiles/test.elf.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test.elf.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test.elf.dir/flags.make

CMakeFiles/test.elf.dir/src/c2World.cpp.o: CMakeFiles/test.elf.dir/flags.make
CMakeFiles/test.elf.dir/src/c2World.cpp.o: src/c2World.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/altar/Desktop/Projects/Cell Library/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test.elf.dir/src/c2World.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test.elf.dir/src/c2World.cpp.o -c "/home/altar/Desktop/Projects/Cell Library/src/c2World.cpp"

CMakeFiles/test.elf.dir/src/c2World.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.elf.dir/src/c2World.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/altar/Desktop/Projects/Cell Library/src/c2World.cpp" > CMakeFiles/test.elf.dir/src/c2World.cpp.i

CMakeFiles/test.elf.dir/src/c2World.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.elf.dir/src/c2World.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/altar/Desktop/Projects/Cell Library/src/c2World.cpp" -o CMakeFiles/test.elf.dir/src/c2World.cpp.s

CMakeFiles/test.elf.dir/src/c2Collisions.cpp.o: CMakeFiles/test.elf.dir/flags.make
CMakeFiles/test.elf.dir/src/c2Collisions.cpp.o: src/c2Collisions.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/altar/Desktop/Projects/Cell Library/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/test.elf.dir/src/c2Collisions.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test.elf.dir/src/c2Collisions.cpp.o -c "/home/altar/Desktop/Projects/Cell Library/src/c2Collisions.cpp"

CMakeFiles/test.elf.dir/src/c2Collisions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.elf.dir/src/c2Collisions.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/altar/Desktop/Projects/Cell Library/src/c2Collisions.cpp" > CMakeFiles/test.elf.dir/src/c2Collisions.cpp.i

CMakeFiles/test.elf.dir/src/c2Collisions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.elf.dir/src/c2Collisions.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/altar/Desktop/Projects/Cell Library/src/c2Collisions.cpp" -o CMakeFiles/test.elf.dir/src/c2Collisions.cpp.s

CMakeFiles/test.elf.dir/src/c2Cell.cpp.o: CMakeFiles/test.elf.dir/flags.make
CMakeFiles/test.elf.dir/src/c2Cell.cpp.o: src/c2Cell.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/altar/Desktop/Projects/Cell Library/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/test.elf.dir/src/c2Cell.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test.elf.dir/src/c2Cell.cpp.o -c "/home/altar/Desktop/Projects/Cell Library/src/c2Cell.cpp"

CMakeFiles/test.elf.dir/src/c2Cell.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.elf.dir/src/c2Cell.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/altar/Desktop/Projects/Cell Library/src/c2Cell.cpp" > CMakeFiles/test.elf.dir/src/c2Cell.cpp.i

CMakeFiles/test.elf.dir/src/c2Cell.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.elf.dir/src/c2Cell.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/altar/Desktop/Projects/Cell Library/src/c2Cell.cpp" -o CMakeFiles/test.elf.dir/src/c2Cell.cpp.s

CMakeFiles/test.elf.dir/src/testing.cpp.o: CMakeFiles/test.elf.dir/flags.make
CMakeFiles/test.elf.dir/src/testing.cpp.o: src/testing.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/altar/Desktop/Projects/Cell Library/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/test.elf.dir/src/testing.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test.elf.dir/src/testing.cpp.o -c "/home/altar/Desktop/Projects/Cell Library/src/testing.cpp"

CMakeFiles/test.elf.dir/src/testing.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.elf.dir/src/testing.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/altar/Desktop/Projects/Cell Library/src/testing.cpp" > CMakeFiles/test.elf.dir/src/testing.cpp.i

CMakeFiles/test.elf.dir/src/testing.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.elf.dir/src/testing.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/altar/Desktop/Projects/Cell Library/src/testing.cpp" -o CMakeFiles/test.elf.dir/src/testing.cpp.s

# Object files for target test.elf
test_elf_OBJECTS = \
"CMakeFiles/test.elf.dir/src/c2World.cpp.o" \
"CMakeFiles/test.elf.dir/src/c2Collisions.cpp.o" \
"CMakeFiles/test.elf.dir/src/c2Cell.cpp.o" \
"CMakeFiles/test.elf.dir/src/testing.cpp.o"

# External object files for target test.elf
test_elf_EXTERNAL_OBJECTS =

test.elf: CMakeFiles/test.elf.dir/src/c2World.cpp.o
test.elf: CMakeFiles/test.elf.dir/src/c2Collisions.cpp.o
test.elf: CMakeFiles/test.elf.dir/src/c2Cell.cpp.o
test.elf: CMakeFiles/test.elf.dir/src/testing.cpp.o
test.elf: CMakeFiles/test.elf.dir/build.make
test.elf: CMakeFiles/test.elf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/altar/Desktop/Projects/Cell Library/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable test.elf"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test.elf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test.elf.dir/build: test.elf

.PHONY : CMakeFiles/test.elf.dir/build

CMakeFiles/test.elf.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test.elf.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test.elf.dir/clean

CMakeFiles/test.elf.dir/depend:
	cd "/home/altar/Desktop/Projects/Cell Library" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/altar/Desktop/Projects/Cell Library" "/home/altar/Desktop/Projects/Cell Library" "/home/altar/Desktop/Projects/Cell Library" "/home/altar/Desktop/Projects/Cell Library" "/home/altar/Desktop/Projects/Cell Library/CMakeFiles/test.elf.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/test.elf.dir/depend
