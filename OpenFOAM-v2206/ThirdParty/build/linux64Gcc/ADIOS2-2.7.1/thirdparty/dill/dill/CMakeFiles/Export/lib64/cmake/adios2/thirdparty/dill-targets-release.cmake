#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "dill::dill" for configuration "Release"
set_property(TARGET dill::dill APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(dill::dill PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib64/libadios2_dill.so.2.4.1"
  IMPORTED_SONAME_RELEASE "libadios2_dill.so.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS dill::dill )
list(APPEND _IMPORT_CHECK_FILES_FOR_dill::dill "${_IMPORT_PREFIX}/lib64/libadios2_dill.so.2.4.1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
