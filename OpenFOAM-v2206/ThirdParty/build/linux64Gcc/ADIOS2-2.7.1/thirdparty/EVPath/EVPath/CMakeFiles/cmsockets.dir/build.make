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
include thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/compiler_depend.make

# Include the progress variables for this target.
include thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/progress.make

# Include the compile flags for this target's objects.
include thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/flags.make

thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/cmsockets.c.o: thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/flags.make
thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/cmsockets.c.o: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/EVPath/EVPath/cmsockets.c
thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/cmsockets.c.o: thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/cmsockets.c.o"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/EVPath/EVPath && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/cmsockets.c.o -MF CMakeFiles/cmsockets.dir/cmsockets.c.o.d -o CMakeFiles/cmsockets.dir/cmsockets.c.o -c /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/EVPath/EVPath/cmsockets.c

thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/cmsockets.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cmsockets.dir/cmsockets.c.i"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/EVPath/EVPath && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/EVPath/EVPath/cmsockets.c > CMakeFiles/cmsockets.dir/cmsockets.c.i

thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/cmsockets.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cmsockets.dir/cmsockets.c.s"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/EVPath/EVPath && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/EVPath/EVPath/cmsockets.c -o CMakeFiles/cmsockets.dir/cmsockets.c.s

thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/ip_config.c.o: thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/flags.make
thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/ip_config.c.o: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/EVPath/EVPath/ip_config.c
thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/ip_config.c.o: thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/ip_config.c.o"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/EVPath/EVPath && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/ip_config.c.o -MF CMakeFiles/cmsockets.dir/ip_config.c.o.d -o CMakeFiles/cmsockets.dir/ip_config.c.o -c /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/EVPath/EVPath/ip_config.c

thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/ip_config.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cmsockets.dir/ip_config.c.i"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/EVPath/EVPath && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/EVPath/EVPath/ip_config.c > CMakeFiles/cmsockets.dir/ip_config.c.i

thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/ip_config.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cmsockets.dir/ip_config.c.s"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/EVPath/EVPath && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/EVPath/EVPath/ip_config.c -o CMakeFiles/cmsockets.dir/ip_config.c.s

# Object files for target cmsockets
cmsockets_OBJECTS = \
"CMakeFiles/cmsockets.dir/cmsockets.c.o" \
"CMakeFiles/cmsockets.dir/ip_config.c.o"

# External object files for target cmsockets
cmsockets_EXTERNAL_OBJECTS =

thirdparty/EVPath/EVPath/lib64/libadios2_cmsockets.so: thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/cmsockets.c.o
thirdparty/EVPath/EVPath/lib64/libadios2_cmsockets.so: thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/ip_config.c.o
thirdparty/EVPath/EVPath/lib64/libadios2_cmsockets.so: thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/build.make
thirdparty/EVPath/EVPath/lib64/libadios2_cmsockets.so: lib64/libadios2_atl.so.2.2.1
thirdparty/EVPath/EVPath/lib64/libadios2_cmsockets.so: thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C shared module lib64/libadios2_cmsockets.so"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/EVPath/EVPath && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cmsockets.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/build: thirdparty/EVPath/EVPath/lib64/libadios2_cmsockets.so
.PHONY : thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/build

thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/clean:
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/EVPath/EVPath && $(CMAKE_COMMAND) -P CMakeFiles/cmsockets.dir/cmake_clean.cmake
.PHONY : thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/clean

thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/depend:
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1 /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/EVPath/EVPath /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1 /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/EVPath/EVPath /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : thirdparty/EVPath/EVPath/CMakeFiles/cmsockets.dir/depend

