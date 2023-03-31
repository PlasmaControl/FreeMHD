# Client maintainer: chuck.atkins@kitware.com

set(ENV{CC}  xlc)
set(ENV{CXX} xlc++)
set(ENV{FC}  xlf)

find_program(MPICC mpixlc)
find_program(MPICXX mpixlC)
find_program(MPIFC mpixlf)

set(dashboard_cache "
ADIOS2_USE_BZip2:BOOL=OFF
ADIOS2_USE_Blosc:BOOL=OFF
ADIOS2_USE_DataMan:BOOL=OFF
ADIOS2_USE_Fortran:BOOL=ON
ADIOS2_USE_HDF5:BOOL=ON
ADIOS2_USE_MPI:BOOL=ON
ADIOS2_USE_Python:BOOL=OFF
ADIOS2_USE_SZ:BOOL=ON
ADIOS2_USE_ZeroMQ:STRING=OFF
ADIOS2_USE_ZFP:BOOL=ON

MPI_C_COMPILER:FILEPATH=${MPICC}
MPI_CXX_COMPILER:FILEPATH=${MPICXX}
MPI_Fortran_COMPILER:FILEPATH=${MPIFC}
MPIEXEC_EXTRA_FLAGS:STRING=--allow-run-as-root --oversubscribe -tcp
")

set(CTEST_TEST_ARGS
  PARALLEL_LEVEL 1
  INCLUDE "^(Interface|Install)"
)
set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
list(APPEND CTEST_UPDATE_NOTES_FILES "${CMAKE_CURRENT_LIST_FILE}")
include(${CMAKE_CURRENT_LIST_DIR}/ci-common.cmake)
