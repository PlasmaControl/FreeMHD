if (WIN32)
  set(ENV{PATH}
    "${catalyst_runtime_dir};$ENV{PATH}")
endif ()

set(replay_argument)
if (catalyst_replay_command)
  set(replay_argument
    "-Dcatalyst_replay_command=${catalyst_replay_command}")
endif ()

execute_process(
  COMMAND "${CMAKE_CTEST_COMMAND}"
          --build-generator
            "${CMAKE_GENERATOR}"
          --build-and-test
            "${src}/${dir}"
            "${CMAKE_CURRENT_BINARY_DIR}/${mode}-${dir}"
          --build-options
            "-DBUILD_TESTING:BOOL=ON"
            "-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}"
            "-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}"
            "-Dcatalyst_DIR:PATH=${root}/${CATALYST_INSTALL_PACKAGE_DIR}"
            ${replay_argument}
          --test-command
            "${CMAKE_CTEST_COMMAND}"
              -C $<CONFIGURATION>
              -V
  RESULT_VARIABLE status)

if (NOT status EQUAL 0)
  message(FATAL_ERROR "test or example failed!")
endif ()
