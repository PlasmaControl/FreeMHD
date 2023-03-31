function(_add_example2 example_or_test dir mode root)
  set(replay_argument)
  if (TARGET catalyst_replay)
    set(replay_argument
      "-Dcatalyst_replay_command:PATH=$<TARGET_FILE:catalyst_replay>")
  endif ()

  add_test(
    NAME    "${example_or_test}-${mode}-${dir}-prepare"
    COMMAND "${CMAKE_COMMAND}" -E rm -rf
            "${CMAKE_CURRENT_BINARY_DIR}/${mode}-${dir}")
  add_test(
    NAME    "${example_or_test}-${mode}-${dir}"
    COMMAND "${CMAKE_COMMAND}"
            "-DCMAKE_GENERATOR=${CMAKE_GENERATOR}"
            "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
            "-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}"
            "-DCATALYST_INSTALL_PACKAGE_DIR=${CATALYST_INSTALL_PACKAGE_DIR}"
            ${replay_argument}
            "-Dsrc=${CMAKE_CURRENT_SOURCE_DIR}"
            "-Droot=${root}"
            "-Ddir=${dir}"
            "-Dmode=${mode}"
            "-Dcatalyst_runtime_dir=$<TARGET_FILE_DIR:catalyst>"
            -P "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/catalyst-test-path-setting.cmake")
  set_tests_properties("${example_or_test}-${mode}-${dir}"
    PROPERTIES
      FIXTURES_REQUIRED "fixture-${mode}-${dir}")
  set_tests_properties("${example_or_test}-${mode}-${dir}-prepare"
    PROPERTIES
      FIXTURES_SETUP "fixture-${mode}-${dir}")

  if (mode STREQUAL "install")
    # install-based tests, depend on the install fixture.
    set_tests_properties("${example_or_test}-${mode}-${dir}"
      PROPERTIES
        FIXTURES_REQUIRED "install")
  endif()
endfunction()

function(_add_example example_or_test dir)
  _add_example2("${example_or_test}"
               "${dir}"
               build
               "${CATALYST_BINARY_DIR}")
  _add_example2("${example_or_test}"
               "${dir}"
               install
               "${install_prefix}/${CMAKE_INSTALL_PREFIX}")
endfunction()

macro(add_catalyst_example dir)
  _add_example(example "${dir}")
endmacro()

macro(add_catalyst_test dir)
  _add_example(test "${dir}")
endmacro()

macro(add_install_test)
  # add a test to do installation of catalyst
  set(install_prefix "${CMAKE_CURRENT_BINARY_DIR}/root")
  add_test(
    NAME  "install-prepare"
    COMMAND "${CMAKE_COMMAND}" -E rm -rf "${install_prefix}")
  add_test(
    NAME  "install"
    COMMAND "${CMAKE_COMMAND}"
            --build ${CATALYST_BINARY_DIR}
            --target "install"
            --config $<CONFIGURATION>
    WORKING_DIRECTORY ${CATALYST_BINARY_DIR})
  set_tests_properties("install"
    PROPERTIES
      ENVIRONMENT "DESTDIR=${install_prefix}")
  set_tests_properties("install-prepare"
    PROPERTIES
      FIXTURES_SETUP "install-prepare")
  set_tests_properties("install"
    PROPERTIES
      FIXTURES_REQUIRED "install-prepare"
      FIXTURES_SETUP    "install")
endmacro()
