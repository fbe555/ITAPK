# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_SOURCE_DIR = /media/winlin/apk/ex1/shared_ptr

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /media/winlin/apk/ex1/shared_ptr/build

# Include any dependencies generated for this target.
include CMakeFiles/shared_ptr.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/shared_ptr.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/shared_ptr.dir/flags.make

CMakeFiles/shared_ptr.dir/main.cpp.o: CMakeFiles/shared_ptr.dir/flags.make
CMakeFiles/shared_ptr.dir/main.cpp.o: ../main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /media/winlin/apk/ex1/shared_ptr/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/shared_ptr.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/shared_ptr.dir/main.cpp.o -c /media/winlin/apk/ex1/shared_ptr/main.cpp

CMakeFiles/shared_ptr.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/shared_ptr.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /media/winlin/apk/ex1/shared_ptr/main.cpp > CMakeFiles/shared_ptr.dir/main.cpp.i

CMakeFiles/shared_ptr.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/shared_ptr.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /media/winlin/apk/ex1/shared_ptr/main.cpp -o CMakeFiles/shared_ptr.dir/main.cpp.s

CMakeFiles/shared_ptr.dir/main.cpp.o.requires:
.PHONY : CMakeFiles/shared_ptr.dir/main.cpp.o.requires

CMakeFiles/shared_ptr.dir/main.cpp.o.provides: CMakeFiles/shared_ptr.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/shared_ptr.dir/build.make CMakeFiles/shared_ptr.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/shared_ptr.dir/main.cpp.o.provides

CMakeFiles/shared_ptr.dir/main.cpp.o.provides.build: CMakeFiles/shared_ptr.dir/main.cpp.o

# Object files for target shared_ptr
shared_ptr_OBJECTS = \
"CMakeFiles/shared_ptr.dir/main.cpp.o"

# External object files for target shared_ptr
shared_ptr_EXTERNAL_OBJECTS =

shared_ptr: CMakeFiles/shared_ptr.dir/main.cpp.o
shared_ptr: CMakeFiles/shared_ptr.dir/build.make
shared_ptr: CMakeFiles/shared_ptr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable shared_ptr"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/shared_ptr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/shared_ptr.dir/build: shared_ptr
.PHONY : CMakeFiles/shared_ptr.dir/build

CMakeFiles/shared_ptr.dir/requires: CMakeFiles/shared_ptr.dir/main.cpp.o.requires
.PHONY : CMakeFiles/shared_ptr.dir/requires

CMakeFiles/shared_ptr.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/shared_ptr.dir/cmake_clean.cmake
.PHONY : CMakeFiles/shared_ptr.dir/clean

CMakeFiles/shared_ptr.dir/depend:
	cd /media/winlin/apk/ex1/shared_ptr/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/winlin/apk/ex1/shared_ptr /media/winlin/apk/ex1/shared_ptr /media/winlin/apk/ex1/shared_ptr/build /media/winlin/apk/ex1/shared_ptr/build /media/winlin/apk/ex1/shared_ptr/build/CMakeFiles/shared_ptr.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/shared_ptr.dir/depend

