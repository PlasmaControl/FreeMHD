
function usage() {
  echo "adios2-config [OPTION]"
  echo "  -h, --help       Display help information"
  echo "  -v, --version    Display version information"
  echo "  -c               Both compile and link flags for the C bindings"
  echo "  --c-flags        Preprocessor and compile flags for the C bindings"
  echo "  --c-libs         Linker flags for the C bindings"
  echo "  -x, --cxx        Both compile and link flags for the C++ bindings"
  echo "  --cxx-flags      Preprocessor and compile flags for the C++ bindings"
  echo "  --cxx-libs       Linker flags for the C++ bindings"
  echo "  -f, --fortran    Both compile and link flags for the F90 bindings"
  echo "  --fortran-flags  Preprocessor and compile flags for the F90 bindings"
  echo "  --fortran-libs   Linker flags for the F90 bindings"
  echo "  -s, --serial     Select flags for serial applications"
  echo "  -m, --mpi        Select flags for mpi applications"
}

if [ $# -eq 0 ]
then
  usage
  exit 1
fi

CF=0
CL=0
CXXF=0
CXXL=0
FF=0
FL=0
VARIANT="${ADIOS2_DEFAULT_VARIANT}"

while [ $# -gt 0 ]
do
  case $1
  in
    -c) CF=1; CL=1 ;;
    --c-flags) CF=1 ;;
    --c-libs) CL=1 ;;
    -x|--cxx) CXXF=1; CXXL=1 ;;
    --cxx-flags) CXXF=1 ;;
    --cxx-libs) CXXL=1 ;;
    -f|--fortran) FF=1; FL=1 ;;
    --fortran-flags) FF=1 ;;
    --fortran-libs) FL=1 ;;
    -s|--serial) VARIANT="serial" ;;
    -m|--mpi)
      if [ $ADIOS2_CONFIG_MPI -ne 1 ]; then
        echo >&2 "ADIOS2 was not built with MPI support: $1 not supported"
        exit 1
      fi
      VARIANT="mpi" ;;
    -v|--version)
      echo "ADIOS 2.7.1"
      echo "Copyright (c) 2019 UT-BATTELLE, LLC"
      echo "Licensed under the Apache License, Version 2.0"
      exit 0
      ;;
    -h|--help)
      usage
      exit 1
      ;;
    *)
      echo "Error: Unknown argument: $1"
      usage
      exit 1
      ;;
  esac
  shift
done

if [ $CF -eq 1 ]; then eval echo -n "\${ADIOS2_C_CFLAGS_${VARIANT}}\ "; fi
if [ $CL -eq 1 ]; then eval echo -n "\${ADIOS2_C_LDFLAGS_${VARIANT}}\ "; fi
if [ $CXXF -eq 1 ]; then eval echo -n "\${ADIOS2_CXX_CXXFLAGS_${VARIANT}}\ "; fi
if [ $CXXL -eq 1 ]; then eval echo -n "\${ADIOS2_CXX_LDFLAGS_${VARIANT}}\ "; fi
if [ $FF -eq 1 ]; then eval echo -n "\${ADIOS2_Fortran_FFLAGS_${VARIANT}}\ "; fi
if [ $FL -eq 1 ]; then eval echo -n "\${ADIOS2_Fortran_LDFLAGS_${VARIANT}}\ "; fi

echo ""
