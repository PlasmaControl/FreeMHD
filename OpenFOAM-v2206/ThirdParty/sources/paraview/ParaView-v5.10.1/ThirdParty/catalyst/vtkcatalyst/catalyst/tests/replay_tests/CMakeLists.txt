cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(CATALYST_REPLAY_TESTS)

# These tests require that we use num_ranks != 1 at least once.
# Hence, we can not run it unless MPI is enabled
if (CATALYST_USE_MPI)
  add_catalyst_test(replay_high_num_ranks)
  add_catalyst_test(replay_ranks_mismatch)
endif()

add_catalyst_test(replay_high_num_execute_invc)
add_catalyst_test(replay_no_data_dump_dir)
add_catalyst_test(replay_missing_initialize_data)
add_catalyst_test(replay_missing_execute_invc)
