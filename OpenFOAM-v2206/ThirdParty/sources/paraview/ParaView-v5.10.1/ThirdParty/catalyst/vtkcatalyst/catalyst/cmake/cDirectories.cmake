include(GNUInstallDirs)

c_set_if_not_set(catalyst_subdir "catalyst-${CATALYST_VERSION_MAJOR}.${CATALYST_VERSION_MINOR}")
c_set_if_not_set(CATALYST_INSTALL_RUNTIME_DIR "${CMAKE_INSTALL_BINDIR}")
c_set_if_not_set(CATALYST_INSTALL_LIBRARY_DIR "${CMAKE_INSTALL_LIBDIR}")
c_set_if_not_set(CATALYST_INSTALL_ARCHIVE_DIR "${CMAKE_INSTALL_LIBDIR}")
c_set_if_not_set(CATALYST_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}/${catalyst_subdir}")
c_set_if_not_set(CATALYST_INSTALL_PACKAGE_DIR "${CATALYST_INSTALL_LIBRARY_DIR}/cmake/${catalyst_subdir}")

# These are currently not used, but may be needed in future, hence leaving them
# commented out.
# c_set_if_not_set(CATALYST_INSTALL_DATA_DIR "${CMAKE_INSTALL_DATADIR}/${catalyst_subdir}")
# c_set_if_not_set(CATALYST_INSTALL_DOC_DIR "${CMAKE_INSTALL_DATAROOTDIR}/doc/${catalyst_subdir}")
# c_set_if_not_set(CATALYST_INSTALL_DOXYGEN_DIR "${CATALYST_INSTALL_DOC_DIR}/doxygen")
# c_set_if_not_set(CATALYST_INSTALL_NDK_MODULES_DIR "${CATALYST_INSTALL_DATA_DIR}/ndk-modules")

# Set up our directory structure for output libraries and binaries
c_set_if_not_set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CATALYST_INSTALL_RUNTIME_DIR}")
c_set_if_not_set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CATALYST_INSTALL_LIBRARY_DIR}")
c_set_if_not_set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CATALYST_INSTALL_ARCHIVE_DIR}")
