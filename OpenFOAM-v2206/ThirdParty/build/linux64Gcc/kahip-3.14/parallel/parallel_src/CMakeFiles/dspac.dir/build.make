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
CMAKE_SOURCE_DIR = /home/bw0594/OpenFOAM-v2206/ThirdParty/sources/kahip/kahip-3.14

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14

# Include any dependencies generated for this target.
include parallel/parallel_src/CMakeFiles/dspac.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include parallel/parallel_src/CMakeFiles/dspac.dir/compiler_depend.make

# Include the compile flags for this target's objects.
include parallel/parallel_src/CMakeFiles/dspac.dir/flags.make

parallel/parallel_src/CMakeFiles/dspac.dir/app/dspac.cpp.o: parallel/parallel_src/CMakeFiles/dspac.dir/flags.make
parallel/parallel_src/CMakeFiles/dspac.dir/app/dspac.cpp.o: /home/bw0594/OpenFOAM-v2206/ThirdParty/sources/kahip/kahip-3.14/parallel/parallel_src/app/dspac.cpp
parallel/parallel_src/CMakeFiles/dspac.dir/app/dspac.cpp.o: parallel/parallel_src/CMakeFiles/dspac.dir/compiler_depend.ts
	cd /home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT parallel/parallel_src/CMakeFiles/dspac.dir/app/dspac.cpp.o -MF CMakeFiles/dspac.dir/app/dspac.cpp.o.d -o CMakeFiles/dspac.dir/app/dspac.cpp.o -c /home/bw0594/OpenFOAM-v2206/ThirdParty/sources/kahip/kahip-3.14/parallel/parallel_src/app/dspac.cpp

parallel/parallel_src/CMakeFiles/dspac.dir/app/dspac.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dspac.dir/app/dspac.cpp.i"
	cd /home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bw0594/OpenFOAM-v2206/ThirdParty/sources/kahip/kahip-3.14/parallel/parallel_src/app/dspac.cpp > CMakeFiles/dspac.dir/app/dspac.cpp.i

parallel/parallel_src/CMakeFiles/dspac.dir/app/dspac.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dspac.dir/app/dspac.cpp.s"
	cd /home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bw0594/OpenFOAM-v2206/ThirdParty/sources/kahip/kahip-3.14/parallel/parallel_src/app/dspac.cpp -o CMakeFiles/dspac.dir/app/dspac.cpp.s

# Object files for target dspac
dspac_OBJECTS = \
"CMakeFiles/dspac.dir/app/dspac.cpp.o"

# External object files for target dspac
dspac_EXTERNAL_OBJECTS = \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/data_structure/parallel_graph_access.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/data_structure/balance_management.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/data_structure/balance_management_refinement.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/data_structure/balance_management_coarsening.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/parallel_label_compress/node_ordering.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/parallel_contraction_projection/parallel_contraction.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/parallel_contraction_projection/parallel_block_down_propagation.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/parallel_contraction_projection/parallel_projection.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/distributed_partitioning/distributed_partitioner.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/distributed_partitioning/initial_partitioning/initial_partitioning.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/distributed_partitioning/initial_partitioning/distributed_evolutionary_partitioning.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/distributed_partitioning/initial_partitioning/random_initial_partitioning.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/communication/mpi_tools.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/communication/dummy_operations.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/io/parallel_graph_io.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/io/parallel_vector_io.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/tools/random_functions.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/lib/tools/distributed_quality_metrics.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libparallel.dir/extern/argtable3-3.0.3/argtable3.c.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libdspac.dir/lib/dspac/dspac.cpp.o" \
"/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/libdspac.dir/lib/dspac/edge_balanced_graph_io.cpp.o"

parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/dspac.dir/app/dspac.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/data_structure/parallel_graph_access.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/data_structure/balance_management.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/data_structure/balance_management_refinement.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/data_structure/balance_management_coarsening.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/parallel_label_compress/node_ordering.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/parallel_contraction_projection/parallel_contraction.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/parallel_contraction_projection/parallel_block_down_propagation.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/parallel_contraction_projection/parallel_projection.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/distributed_partitioning/distributed_partitioner.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/distributed_partitioning/initial_partitioning/initial_partitioning.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/distributed_partitioning/initial_partitioning/distributed_evolutionary_partitioning.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/distributed_partitioning/initial_partitioning/random_initial_partitioning.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/communication/mpi_tools.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/communication/dummy_operations.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/io/parallel_graph_io.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/io/parallel_vector_io.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/tools/random_functions.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/lib/tools/distributed_quality_metrics.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libparallel.dir/extern/argtable3-3.0.3/argtable3.c.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libdspac.dir/lib/dspac/dspac.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/libdspac.dir/lib/dspac/edge_balanced_graph_io.cpp.o
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/dspac.dir/build.make
parallel/parallel_src/dspac: parallel/modified_kahip/liblibmodified_kahip_interface.a
parallel/parallel_src/dspac: /usr/local/openmpi/4.1.0/gcc/lib64/libmpi_cxx.so
parallel/parallel_src/dspac: /usr/local/openmpi/4.1.0/gcc/lib64/libmpi.so
parallel/parallel_src/dspac: /usr/lib/gcc/x86_64-redhat-linux/8/libgomp.so
parallel/parallel_src/dspac: parallel/parallel_src/CMakeFiles/dspac.dir/link.txt
	cd /home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dspac.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
parallel/parallel_src/CMakeFiles/dspac.dir/build: parallel/parallel_src/dspac
.PHONY : parallel/parallel_src/CMakeFiles/dspac.dir/build

parallel/parallel_src/CMakeFiles/dspac.dir/clean:
	cd /home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src && $(CMAKE_COMMAND) -P CMakeFiles/dspac.dir/cmake_clean.cmake
.PHONY : parallel/parallel_src/CMakeFiles/dspac.dir/clean

parallel/parallel_src/CMakeFiles/dspac.dir/depend:
	cd /home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bw0594/OpenFOAM-v2206/ThirdParty/sources/kahip/kahip-3.14 /home/bw0594/OpenFOAM-v2206/ThirdParty/sources/kahip/kahip-3.14/parallel/parallel_src /home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14 /home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src /home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/parallel/parallel_src/CMakeFiles/dspac.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : parallel/parallel_src/CMakeFiles/dspac.dir/depend
