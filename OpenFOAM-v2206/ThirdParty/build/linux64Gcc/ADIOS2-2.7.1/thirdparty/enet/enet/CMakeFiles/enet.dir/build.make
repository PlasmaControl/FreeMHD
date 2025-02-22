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
include thirdparty/enet/enet/CMakeFiles/enet.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include thirdparty/enet/enet/CMakeFiles/enet.dir/compiler_depend.make

# Include the progress variables for this target.
include thirdparty/enet/enet/CMakeFiles/enet.dir/progress.make

# Include the compile flags for this target's objects.
include thirdparty/enet/enet/CMakeFiles/enet.dir/flags.make

thirdparty/enet/enet/CMakeFiles/enet.dir/callbacks.c.o: thirdparty/enet/enet/CMakeFiles/enet.dir/flags.make
thirdparty/enet/enet/CMakeFiles/enet.dir/callbacks.c.o: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/callbacks.c
thirdparty/enet/enet/CMakeFiles/enet.dir/callbacks.c.o: thirdparty/enet/enet/CMakeFiles/enet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object thirdparty/enet/enet/CMakeFiles/enet.dir/callbacks.c.o"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT thirdparty/enet/enet/CMakeFiles/enet.dir/callbacks.c.o -MF CMakeFiles/enet.dir/callbacks.c.o.d -o CMakeFiles/enet.dir/callbacks.c.o -c /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/callbacks.c

thirdparty/enet/enet/CMakeFiles/enet.dir/callbacks.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/enet.dir/callbacks.c.i"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/callbacks.c > CMakeFiles/enet.dir/callbacks.c.i

thirdparty/enet/enet/CMakeFiles/enet.dir/callbacks.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/enet.dir/callbacks.c.s"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/callbacks.c -o CMakeFiles/enet.dir/callbacks.c.s

thirdparty/enet/enet/CMakeFiles/enet.dir/compress.c.o: thirdparty/enet/enet/CMakeFiles/enet.dir/flags.make
thirdparty/enet/enet/CMakeFiles/enet.dir/compress.c.o: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/compress.c
thirdparty/enet/enet/CMakeFiles/enet.dir/compress.c.o: thirdparty/enet/enet/CMakeFiles/enet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object thirdparty/enet/enet/CMakeFiles/enet.dir/compress.c.o"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT thirdparty/enet/enet/CMakeFiles/enet.dir/compress.c.o -MF CMakeFiles/enet.dir/compress.c.o.d -o CMakeFiles/enet.dir/compress.c.o -c /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/compress.c

thirdparty/enet/enet/CMakeFiles/enet.dir/compress.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/enet.dir/compress.c.i"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/compress.c > CMakeFiles/enet.dir/compress.c.i

thirdparty/enet/enet/CMakeFiles/enet.dir/compress.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/enet.dir/compress.c.s"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/compress.c -o CMakeFiles/enet.dir/compress.c.s

thirdparty/enet/enet/CMakeFiles/enet.dir/host.c.o: thirdparty/enet/enet/CMakeFiles/enet.dir/flags.make
thirdparty/enet/enet/CMakeFiles/enet.dir/host.c.o: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/host.c
thirdparty/enet/enet/CMakeFiles/enet.dir/host.c.o: thirdparty/enet/enet/CMakeFiles/enet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object thirdparty/enet/enet/CMakeFiles/enet.dir/host.c.o"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT thirdparty/enet/enet/CMakeFiles/enet.dir/host.c.o -MF CMakeFiles/enet.dir/host.c.o.d -o CMakeFiles/enet.dir/host.c.o -c /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/host.c

thirdparty/enet/enet/CMakeFiles/enet.dir/host.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/enet.dir/host.c.i"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/host.c > CMakeFiles/enet.dir/host.c.i

thirdparty/enet/enet/CMakeFiles/enet.dir/host.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/enet.dir/host.c.s"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/host.c -o CMakeFiles/enet.dir/host.c.s

thirdparty/enet/enet/CMakeFiles/enet.dir/list.c.o: thirdparty/enet/enet/CMakeFiles/enet.dir/flags.make
thirdparty/enet/enet/CMakeFiles/enet.dir/list.c.o: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/list.c
thirdparty/enet/enet/CMakeFiles/enet.dir/list.c.o: thirdparty/enet/enet/CMakeFiles/enet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object thirdparty/enet/enet/CMakeFiles/enet.dir/list.c.o"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT thirdparty/enet/enet/CMakeFiles/enet.dir/list.c.o -MF CMakeFiles/enet.dir/list.c.o.d -o CMakeFiles/enet.dir/list.c.o -c /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/list.c

thirdparty/enet/enet/CMakeFiles/enet.dir/list.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/enet.dir/list.c.i"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/list.c > CMakeFiles/enet.dir/list.c.i

thirdparty/enet/enet/CMakeFiles/enet.dir/list.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/enet.dir/list.c.s"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/list.c -o CMakeFiles/enet.dir/list.c.s

thirdparty/enet/enet/CMakeFiles/enet.dir/packet.c.o: thirdparty/enet/enet/CMakeFiles/enet.dir/flags.make
thirdparty/enet/enet/CMakeFiles/enet.dir/packet.c.o: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/packet.c
thirdparty/enet/enet/CMakeFiles/enet.dir/packet.c.o: thirdparty/enet/enet/CMakeFiles/enet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object thirdparty/enet/enet/CMakeFiles/enet.dir/packet.c.o"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT thirdparty/enet/enet/CMakeFiles/enet.dir/packet.c.o -MF CMakeFiles/enet.dir/packet.c.o.d -o CMakeFiles/enet.dir/packet.c.o -c /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/packet.c

thirdparty/enet/enet/CMakeFiles/enet.dir/packet.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/enet.dir/packet.c.i"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/packet.c > CMakeFiles/enet.dir/packet.c.i

thirdparty/enet/enet/CMakeFiles/enet.dir/packet.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/enet.dir/packet.c.s"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/packet.c -o CMakeFiles/enet.dir/packet.c.s

thirdparty/enet/enet/CMakeFiles/enet.dir/peer.c.o: thirdparty/enet/enet/CMakeFiles/enet.dir/flags.make
thirdparty/enet/enet/CMakeFiles/enet.dir/peer.c.o: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/peer.c
thirdparty/enet/enet/CMakeFiles/enet.dir/peer.c.o: thirdparty/enet/enet/CMakeFiles/enet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object thirdparty/enet/enet/CMakeFiles/enet.dir/peer.c.o"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT thirdparty/enet/enet/CMakeFiles/enet.dir/peer.c.o -MF CMakeFiles/enet.dir/peer.c.o.d -o CMakeFiles/enet.dir/peer.c.o -c /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/peer.c

thirdparty/enet/enet/CMakeFiles/enet.dir/peer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/enet.dir/peer.c.i"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/peer.c > CMakeFiles/enet.dir/peer.c.i

thirdparty/enet/enet/CMakeFiles/enet.dir/peer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/enet.dir/peer.c.s"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/peer.c -o CMakeFiles/enet.dir/peer.c.s

thirdparty/enet/enet/CMakeFiles/enet.dir/protocol.c.o: thirdparty/enet/enet/CMakeFiles/enet.dir/flags.make
thirdparty/enet/enet/CMakeFiles/enet.dir/protocol.c.o: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/protocol.c
thirdparty/enet/enet/CMakeFiles/enet.dir/protocol.c.o: thirdparty/enet/enet/CMakeFiles/enet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object thirdparty/enet/enet/CMakeFiles/enet.dir/protocol.c.o"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT thirdparty/enet/enet/CMakeFiles/enet.dir/protocol.c.o -MF CMakeFiles/enet.dir/protocol.c.o.d -o CMakeFiles/enet.dir/protocol.c.o -c /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/protocol.c

thirdparty/enet/enet/CMakeFiles/enet.dir/protocol.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/enet.dir/protocol.c.i"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/protocol.c > CMakeFiles/enet.dir/protocol.c.i

thirdparty/enet/enet/CMakeFiles/enet.dir/protocol.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/enet.dir/protocol.c.s"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/protocol.c -o CMakeFiles/enet.dir/protocol.c.s

thirdparty/enet/enet/CMakeFiles/enet.dir/unix.c.o: thirdparty/enet/enet/CMakeFiles/enet.dir/flags.make
thirdparty/enet/enet/CMakeFiles/enet.dir/unix.c.o: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/unix.c
thirdparty/enet/enet/CMakeFiles/enet.dir/unix.c.o: thirdparty/enet/enet/CMakeFiles/enet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object thirdparty/enet/enet/CMakeFiles/enet.dir/unix.c.o"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT thirdparty/enet/enet/CMakeFiles/enet.dir/unix.c.o -MF CMakeFiles/enet.dir/unix.c.o.d -o CMakeFiles/enet.dir/unix.c.o -c /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/unix.c

thirdparty/enet/enet/CMakeFiles/enet.dir/unix.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/enet.dir/unix.c.i"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/unix.c > CMakeFiles/enet.dir/unix.c.i

thirdparty/enet/enet/CMakeFiles/enet.dir/unix.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/enet.dir/unix.c.s"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && /usr/local/openmpi/4.1.0/gcc/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet/unix.c -o CMakeFiles/enet.dir/unix.c.s

# Object files for target enet
enet_OBJECTS = \
"CMakeFiles/enet.dir/callbacks.c.o" \
"CMakeFiles/enet.dir/compress.c.o" \
"CMakeFiles/enet.dir/host.c.o" \
"CMakeFiles/enet.dir/list.c.o" \
"CMakeFiles/enet.dir/packet.c.o" \
"CMakeFiles/enet.dir/peer.c.o" \
"CMakeFiles/enet.dir/protocol.c.o" \
"CMakeFiles/enet.dir/unix.c.o"

# External object files for target enet
enet_EXTERNAL_OBJECTS =

lib64/libadios2_enet.so.1.3.14: thirdparty/enet/enet/CMakeFiles/enet.dir/callbacks.c.o
lib64/libadios2_enet.so.1.3.14: thirdparty/enet/enet/CMakeFiles/enet.dir/compress.c.o
lib64/libadios2_enet.so.1.3.14: thirdparty/enet/enet/CMakeFiles/enet.dir/host.c.o
lib64/libadios2_enet.so.1.3.14: thirdparty/enet/enet/CMakeFiles/enet.dir/list.c.o
lib64/libadios2_enet.so.1.3.14: thirdparty/enet/enet/CMakeFiles/enet.dir/packet.c.o
lib64/libadios2_enet.so.1.3.14: thirdparty/enet/enet/CMakeFiles/enet.dir/peer.c.o
lib64/libadios2_enet.so.1.3.14: thirdparty/enet/enet/CMakeFiles/enet.dir/protocol.c.o
lib64/libadios2_enet.so.1.3.14: thirdparty/enet/enet/CMakeFiles/enet.dir/unix.c.o
lib64/libadios2_enet.so.1.3.14: thirdparty/enet/enet/CMakeFiles/enet.dir/build.make
lib64/libadios2_enet.so.1.3.14: thirdparty/enet/enet/CMakeFiles/enet.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking C shared library ../../../lib64/libadios2_enet.so"
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/enet.dir/link.txt --verbose=$(VERBOSE)
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && $(CMAKE_COMMAND) -E cmake_symlink_library ../../../lib64/libadios2_enet.so.1.3.14 ../../../lib64/libadios2_enet.so.1 ../../../lib64/libadios2_enet.so

lib64/libadios2_enet.so.1: lib64/libadios2_enet.so.1.3.14
	@$(CMAKE_COMMAND) -E touch_nocreate lib64/libadios2_enet.so.1

lib64/libadios2_enet.so: lib64/libadios2_enet.so.1.3.14
	@$(CMAKE_COMMAND) -E touch_nocreate lib64/libadios2_enet.so

# Rule to build all files generated by this target.
thirdparty/enet/enet/CMakeFiles/enet.dir/build: lib64/libadios2_enet.so
.PHONY : thirdparty/enet/enet/CMakeFiles/enet.dir/build

thirdparty/enet/enet/CMakeFiles/enet.dir/clean:
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet && $(CMAKE_COMMAND) -P CMakeFiles/enet.dir/cmake_clean.cmake
.PHONY : thirdparty/enet/enet/CMakeFiles/enet.dir/clean

thirdparty/enet/enet/CMakeFiles/enet.dir/depend:
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1 /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1 /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet/CMakeFiles/enet.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : thirdparty/enet/enet/CMakeFiles/enet.dir/depend

