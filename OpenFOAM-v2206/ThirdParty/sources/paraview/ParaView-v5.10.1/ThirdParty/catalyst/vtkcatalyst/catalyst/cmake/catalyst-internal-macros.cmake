macro (c_set_if_not_set variable)
  if (NOT DEFINED "${variable}")
    set("${variable}" ${ARGN})
  endif()
endmacro()

function(c_install_targets)
  install(
    TARGETS ${ARGN}
    EXPORT  Catalyst
    ARCHIVE
      DESTINATION "${CATALYST_INSTALL_ARCHIVE_DIR}"
    LIBRARY
      DESTINATION "${CATALYST_INSTALL_LIBRARY_DIR}"
    RUNTIME
      DESTINATION "${CATALYST_INSTALL_RUNTIME_DIR}")
  foreach (arg IN LISTS ARGN)
    get_property(type
                 TARGET ${arg}
                 PROPERTY TYPE)
    # all static libraries are mangled using `catalyst_` prefix.
    # we don't mangle shared libs since this project only builds the
    # `catalyst` library as shared.
    if (type STREQUAL STATIC_LIBRARY)
      set_property(TARGET ${arg}
        PROPERTY OUTPUT_NAME ${arg}${CATALYST_CUSTOM_LIBRARY_SUFFIX})
    endif()
  endforeach()
endfunction()

function(c_install_headers)
  cmake_parse_arguments(PARSE_ARGV 0 "arg" "" "SUBDIR" "HEADERS")
  if (arg_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR
      "unknown arguments passed to 'c_install_headers': '${arg_UNPARSED_ARGUMENTS}'")
  endif()
  install(
    FILES ${arg_HEADERS}
    DESTINATION "${CATALYST_INSTALL_INCLUDE_DIR}/${arg_SUBDIR}")
endfunction()
