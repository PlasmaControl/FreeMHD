cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(CATALYST_REPLAY_HIGH_NUM_EXECUTE_INVC)

find_package(catalyst
  REQUIRED
  COMPONENTS SDK)

# No need to repeat the same adaptor and driver multiple times
# across tests. Here we'll just use the common adaptor.
get_filename_component(PARENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
set(common_src_dir "${PARENT_DIR}/common_src_dir")

catalyst_implementation(
  TARGET  high_num_execute_invc_adaptor
  NAME    replay
  SOURCES "${common_src_dir}/common_replay_adaptor.cxx")

include(CTest)
if (BUILD_TESTING)
  add_executable(high_num_execute_invc_driver "high_num_execute_invc_driver.cxx")

  target_link_libraries(high_num_execute_invc_driver
    PRIVATE
      catalyst::catalyst)

  set(num_ranks_if_mpi 2)
  set(mpi_prefix_or_blank)
  if (CATALYST_USE_MPI)
    target_link_libraries(high_num_execute_invc_driver PRIVATE MPI::MPI_C)
    target_compile_definitions(high_num_execute_invc_driver
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
  set(data_dump_directory "${CMAKE_CURRENT_BINARY_DIR}/data_dump")

  add_test(
    NAME high_num_execute_invc_prepare
    COMMAND "${CMAKE_COMMAND}" -E rm -rf "${data_dump_directory}")

  set_tests_properties(high_num_execute_invc_prepare
    PROPERTIES
      FIXTURES_SETUP prepare)

  # Add a test for writing out the conduit nodes to disk
  add_test(
    NAME high_num_execute_invc_write_out
    COMMAND ${mpi_prefix_or_blank} "$<TARGET_FILE:high_num_execute_invc_driver>"
            "$<TARGET_FILE_DIR:high_num_execute_invc_adaptor>"
  )

  set_tests_properties(high_num_execute_invc_write_out
    PROPERTIES
      ENVIRONMENT "CATALYST_DATA_DUMP_DIRECTORY=${data_dump_directory}"
      FIXTURES_SETUP write_out
      FIXTURES_REQUIRED prepare)

  add_test(
    NAME high_num_execute_invc_read_in
    COMMAND ${mpi_prefix_or_blank} ${catalyst_replay_command} ${data_dump_directory}
  )

  set_tests_properties(high_num_execute_invc_read_in
    PROPERTIES
      FIXTURES_REQUIRED write_out)

endif()
