add_executable(catalyst_replay catalyst_replay.cpp)
target_link_libraries(catalyst_replay PRIVATE catalyst)

if(CATALYST_USE_MPI)
  target_link_libraries(catalyst_replay PRIVATE MPI::MPI_C)
  target_compile_definitions(catalyst_replay
    PRIVATE CATALYST_USE_MPI
            # force `mpi.h` to not include cxx components to avoid
            # unnecessary dependency on MPI_CPP
            MPICH_SKIP_MPICXX
            OMPI_SKIP_MPICXX
            MPI_NO_CPPBIND
            _MPICC_H)
endif()
