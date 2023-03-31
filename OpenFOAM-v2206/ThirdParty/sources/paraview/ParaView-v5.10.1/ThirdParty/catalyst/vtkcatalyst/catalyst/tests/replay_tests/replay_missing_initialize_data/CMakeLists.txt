cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(CATALYST_REPLAY_MISSING_INITIALIZE_DATA)

find_package(catalyst
  REQUIRED
  COMPONENTS SDK)

# No need to repeat the same adaptor and driver multiple times
# across tests
get_filename_component(PARENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
set(src_dir "${PARENT_DIR}/common_src_dir")

catalyst_implementation(
  TARGET  missing_initialize_adaptor
  NAME    replay
  SOURCES "${src_dir}/common_replay_adaptor.cxx")

include(CTest)
if (BUILD_TESTING)
  add_executable(missing_initialize_driver "${src_dir}/common_replay_driver.cxx")

  target_link_libraries(missing_initialize_driver
    PRIVATE
      catalyst::catalyst)

  set(num_ranks_if_mpi 2)
  set(mpi_prefix_or_blank)
  if (CATALYST_USE_MPI)
    target_link_libraries(missing_initialize_driver PRIVATE MPI::MPI_C)
    target_compile_definitions(missing_initialize_driver
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

  # Set up the data dump directory for the test
  set(data_dump_directory "${CMAKE_CURRENT_BINARY_DIR}/data_dump/")

  add_test(
    NAME missing_initialize_prepare
    COMMAND "${CMAKE_COMMAND}" -E rm -rf "${data_dump_directory}")

  set_tests_properties(missing_initialize_prepare
    PROPERTIES
      FIXTURES_SETUP prepare_fixture)

  add_test(
    NAME missing_initialize_write_out
    COMMAND ${mpi_prefix_or_blank} "$<TARGET_FILE:missing_initialize_driver>"
            "$<TARGET_FILE_DIR:missing_initialize_adaptor>"
  )

  file(MAKE_DIRECTORY ${data_dump_directory})

  set(write_out_environ_str
     "CATALYST_DATA_DUMP_DIRECTORY=${data_dump_directory}"
  )

  set_tests_properties(missing_initialize_write_out
    PROPERTIES
      ENVIRONMENT "${write_out_environ_str}"
      FIXTURES_SETUP missing_initialize_write_out_fixture
      FIXTURES_REQUIRED prepare_fixture)


  # Remove one of the ranks' initialize data
  if (CATALYST_USE_MPI)
    add_test(NAME remove_initialize
             COMMAND "${CMAKE_COMMAND}" -E rm
                     "${data_dump_directory}/initialize_params.conduit_bin.${num_ranks_if_mpi}.0")
  else()
    add_test(NAME remove_initialize
             COMMAND "${CMAKE_COMMAND}" -E rm
                     "${data_dump_directory}/initialize_params.conduit_bin.1.0")
  endif()

  set_tests_properties(remove_initialize
    PROPERTIES
      FIXTURES_REQUIRED missing_initialize_write_out_fixture
      FIXTURES_SETUP remove_initialize_fixture)


  # Add a test to check behavior when we're missing node data
  # for one rank's initialize stage.
  add_test(
    NAME missing_initialize_read_in
    COMMAND ${mpi_prefix_or_blank} ${catalyst_replay_command} ${data_dump_directory}
  )

  # The expected error message will be different based on how many files we write out,
  # which depends on the number of ranks.
  set(pass_str "ERROR: Missing node data for at least one stage")
  if (CATALYST_USE_MPI AND ${num_ranks_if_mpi} GREATER 1)
    set(pass_str "ERROR: Mismatch in the number of calls to initialize or finalize")
  endif()

  set_tests_properties(missing_initialize_read_in
    PROPERTIES
      PASS_REGULAR_EXPRESSION ${pass_str}
      FIXTURES_REQUIRED remove_initialize_fixture)

endif()
