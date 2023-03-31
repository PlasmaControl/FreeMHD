# Install script for directory: /home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/install/pre

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/bw0594/OpenFOAM-v2206/ThirdParty/platforms/linux64Gcc/ADIOS2-2.7.1")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xadios2_core-developmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  
  message("Pre-installation cleanup of CMake files")
  file(REMOVE_RECURSE "/home/bw0594/OpenFOAM-v2206/ThirdParty/platforms/linux64Gcc/ADIOS2-2.7.1/lib64/cmake/adios2")

endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xadios2_core-developmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/adios2" TYPE FILE FILES
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/CMakeFindDependencyMacro.cmake"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/FindBZip2.cmake"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/FindBlosc.cmake"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/FindCrayDRC.cmake"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/FindDataSpaces.cmake"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/FindHDF5.cmake"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/FindIME.cmake"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/FindLIBFABRIC.cmake"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/FindMGARD.cmake"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/FindMPI.cmake"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/FindPkgConfig.cmake"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/FindPython.cmake"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/FindPythonModule.cmake"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/FindSZ.cmake"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/FindZeroMQ.cmake"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/Findpugixml.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xadios2_core-developmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/adios2/upstream" TYPE DIRECTORY FILES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/")
endif()

