#!/bin/bash

# Disable the make jobserver???
unset MAKEFLAGS
unset MAKE

function make_target_flags() {
  local tgt=$1
  sleep 1
  make -j1 VERBOSE=1 $tgt 2>&1 | \
    awk "/foo.(c|cxx|F90)( |$)/{print} /-o *(lib)?$tgt/{print}" | \
    sed -e "s| \+| |g" -e "s| *[^ ]*$tgt[^ ]*||g" \
      -e "s| *-D *WITH_ADIOS2||" > $tgt.flags
}

function a_without_b() {
  local A="$1"
  local B="$2"
  local skip=0
  local firstA=1
  for a in $A
  do
    if [ $firstA -eq 1 ]
    then
      firstA=0
      continue
    fi
    skip=0
    local firstB=1
    for b in ${B}
    do
      if [ $firstB -eq 1 ]
      then
        firstB=0
        continue
      fi
      if [ "${a}" == "${b}" ]
      then
        skip=1
        break
      fi
    done
    if [ $skip -eq 1 ]
    then
      continue
    fi
    echo $a
  done
}

function flag_diff() {
  local A="$1"
  local B="$2"
  a_without_b "$2" "$1"
}

function prefixify() {
  echo "$@" | sed "s|${PREFIX}|\${ADIOS2_PREFIX}|g"
}

PREFIX="${DESTDIR}$1"
shift
CMAKE="/usr/bin/cmake"
SOURCE_DIR="/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/install/post/adios2-config-dummy"
BUILD_DIR=$(mktemp -d)
pushd ${BUILD_DIR}

if [ 1 -eq 1 ]
then
  export CC="/usr/local/openmpi/4.1.0/gcc/bin/mpicc"
fi
if [ 1 -eq 1 ]
then
  export CXX="/usr/local/openmpi/4.1.0/gcc/bin/mpicxx"
fi
if [ 0 -eq 1 ]
then
  export FC=""
fi

EXTRA_CMAKE_ARGS="${EXTRA_CMAKE_ARGS} -Dadios2_DIR=${PREFIX}/lib64/cmake/adios2"
if [ -n "" ]
then
  EXTRA_CMAKE_ARGS="${EXTRA_CMAKE_ARGS} -DCMAKE_PREFIX_PATH="
fi

echo "Configuring adios2-config dummy CMake project"
if ! ${CMAKE} -G "Unix Makefiles" ${SOURCE_DIR} ${EXTRA_CMAKE_ARGS}
then
  echo "Failed"
  exit 1
fi

rm -f adios2.flags

variants="serial"
variant_default="serial"
if [ 1 -eq 1 ]
then
  variants="$variants mpi"
  variant_default="mpi"
fi

for variant in $variants
do
  echo "Extracting ADIOS flags for C bindings (${variant})"
  echo "  Without ADIOS"
  make_target_flags ${variant}_without_C
  without_C_CFLAGS=$(head -1 ${variant}_without_C.flags)
  without_C_LDFLAGS=$(tail -1 ${variant}_without_C.flags)
  echo "  With ADIOS"
  make_target_flags ${variant}_with_C
  with_C_CFLAGS=$(head -1 ${variant}_with_C.flags)
  with_C_LDFLAGS=$(tail -1 ${variant}_with_C.flags)
  echo "  Extracting diff"
  ADIOS2_C_CFLAGS="$(prefixify $(flag_diff "${without_C_CFLAGS}" "${with_C_CFLAGS}"))"
  ADIOS2_C_LDFLAGS="$(prefixify $(flag_diff "${without_C_LDFLAGS}" "${with_C_LDFLAGS}"))"
  echo "  Exporting"
  echo ADIOS2_C_CFLAGS_${variant}=\"${ADIOS2_C_CFLAGS}\" >> adios2.flags
  echo ADIOS2_C_LDFLAGS_${variant}=\"${ADIOS2_C_LDFLAGS}\" >> adios2.flags

  echo "Extracting ADIOS flags for C++ bindings (${variant})"
  echo "  Without ADIOS"
  make_target_flags ${variant}_without_CXX
  without_CXX_CXXFLAGS=$(head -1 ${variant}_without_CXX.flags)
  without_CXX_LDFLAGS=$(tail -1 ${variant}_without_CXX.flags)
  echo "  With ADIOS"
  make_target_flags ${variant}_with_CXX
  with_CXX_CXXFLAGS=$(head -1 ${variant}_with_CXX.flags)
  with_CXX_LDFLAGS=$(tail -1 ${variant}_with_CXX.flags)
  echo "  Extracting diff"
  ADIOS2_CXX_CXXFLAGS="$(prefixify $(flag_diff "${without_CXX_CXXFLAGS}" "${with_CXX_CXXFLAGS}"))"
  ADIOS2_CXX_LDFLAGS="$(prefixify $(flag_diff "${without_CXX_LDFLAGS}" "${with_CXX_LDFLAGS}"))"
  echo "  Exporting"
  echo ADIOS2_CXX_CXXFLAGS_${variant}=\"${ADIOS2_CXX_CXXFLAGS}\" >> adios2.flags
  echo ADIOS2_CXX_LDFLAGS_${variant}=\"${ADIOS2_CXX_LDFLAGS}\" >> adios2.flags

  if [ 0 -eq 1 ]
  then
    echo "Extracting ADIOS flags for Fortran bindings (${variant})"
    echo "  Without ADIOS"
    make_target_flags ${variant}_without_Fortran
    without_Fortran_FFLAGS=$(head -1 ${variant}_without_Fortran.flags)
    without_Fortran_LDFLAGS=$(tail -1 ${variant}_without_Fortran.flags)
    echo "  With ADIOS"
    make_target_flags ${variant}_with_Fortran
    with_Fortran_FFLAGS=$(head -1 ${variant}_with_Fortran.flags)
    with_Fortran_LDFLAGS=$(tail -1 ${variant}_with_Fortran.flags)
    echo "  Extracting diff"
    ADIOS2_Fortran_FFLAGS="$(prefixify $(flag_diff "${without_Fortran_FFLAGS}" "${with_Fortran_FFLAGS}"))"
    ADIOS2_Fortran_LDFLAGS="$(prefixify $(flag_diff "${without_Fortran_LDFLAGS}" "${with_Fortran_LDFLAGS}"))"
    echo "  Exporting"
    echo ADIOS2_Fortran_FFLAGS_${variant}=\"${ADIOS2_Fortran_FFLAGS}\" >> adios2.flags
    echo ADIOS2_Fortran_LDFLAGS_${variant}=\"${ADIOS2_Fortran_LDFLAGS}\" >> adios2.flags
  else
    echo ADIOS2_Fortran_FFLAGS_${variant}="" >> adios2.flags
    echo ADIOS2_Fortran_LDFLAGS_${variant}="" >> adios2.flags
  fi
done

echo "ADIOS2_CONFIG_MPI=1" >> adios2.flags
echo "ADIOS2_DEFAULT_VARIANT=${variant_default}" >> adios2.flags

if [[ "bin" == /* ]]
then
  # absolute path
  BINDIR="bin"
else
  # relative path
  BINDIR="${PREFIX}/bin"
fi

echo "Writing ${BINDIR}/adios2-config"
cat \
  "/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/cmake/install/post/adios2-config.pre.sh" \
  adios2.flags \
  "/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/cmake/install/post/adios2-config.post.sh" > "${BINDIR}/adios2-config"
chmod +x "${BINDIR}/adios2-config"
popd 
rm -rf ${BUILD_DIR}
