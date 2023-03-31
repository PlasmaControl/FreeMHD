add_subdirectory(catalyst_tests)

find_package(GTest)
if (GTest_FOUND)
  add_subdirectory(conduit_tests)
endif ()

if(CATALYST_BUILD_REPLAY)
  add_subdirectory(replay_tests)
endif()
