# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/291/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /snap/clion/291/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chordata/Documents/GitHub/iqtree2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug

# Include any dependencies generated for this target.
include phylo-yaml/CMakeFiles/phyloYAML.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include phylo-yaml/CMakeFiles/phyloYAML.dir/compiler_depend.make

# Include the progress variables for this target.
include phylo-yaml/CMakeFiles/phyloYAML.dir/progress.make

# Include the compile flags for this target's objects.
include phylo-yaml/CMakeFiles/phyloYAML.dir/flags.make

phylo-yaml/CMakeFiles/phyloYAML.dir/statespace.cpp.o: phylo-yaml/CMakeFiles/phyloYAML.dir/flags.make
phylo-yaml/CMakeFiles/phyloYAML.dir/statespace.cpp.o: /home/chordata/Documents/GitHub/iqtree2/phylo-yaml/statespace.cpp
phylo-yaml/CMakeFiles/phyloYAML.dir/statespace.cpp.o: phylo-yaml/CMakeFiles/phyloYAML.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object phylo-yaml/CMakeFiles/phyloYAML.dir/statespace.cpp.o"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/phylo-yaml && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT phylo-yaml/CMakeFiles/phyloYAML.dir/statespace.cpp.o -MF CMakeFiles/phyloYAML.dir/statespace.cpp.o.d -o CMakeFiles/phyloYAML.dir/statespace.cpp.o -c /home/chordata/Documents/GitHub/iqtree2/phylo-yaml/statespace.cpp

phylo-yaml/CMakeFiles/phyloYAML.dir/statespace.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/phyloYAML.dir/statespace.cpp.i"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/phylo-yaml && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chordata/Documents/GitHub/iqtree2/phylo-yaml/statespace.cpp > CMakeFiles/phyloYAML.dir/statespace.cpp.i

phylo-yaml/CMakeFiles/phyloYAML.dir/statespace.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/phyloYAML.dir/statespace.cpp.s"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/phylo-yaml && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chordata/Documents/GitHub/iqtree2/phylo-yaml/statespace.cpp -o CMakeFiles/phyloYAML.dir/statespace.cpp.s

# Object files for target phyloYAML
phyloYAML_OBJECTS = \
"CMakeFiles/phyloYAML.dir/statespace.cpp.o"

# External object files for target phyloYAML
phyloYAML_EXTERNAL_OBJECTS =

phylo-yaml/libphyloYAML.a: phylo-yaml/CMakeFiles/phyloYAML.dir/statespace.cpp.o
phylo-yaml/libphyloYAML.a: phylo-yaml/CMakeFiles/phyloYAML.dir/build.make
phylo-yaml/libphyloYAML.a: phylo-yaml/CMakeFiles/phyloYAML.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libphyloYAML.a"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/phylo-yaml && $(CMAKE_COMMAND) -P CMakeFiles/phyloYAML.dir/cmake_clean_target.cmake
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/phylo-yaml && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/phyloYAML.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
phylo-yaml/CMakeFiles/phyloYAML.dir/build: phylo-yaml/libphyloYAML.a
.PHONY : phylo-yaml/CMakeFiles/phyloYAML.dir/build

phylo-yaml/CMakeFiles/phyloYAML.dir/clean:
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/phylo-yaml && $(CMAKE_COMMAND) -P CMakeFiles/phyloYAML.dir/cmake_clean.cmake
.PHONY : phylo-yaml/CMakeFiles/phyloYAML.dir/clean

phylo-yaml/CMakeFiles/phyloYAML.dir/depend:
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chordata/Documents/GitHub/iqtree2 /home/chordata/Documents/GitHub/iqtree2/phylo-yaml /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/phylo-yaml /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/phylo-yaml/CMakeFiles/phyloYAML.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : phylo-yaml/CMakeFiles/phyloYAML.dir/depend

