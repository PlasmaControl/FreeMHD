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
include thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/compiler_depend.make

# Include the progress variables for this target.
include thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/progress.make

# Include the compile flags for this target's objects.
include thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/flags.make

thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/cmmulticast.c.o: thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/flags.make
thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/cmmulticast.c.o: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/EVPath/EVPath/cmmulticast.c
thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/cmmulticast.c.o: thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/cmmulticast.c.o"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/EVPath/EVPath && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/cmmulticast.c.o -MF CMakeFiles/cmmulticast.dir/cmmulticast.c.o.d -o CMakeFiles/cmmulticast.dir/cmmulticast.c.o -c /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/EVPath/EVPath/cmmulticast.c

thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/cmmulticast.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cmmulticast.dir/cmmulticast.c.i"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/EVPath/EVPath && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/EVPath/EVPath/cmmulticast.c > CMakeFiles/cmmulticast.dir/cmmulticast.c.i

thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/cmmulticast.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cmmulticast.dir/cmmulticast.c.s"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/EVPath/EVPath && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/EVPath/EVPath/cmmulticast.c -o CMakeFiles/cmmulticast.dir/cmmulticast.c.s

# Object files for target cmmulticast
cmmulticast_OBJECTS = \
"CMakeFiles/cmmulticast.dir/cmmulticast.c.o"

# External object files for target cmmulticast
cmmulticast_EXTERNAL_OBJECTS =

thirdparty/EVPath/EVPath/lib64/libadios2_cmmulticast.so: thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/cmmulticast.c.o
thirdparty/EVPath/EVPath/lib64/libadios2_cmmulticast.so: thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/build.make
thirdparty/EVPath/EVPath/lib64/libadios2_cmmulticast.so: lib64/libadios2_atl.so.2.2.1
thirdparty/EVPath/EVPath/lib64/libadios2_cmmulticast.so: thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared module lib64/libadios2_cmmulticast.so"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/EVPath/EVPath && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cmmulticast.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/build: thirdparty/EVPath/EVPath/lib64/libadios2_cmmulticast.so
.PHONY : thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/build

thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/clean:
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/EVPath/EVPath && $(CMAKE_COMMAND) -P CMakeFiles/cmmulticast.dir/cmake_clean.cmake
.PHONY : thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/clean

thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/depend:
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1 /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/EVPath/EVPath /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1 /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/EVPath/EVPath /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : thirdparty/EVPath/EVPath/CMakeFiles/cmmulticast.dir/depend

