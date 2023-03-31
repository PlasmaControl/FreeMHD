# Install script for directory: /home/bw0594/OpenFOAM-v2206/ThirdParty/sources/cgal/CGAL-4.14.3

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/bw0594/OpenFOAM-v2206/ThirdParty/platforms/linux64Gcc/CGAL-4.14.3")
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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/CGAL-4.14.3" TYPE FILE FILES
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/cgal/CGAL-4.14.3/AUTHORS"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/cgal/CGAL-4.14.3/CHANGES.md"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/cgal/CGAL-4.14.3/LICENSE"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/cgal/CGAL-4.14.3/LICENSE.FREE_USE"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/cgal/CGAL-4.14.3/LICENSE.GPL"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/cgal/CGAL-4.14.3/LICENSE.LGPL"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/cgal/CGAL-4.14.3/include/CGAL" REGEX "/\\.svn$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/cgal/CGAL-4.14.3/scripts/cgal_create_CMakeLists"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/cgal/CGAL-4.14.3/scripts/cgal_create_cmake_script"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/cgal/CGAL-4.14.3/scripts/cgal_make_macosx_app"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/CGAL" TYPE DIRECTORY FILES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/cgal/CGAL-4.14.3/cmake/modules/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/CGAL" TYPE FILE FILES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/cgal/CGAL-4.14.3/cmake/modules/UseCGAL.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/CGAL" TYPE FILE FILES
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/CGAL-4.14.3/config/CGALConfig-installation-dirs.cmake"
    "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/cgal/CGAL-4.14.3/lib/cmake/CGAL/CGALConfig.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE FILES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/cgal/CGAL-4.14.3/auxiliary/cgal_create_cmake_script.1")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/CGAL-4.14.3/src/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/CGAL-4.14.3/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
