option(CATALYST_BUILD_REPLAY "Build the catalyst replay executable" ON)
cmake_dependent_option(CATALYST_USE_MPI "Enable MPI related features" OFF
  "CATALYST_BUILD_REPLAY" OFF)
if (CATALYST_BUILD_REPLAY)
  add_subdirectory(replay)
endif ()
