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
include main/CMakeFiles/main.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include main/CMakeFiles/main.dir/compiler_depend.make

# Include the progress variables for this target.
include main/CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include main/CMakeFiles/main.dir/flags.make

main/CMakeFiles/main.dir/main.cpp.o: main/CMakeFiles/main.dir/flags.make
main/CMakeFiles/main.dir/main.cpp.o: /home/chordata/Documents/GitHub/iqtree2/main/main.cpp
main/CMakeFiles/main.dir/main.cpp.o: main/CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object main/CMakeFiles/main.dir/main.cpp.o"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT main/CMakeFiles/main.dir/main.cpp.o -MF CMakeFiles/main.dir/main.cpp.o.d -o CMakeFiles/main.dir/main.cpp.o -c /home/chordata/Documents/GitHub/iqtree2/main/main.cpp

main/CMakeFiles/main.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/main.dir/main.cpp.i"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chordata/Documents/GitHub/iqtree2/main/main.cpp > CMakeFiles/main.dir/main.cpp.i

main/CMakeFiles/main.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/main.dir/main.cpp.s"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chordata/Documents/GitHub/iqtree2/main/main.cpp -o CMakeFiles/main.dir/main.cpp.s

main/CMakeFiles/main.dir/phyloanalysis.cpp.o: main/CMakeFiles/main.dir/flags.make
main/CMakeFiles/main.dir/phyloanalysis.cpp.o: /home/chordata/Documents/GitHub/iqtree2/main/phyloanalysis.cpp
main/CMakeFiles/main.dir/phyloanalysis.cpp.o: main/CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object main/CMakeFiles/main.dir/phyloanalysis.cpp.o"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT main/CMakeFiles/main.dir/phyloanalysis.cpp.o -MF CMakeFiles/main.dir/phyloanalysis.cpp.o.d -o CMakeFiles/main.dir/phyloanalysis.cpp.o -c /home/chordata/Documents/GitHub/iqtree2/main/phyloanalysis.cpp

main/CMakeFiles/main.dir/phyloanalysis.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/main.dir/phyloanalysis.cpp.i"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chordata/Documents/GitHub/iqtree2/main/phyloanalysis.cpp > CMakeFiles/main.dir/phyloanalysis.cpp.i

main/CMakeFiles/main.dir/phyloanalysis.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/main.dir/phyloanalysis.cpp.s"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chordata/Documents/GitHub/iqtree2/main/phyloanalysis.cpp -o CMakeFiles/main.dir/phyloanalysis.cpp.s

main/CMakeFiles/main.dir/phylotesting.cpp.o: main/CMakeFiles/main.dir/flags.make
main/CMakeFiles/main.dir/phylotesting.cpp.o: /home/chordata/Documents/GitHub/iqtree2/main/phylotesting.cpp
main/CMakeFiles/main.dir/phylotesting.cpp.o: main/CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object main/CMakeFiles/main.dir/phylotesting.cpp.o"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT main/CMakeFiles/main.dir/phylotesting.cpp.o -MF CMakeFiles/main.dir/phylotesting.cpp.o.d -o CMakeFiles/main.dir/phylotesting.cpp.o -c /home/chordata/Documents/GitHub/iqtree2/main/phylotesting.cpp

main/CMakeFiles/main.dir/phylotesting.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/main.dir/phylotesting.cpp.i"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chordata/Documents/GitHub/iqtree2/main/phylotesting.cpp > CMakeFiles/main.dir/phylotesting.cpp.i

main/CMakeFiles/main.dir/phylotesting.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/main.dir/phylotesting.cpp.s"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chordata/Documents/GitHub/iqtree2/main/phylotesting.cpp -o CMakeFiles/main.dir/phylotesting.cpp.s

main/CMakeFiles/main.dir/treetesting.cpp.o: main/CMakeFiles/main.dir/flags.make
main/CMakeFiles/main.dir/treetesting.cpp.o: /home/chordata/Documents/GitHub/iqtree2/main/treetesting.cpp
main/CMakeFiles/main.dir/treetesting.cpp.o: main/CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object main/CMakeFiles/main.dir/treetesting.cpp.o"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT main/CMakeFiles/main.dir/treetesting.cpp.o -MF CMakeFiles/main.dir/treetesting.cpp.o.d -o CMakeFiles/main.dir/treetesting.cpp.o -c /home/chordata/Documents/GitHub/iqtree2/main/treetesting.cpp

main/CMakeFiles/main.dir/treetesting.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/main.dir/treetesting.cpp.i"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chordata/Documents/GitHub/iqtree2/main/treetesting.cpp > CMakeFiles/main.dir/treetesting.cpp.i

main/CMakeFiles/main.dir/treetesting.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/main.dir/treetesting.cpp.s"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chordata/Documents/GitHub/iqtree2/main/treetesting.cpp -o CMakeFiles/main.dir/treetesting.cpp.s

main/CMakeFiles/main.dir/timetree.cpp.o: main/CMakeFiles/main.dir/flags.make
main/CMakeFiles/main.dir/timetree.cpp.o: /home/chordata/Documents/GitHub/iqtree2/main/timetree.cpp
main/CMakeFiles/main.dir/timetree.cpp.o: main/CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object main/CMakeFiles/main.dir/timetree.cpp.o"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT main/CMakeFiles/main.dir/timetree.cpp.o -MF CMakeFiles/main.dir/timetree.cpp.o.d -o CMakeFiles/main.dir/timetree.cpp.o -c /home/chordata/Documents/GitHub/iqtree2/main/timetree.cpp

main/CMakeFiles/main.dir/timetree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/main.dir/timetree.cpp.i"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chordata/Documents/GitHub/iqtree2/main/timetree.cpp > CMakeFiles/main.dir/timetree.cpp.i

main/CMakeFiles/main.dir/timetree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/main.dir/timetree.cpp.s"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chordata/Documents/GitHub/iqtree2/main/timetree.cpp -o CMakeFiles/main.dir/timetree.cpp.s

main/CMakeFiles/main.dir/alisim.cpp.o: main/CMakeFiles/main.dir/flags.make
main/CMakeFiles/main.dir/alisim.cpp.o: /home/chordata/Documents/GitHub/iqtree2/main/alisim.cpp
main/CMakeFiles/main.dir/alisim.cpp.o: main/CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object main/CMakeFiles/main.dir/alisim.cpp.o"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT main/CMakeFiles/main.dir/alisim.cpp.o -MF CMakeFiles/main.dir/alisim.cpp.o.d -o CMakeFiles/main.dir/alisim.cpp.o -c /home/chordata/Documents/GitHub/iqtree2/main/alisim.cpp

main/CMakeFiles/main.dir/alisim.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/main.dir/alisim.cpp.i"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chordata/Documents/GitHub/iqtree2/main/alisim.cpp > CMakeFiles/main.dir/alisim.cpp.i

main/CMakeFiles/main.dir/alisim.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/main.dir/alisim.cpp.s"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chordata/Documents/GitHub/iqtree2/main/alisim.cpp -o CMakeFiles/main.dir/alisim.cpp.s

main/CMakeFiles/main.dir/terraceanalysis.cpp.o: main/CMakeFiles/main.dir/flags.make
main/CMakeFiles/main.dir/terraceanalysis.cpp.o: /home/chordata/Documents/GitHub/iqtree2/main/terraceanalysis.cpp
main/CMakeFiles/main.dir/terraceanalysis.cpp.o: main/CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object main/CMakeFiles/main.dir/terraceanalysis.cpp.o"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT main/CMakeFiles/main.dir/terraceanalysis.cpp.o -MF CMakeFiles/main.dir/terraceanalysis.cpp.o.d -o CMakeFiles/main.dir/terraceanalysis.cpp.o -c /home/chordata/Documents/GitHub/iqtree2/main/terraceanalysis.cpp

main/CMakeFiles/main.dir/terraceanalysis.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/main.dir/terraceanalysis.cpp.i"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chordata/Documents/GitHub/iqtree2/main/terraceanalysis.cpp > CMakeFiles/main.dir/terraceanalysis.cpp.i

main/CMakeFiles/main.dir/terraceanalysis.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/main.dir/terraceanalysis.cpp.s"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chordata/Documents/GitHub/iqtree2/main/terraceanalysis.cpp -o CMakeFiles/main.dir/terraceanalysis.cpp.s

# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/main.cpp.o" \
"CMakeFiles/main.dir/phyloanalysis.cpp.o" \
"CMakeFiles/main.dir/phylotesting.cpp.o" \
"CMakeFiles/main.dir/treetesting.cpp.o" \
"CMakeFiles/main.dir/timetree.cpp.o" \
"CMakeFiles/main.dir/alisim.cpp.o" \
"CMakeFiles/main.dir/terraceanalysis.cpp.o"

# External object files for target main
main_EXTERNAL_OBJECTS =

main/libmain.a: main/CMakeFiles/main.dir/main.cpp.o
main/libmain.a: main/CMakeFiles/main.dir/phyloanalysis.cpp.o
main/libmain.a: main/CMakeFiles/main.dir/phylotesting.cpp.o
main/libmain.a: main/CMakeFiles/main.dir/treetesting.cpp.o
main/libmain.a: main/CMakeFiles/main.dir/timetree.cpp.o
main/libmain.a: main/CMakeFiles/main.dir/alisim.cpp.o
main/libmain.a: main/CMakeFiles/main.dir/terraceanalysis.cpp.o
main/libmain.a: main/CMakeFiles/main.dir/build.make
main/libmain.a: main/CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX static library libmain.a"
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && $(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean_target.cmake
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
main/CMakeFiles/main.dir/build: main/libmain.a
.PHONY : main/CMakeFiles/main.dir/build

main/CMakeFiles/main.dir/clean:
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main && $(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean.cmake
.PHONY : main/CMakeFiles/main.dir/clean

main/CMakeFiles/main.dir/depend:
	cd /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chordata/Documents/GitHub/iqtree2 /home/chordata/Documents/GitHub/iqtree2/main /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main /home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main/CMakeFiles/main.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : main/CMakeFiles/main.dir/depend

