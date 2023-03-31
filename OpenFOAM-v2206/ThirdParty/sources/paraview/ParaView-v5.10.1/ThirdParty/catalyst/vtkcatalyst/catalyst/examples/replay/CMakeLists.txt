cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(CATALYST_REPLAY_EXAMPLE)

find_package(catalyst
  REQUIRED
  COMPONENTS SDK)

# use this function call to mark any library as the
# Catalyst API implementation.
catalyst_implementation(
  TARGET  catalyst_example_replay_adaptor
  NAME    example_replay_adaptor
  SOURCES catalyst_example_replay_adaptor.cxx)

if (CATALYST_USE_MPI)
  target_link_libraries(catalyst_example_replay_adaptor PRIVATE MPI::MPI_C)
  target_compile_definitions(catalyst_example_replay_adaptor
    PRIVATE REPLAY_ADAPTOR_USE_MPI
            # force `mpi.h` to not include cxx components to avoid
            # unnecessary dependency on MPI_CPP
            MPICH_SKIP_MPICXX
            OMPI_SKIP_MPICXX
            MPI_NO_CPPBIND
            _MPICC_H)
endif()

include(CTest)
if (BUILD_TESTING)
  add_executable(replay_test replay_test.cxx)

  target_link_libraries(replay_test
    PRIVATE
      catalyst::catalyst)

  set(num_ranks_if_mpi 2)
  set(mpi_prefix_or_blank)
  if (CATALYST_USE_MPI)
    target_link_libraries(replay_test PRIVATE MPI::MPI_C)
    target_compile_definitions(replay_test
      PRIVATE CATALYST_USE_MPI
              # force `mpi.h` to not include cxx components to avoid
              # unnecessary dependency on MPI_CPP
              MPICH_SKIP_MPICXX
              OMPI_SKIP_MPICXX
              MPI_NO_CPPBIND
              _MPICC_H)

    set(mpi_prefix_or_blank
        ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} ${num_ranks_if_mpi})
  endif()

  set(example_data_dump_directory "${CMAKE_CURRENT_BINARY_DIR}/data_dump")

  # remove old contents, if any.
  add_test(
    NAME replay_test_prepare
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${example_data_dump_directory}")

  set_tests_properties(replay_test_prepare
    PROPERTIES
      FIXTURES_SETUP prepare)

  # Add a test for writing out the conduit nodes to disk
  add_test(
    NAME replay_test_write_out
    COMMAND ${mpi_prefix_or_blank} "$<TARGET_FILE:replay_test>" "$<TARGET_FILE_DIR:catalyst_example_replay_adaptor>"
  )

  set_tests_properties(replay_test_write_out
    PROPERTIES
      ENVIRONMENT "CATALYST_DATA_DUMP_DIRECTORY=${example_data_dump_directory}"
      FIXTURES_SETUP write_out
      FIXTURES_REQUIRED prepare)

  add_test(
    NAME replay_test_read_in
    COMMAND ${mpi_prefix_or_blank} ${catalyst_replay_command} "${example_data_dump_directory}")

  set_tests_properties(replay_test_read_in
    PROPERTIES
      FIXTURES_REQUIRED write_out)
endif()
