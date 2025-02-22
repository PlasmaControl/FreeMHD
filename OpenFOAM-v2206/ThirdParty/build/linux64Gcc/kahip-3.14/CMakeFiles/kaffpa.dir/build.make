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
CMAKE_SOURCE_DIR = /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/kahip/kahip-3.14

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14

# Include any dependencies generated for this target.
include CMakeFiles/kaffpa.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/kaffpa.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/kaffpa.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/kaffpa.dir/flags.make

CMakeFiles/kaffpa.dir/app/kaffpa.cpp.o: CMakeFiles/kaffpa.dir/flags.make
CMakeFiles/kaffpa.dir/app/kaffpa.cpp.o: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/kahip/kahip-3.14/app/kaffpa.cpp
CMakeFiles/kaffpa.dir/app/kaffpa.cpp.o: CMakeFiles/kaffpa.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/kaffpa.dir/app/kaffpa.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/kaffpa.dir/app/kaffpa.cpp.o -MF CMakeFiles/kaffpa.dir/app/kaffpa.cpp.o.d -o CMakeFiles/kaffpa.dir/app/kaffpa.cpp.o -c /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/kahip/kahip-3.14/app/kaffpa.cpp

CMakeFiles/kaffpa.dir/app/kaffpa.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kaffpa.dir/app/kaffpa.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/kahip/kahip-3.14/app/kaffpa.cpp > CMakeFiles/kaffpa.dir/app/kaffpa.cpp.i

CMakeFiles/kaffpa.dir/app/kaffpa.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kaffpa.dir/app/kaffpa.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/kahip/kahip-3.14/app/kaffpa.cpp -o CMakeFiles/kaffpa.dir/app/kaffpa.cpp.s

# Object files for target kaffpa
kaffpa_OBJECTS = \
"CMakeFiles/kaffpa.dir/app/kaffpa.cpp.o"

# External object files for target kaffpa
kaffpa_EXTERNAL_OBJECTS = \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/data_structure/graph_hierarchy.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/algorithms/strongly_connected_components.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/algorithms/topological_sort.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/algorithms/push_relabel.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/io/graph_io.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/tools/quality_metrics.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/tools/random_functions.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/tools/graph_extractor.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/tools/misc.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/tools/partition_snapshooter.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/graph_partitioner.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/w_cycles/wcycle_partitioner.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/coarsening/coarsening.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/coarsening/contraction.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/coarsening/edge_rating/edge_ratings.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/coarsening/matching/matching.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/coarsening/matching/random_matching.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/coarsening/matching/gpa/path.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/coarsening/matching/gpa/gpa_matching.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/coarsening/matching/gpa/path_set.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/coarsening/clustering/node_ordering.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/coarsening/clustering/size_constraint_label_propagation.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/initial_partitioning/initial_partitioning.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/initial_partitioning/initial_partitioner.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/initial_partitioning/initial_partition_bipartition.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/initial_partitioning/initial_refinement/initial_refinement.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/initial_partitioning/bipartition.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/initial_partitioning/initial_node_separator.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/uncoarsening.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/separator/area_bfs.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/separator/vertex_separator_algorithm.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/separator/vertex_separator_flow_solver.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/cycle_improvements/greedy_neg_cycle.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/cycle_improvements/problem_factory.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/cycle_improvements/augmented_Qgraph.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/mixed_refinement.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/label_propagation_refinement/label_propagation_refinement.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/refinement.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/2way_fm_refinement/two_way_fm.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/two_way_flow_refinement.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/boundary_bfs.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/flow_solving_kernel/cut_flow_problem_solver.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/most_balanced_minimum_cuts/most_balanced_minimum_cuts.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_refinement.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/complete_boundary.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/partial_boundary.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_scheduling/quotient_graph_scheduling.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_scheduling/simple_quotient_graph_scheduler.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_scheduling/active_block_quotient_graph_scheduler.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement_core.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement_commons.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/cycle_improvements/augmented_Qgraph_fabric.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/cycle_improvements/advanced_models.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/kway_graph_refinement/multitry_kway_fm.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/node_separators/greedy_ns_local_search.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/node_separators/fm_ns_local_search.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/node_separators/localized_fm_ns_local_search.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/algorithms/cycle_search.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/cycle_improvements/cycle_refinement.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/tabu_search/tabu_search.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libkaffpa.dir/extern/argtable3-3.0.3/argtable3.c.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libmapping.dir/lib/mapping/local_search_mapping.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libmapping.dir/lib/mapping/full_search_space.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libmapping.dir/lib/mapping/full_search_space_pruned.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libmapping.dir/lib/mapping/communication_graph_search_space.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libmapping.dir/lib/mapping/fast_construct_mapping.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libmapping.dir/lib/mapping/construct_distance_matrix.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libmapping.dir/lib/mapping/mapping_algorithms.cpp.o" \
"/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/libmapping.dir/lib/mapping/construct_mapping.cpp.o"

kaffpa: CMakeFiles/kaffpa.dir/app/kaffpa.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/data_structure/graph_hierarchy.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/algorithms/strongly_connected_components.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/algorithms/topological_sort.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/algorithms/push_relabel.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/io/graph_io.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/tools/quality_metrics.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/tools/random_functions.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/tools/graph_extractor.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/tools/misc.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/tools/partition_snapshooter.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/graph_partitioner.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/w_cycles/wcycle_partitioner.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/coarsening/coarsening.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/coarsening/contraction.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/coarsening/edge_rating/edge_ratings.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/coarsening/matching/matching.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/coarsening/matching/random_matching.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/coarsening/matching/gpa/path.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/coarsening/matching/gpa/gpa_matching.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/coarsening/matching/gpa/path_set.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/coarsening/clustering/node_ordering.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/coarsening/clustering/size_constraint_label_propagation.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/initial_partitioning/initial_partitioning.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/initial_partitioning/initial_partitioner.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/initial_partitioning/initial_partition_bipartition.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/initial_partitioning/initial_refinement/initial_refinement.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/initial_partitioning/bipartition.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/initial_partitioning/initial_node_separator.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/uncoarsening.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/separator/area_bfs.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/separator/vertex_separator_algorithm.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/separator/vertex_separator_flow_solver.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/cycle_improvements/greedy_neg_cycle.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/cycle_improvements/problem_factory.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/cycle_improvements/augmented_Qgraph.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/mixed_refinement.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/label_propagation_refinement/label_propagation_refinement.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/refinement.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/2way_fm_refinement/two_way_fm.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/two_way_flow_refinement.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/boundary_bfs.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/flow_solving_kernel/cut_flow_problem_solver.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/most_balanced_minimum_cuts/most_balanced_minimum_cuts.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_refinement.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/complete_boundary.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/partial_boundary.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_scheduling/quotient_graph_scheduling.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_scheduling/simple_quotient_graph_scheduler.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_scheduling/active_block_quotient_graph_scheduler.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement_core.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement_commons.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/cycle_improvements/augmented_Qgraph_fabric.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/cycle_improvements/advanced_models.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/kway_graph_refinement/multitry_kway_fm.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/node_separators/greedy_ns_local_search.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/node_separators/fm_ns_local_search.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/node_separators/localized_fm_ns_local_search.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/algorithms/cycle_search.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/cycle_improvements/cycle_refinement.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/lib/partition/uncoarsening/refinement/tabu_search/tabu_search.cpp.o
kaffpa: CMakeFiles/libkaffpa.dir/extern/argtable3-3.0.3/argtable3.c.o
kaffpa: CMakeFiles/libmapping.dir/lib/mapping/local_search_mapping.cpp.o
kaffpa: CMakeFiles/libmapping.dir/lib/mapping/full_search_space.cpp.o
kaffpa: CMakeFiles/libmapping.dir/lib/mapping/full_search_space_pruned.cpp.o
kaffpa: CMakeFiles/libmapping.dir/lib/mapping/communication_graph_search_space.cpp.o
kaffpa: CMakeFiles/libmapping.dir/lib/mapping/fast_construct_mapping.cpp.o
kaffpa: CMakeFiles/libmapping.dir/lib/mapping/construct_distance_matrix.cpp.o
kaffpa: CMakeFiles/libmapping.dir/lib/mapping/mapping_algorithms.cpp.o
kaffpa: CMakeFiles/libmapping.dir/lib/mapping/construct_mapping.cpp.o
kaffpa: CMakeFiles/kaffpa.dir/build.make
kaffpa: /usr/lib/gcc/x86_64-redhat-linux/8/libgomp.so
kaffpa: CMakeFiles/kaffpa.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable kaffpa"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kaffpa.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/kaffpa.dir/build: kaffpa
.PHONY : CMakeFiles/kaffpa.dir/build

CMakeFiles/kaffpa.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/kaffpa.dir/cmake_clean.cmake
.PHONY : CMakeFiles/kaffpa.dir/clean

CMakeFiles/kaffpa.dir/depend:
	cd /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/kahip/kahip-3.14 /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/kahip/kahip-3.14 /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14 /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14 /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/kahip-3.14/CMakeFiles/kaffpa.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/kaffpa.dir/depend

