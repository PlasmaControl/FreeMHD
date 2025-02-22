# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1

# Include any dependencies generated for this target.
include testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/compiler_depend.make

# Include the progress variables for this target.
include testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/progress.make

# Include the compile flags for this target's objects.
include testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/flags.make

testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/TestBPQuery.cpp.o: testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/flags.make
testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/TestBPQuery.cpp.o: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/performance/query/TestBPQuery.cpp
testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/TestBPQuery.cpp.o: testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/TestBPQuery.cpp.o"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/testing/adios2/performance/query && /usr/local/openmpi/4.1.0/gcc/bin/mpicxx $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/TestBPQuery.cpp.o -MF CMakeFiles/Test.Performance.Query.Serial.dir/TestBPQuery.cpp.o.d -o CMakeFiles/Test.Performance.Query.Serial.dir/TestBPQuery.cpp.o -c /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/performance/query/TestBPQuery.cpp

testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/TestBPQuery.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Test.Performance.Query.Serial.dir/TestBPQuery.cpp.i"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/testing/adios2/performance/query && /usr/local/openmpi/4.1.0/gcc/bin/mpicxx $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/performance/query/TestBPQuery.cpp > CMakeFiles/Test.Performance.Query.Serial.dir/TestBPQuery.cpp.i

testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/TestBPQuery.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Test.Performance.Query.Serial.dir/TestBPQuery.cpp.s"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/testing/adios2/performance/query && /usr/local/openmpi/4.1.0/gcc/bin/mpicxx $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/performance/query/TestBPQuery.cpp -o CMakeFiles/Test.Performance.Query.Serial.dir/TestBPQuery.cpp.s

# Object files for target Test.Performance.Query.Serial
Test_Performance_Query_Serial_OBJECTS = \
"CMakeFiles/Test.Performance.Query.Serial.dir/TestBPQuery.cpp.o"

# External object files for target Test.Performance.Query.Serial
Test_Performance_Query_Serial_EXTERNAL_OBJECTS =

bin/Test.Performance.Query.Serial: testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/TestBPQuery.cpp.o
bin/Test.Performance.Query.Serial: testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/build.make
bin/Test.Performance.Query.Serial: lib64/libadios2_cxx11.so.2.7.1
bin/Test.Performance.Query.Serial: lib64/libadios2_c.so.2.7.1
bin/Test.Performance.Query.Serial: lib64/libadios2_core.so.2.7.1
bin/Test.Performance.Query.Serial: lib/libgtest.a
bin/Test.Performance.Query.Serial: testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../../bin/Test.Performance.Query.Serial"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/testing/adios2/performance/query && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Test.Performance.Query.Serial.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/build: bin/Test.Performance.Query.Serial
.PHONY : testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/build

testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/clean:
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/testing/adios2/performance/query && $(CMAKE_COMMAND) -P CMakeFiles/Test.Performance.Query.Serial.dir/cmake_clean.cmake
.PHONY : testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/clean

testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/depend:
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1 /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/performance/query /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1 /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/testing/adios2/performance/query /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : testing/adios2/performance/query/CMakeFiles/Test.Performance.Query.Serial.dir/depend

