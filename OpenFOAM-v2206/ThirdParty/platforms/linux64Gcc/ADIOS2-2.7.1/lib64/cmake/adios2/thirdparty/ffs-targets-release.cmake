#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ffs::ffs" for configuration "Release"
set_property(TARGET ffs::ffs APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ffs::ffs PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "dill::dill"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib64/libadios2_ffs.so.1.6.0"
  IMPORTED_SONAME_RELEASE "libadios2_ffs.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS ffs::ffs )
list(APPEND _IMPORT_CHECK_FILES_FOR_ffs::ffs "${_IMPORT_PREFIX}/lib64/libadios2_ffs.so.1.6.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
