# CMake generated Testfile for 
# Source directory: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/transports
# Build directory: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/testing/adios2/transports
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Transports.*/BufferTest.WriteRead/*.Serial "/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Transports.File.Serial" "--gtest_filter=*/BufferTest.WriteRead/*")
set_tests_properties(Transports.*/BufferTest.WriteRead/*.Serial PROPERTIES  _BACKTRACE_TRIPLES "/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;64;gtest_add_tests;/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/transports/CMakeLists.txt;6;gtest_add_tests_helper;/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/transports/CMakeLists.txt;0;")
