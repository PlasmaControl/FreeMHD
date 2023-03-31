#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "atl::atl" for configuration "Release"
set_property(TARGET atl::atl APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(atl::atl PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib64/libadios2_atl.so.2.2.1"
  IMPORTED_SONAME_RELEASE "libadios2_atl.so.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS atl::atl )
list(APPEND _IMPORT_CHECK_FILES_FOR_atl::atl "${_IMPORT_PREFIX}/lib64/libadios2_atl.so.2.2.1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
