# Macros and other CMake code part of the Catalyst SDK i.e.
# API used to build Catalyst API implementations.

set(_catalyst_macro_directory "${CMAKE_CURRENT_LIST_DIR}")

#[==[
Make a catalyst implementation library.

~~~{.cmake}
catalyst_implementation(
  TARGET  <target>
  NAME    <name>
  [EXPORT <export>]
  [SOURCES <source>...]
  [LIBRARY_DESTINATION <destination>]
  [CATALYST_TARGET <target>]
  )
~~~

The `TARGET` identifies the target to create in this function. This function
creates source files to adapt to the Catalyst ABI and creates a header for use
in the actual implementation.

`LIBRARY_DESTINATION` defaults to `${CMAKE_INSTALL_BINDIR}/catalyst` on Windows
and `${CMAKE_INSTALL_LIBDIR}/catalyst` elsewhere and indicates where the
library will be placed in the build and install tree.

If the `catalyst::catalyst` target is not available in the usage location, an
alternative available target name may be given by the `CATALYST_TARGET`
argument.

The library will be installed with the export set provided to `EXPORT` if
given.
#]==]
function (catalyst_implementation)
  cmake_parse_arguments(PARSE_ARGV 0 catalyst_impl
    ""
    "NAME;TARGET;LIBRARY_DESTINATION;EXPORT;CATALYST_TARGET"
    "SOURCES")
  if (catalyst_impl_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR
      "Unknown argument(s) passed to 'catalyst_implementation': '${catalyst_impl_UNPARSED_ARGUMENTS}'")
  endif ()

  if (NOT catalyst_impl_NAME)
    message(FATAL_ERROR
      "The `NAME` argument is required for `catalyst_implementation`.")
  endif ()

  if (NOT catalyst_impl_TARGET)
    message(FATAL_ERROR
      "The `TARGET` argument is required for `catalyst_implementation`.")
  endif ()

  if (NOT DEFINED catalyst_impl_CATALYST_TARGET)
    set(catalyst_impl_CATALYST_TARGET
      "catalyst::catalyst")
  endif ()
  if (NOT TARGET "${catalyst_impl_CATALYST_TARGET}")
    message(FATAL_ERROR
      "The catalyst target `${catalyst_impl_CATALYST_TARGET}` is not "
      "available.")
  endif ()

  if (NOT catalyst_impl_LIBRARY_DESTINATION)
    if (WIN32)
      set(catalyst_impl_LIBRARY_DESTINATION
        "${CMAKE_INSTALL_BINDIR}/catalyst")
    else ()
      set(catalyst_impl_LIBRARY_DESTINATION
        "${CMAKE_INSTALL_LIBDIR}/catalyst")
    endif ()
  endif ()

  if (NOT DEFINED _catalyst_macro_directory)
    set(_catalyst_macro_directory "${CMAKE_CURRENT_FUNCTION_LIST_DIR}")
  endif ()
  if (NOT _catalyst_macro_directory)
    message(FATAL_ERROR
      "`catalyst_implementation` does not know where its source files are; "
      "`find_package(catalyst)` was performed outside this scope and its "
      "tracking variables are not available. Either update to CMake 3.17 or "
      "ensure that catalyst is found within the scope where "
      "`catalyst_implementation` is used.")
  endif ()

  configure_file(
    "${_catalyst_macro_directory}/catalyst_impl.c.in"
    "${CMAKE_CURRENT_BINARY_DIR}/catalyst_impl_${catalyst_impl_NAME}.c"
    @ONLY)
  configure_file(
    "${_catalyst_macro_directory}/catalyst_impl.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/catalyst_impl_${catalyst_impl_NAME}.h"
    @ONLY)

  if (BUILD_SHARED_LIBS)
    set(catalyst_impl_library_type MODULE)
  else ()
    set(catalyst_impl_library_type STATIC)
  endif ()

  if (catalyst_impl_LIBRARY_DESTINATION)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${catalyst_impl_LIBRARY_DESTINATION}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${catalyst_impl_LIBRARY_DESTINATION}")
  endif ()

  add_library("${catalyst_impl_TARGET}" "${catalyst_impl_library_type}"
    "${CMAKE_CURRENT_BINARY_DIR}/catalyst_impl_${catalyst_impl_NAME}.c"
    "${CMAKE_CURRENT_BINARY_DIR}/catalyst_impl_${catalyst_impl_NAME}.h"
    ${catalyst_impl_SOURCES})
  target_include_directories("${catalyst_impl_TARGET}"
    PRIVATE
      "${CMAKE_CURRENT_BINARY_DIR}")
  target_link_libraries("${catalyst_impl_TARGET}"
    PUBLIC
      "${catalyst_impl_CATALYST_TARGET}")
  set_target_properties("${catalyst_impl_TARGET}"
    PROPERTIES
      OUTPUT_NAME "catalyst-${catalyst_impl_NAME}")

  if (WIN32)
    set_target_properties("${catalyst_impl_TARGET}"
      PROPERTIES
        DEBUG_POSTFIX "d")
  endif ()

  set(catalyst_impl_export_args)
  if (catalyst_impl_EXPORT)
    set(catalyst_impl_export_args
      EXPORT "${catalyst_impl_EXPORT}")
  endif ()

  if (catalyst_impl_LIBRARY_DESTINATION)
    install(
      TARGETS "${catalyst_impl_TARGET}"
      ${catalyst_impl_export_args}
      RUNTIME DESTINATION "${catalyst_impl_LIBRARY_DESTINATION}"
      LIBRARY DESTINATION "${catalyst_impl_LIBRARY_DESTINATION}")
  endif ()
endfunction ()
