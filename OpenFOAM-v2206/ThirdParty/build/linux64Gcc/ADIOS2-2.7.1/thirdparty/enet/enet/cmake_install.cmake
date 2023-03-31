# Install script for directory: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/enet/enet

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/platforms/linux64Gcc/ADIOS2-2.7.1")
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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xadios2_enet-librariesx" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libadios2_enet.so.1.3.14"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libadios2_enet.so.1"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "$ORIGIN/../lib64")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64" TYPE SHARED_LIBRARY FILES
    "/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/lib64/libadios2_enet.so.1.3.14"
    "/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/lib64/libadios2_enet.so.1"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libadios2_enet.so.1.3.14"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libadios2_enet.so.1"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "::::::::::::::::"
           NEW_RPATH "$ORIGIN/../lib64")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xadios2_enet-developmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libadios2_enet.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libadios2_enet.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libadios2_enet.so"
         RPATH "$ORIGIN/../lib64")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64" TYPE SHARED_LIBRARY FILES "/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/lib64/libadios2_enet.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libadios2_enet.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libadios2_enet.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libadios2_enet.so"
         OLD_RPATH "::::::::::::::::"
         NEW_RPATH "$ORIGIN/../lib64")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libadios2_enet.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xadios2_enet-developmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/adios2/thirdparty" TYPE FILE FILES
    "/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet/enet-config.cmake"
    "/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet/enet-config-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xadios2_enet-developmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/adios2/thirdparty/enet-targets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/adios2/thirdparty/enet-targets.cmake"
         "/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet/CMakeFiles/Export/lib64/cmake/adios2/thirdparty/enet-targets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/adios2/thirdparty/enet-targets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/adios2/thirdparty/enet-targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/adios2/thirdparty" TYPE FILE FILES "/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet/CMakeFiles/Export/lib64/cmake/adios2/thirdparty/enet-targets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/adios2/thirdparty" TYPE FILE FILES "/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/thirdparty/enet/enet/CMakeFiles/Export/lib64/cmake/adios2/thirdparty/enet-targets-release.cmake")
  endif()
endif()

