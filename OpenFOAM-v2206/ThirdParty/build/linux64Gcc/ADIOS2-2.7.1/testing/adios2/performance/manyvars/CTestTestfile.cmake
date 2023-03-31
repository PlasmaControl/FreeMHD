# CMake generated Testfile for 
# Source directory: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/performance/manyvars
# Build directory: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/testing/adios2/performance/manyvars
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Performance.*/TestManyVars.DontRedefineVars/*.MPI "/usr/local/openmpi/4.1.0/gcc/bin/mpiexec" "-n" "48" "/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Performance.ManyVars.MPI" "--gtest_filter=*/TestManyVars.DontRedefineVars/*")
set_tests_properties(Performance.*/TestManyVars.DontRedefineVars/*.MPI PROPERTIES  PROCESSORS "48" _BACKTRACE_TRIPLES "/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;79;gtest_add_tests;/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/performance/manyvars/CMakeLists.txt;7;gtest_add_tests_helper;/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/performance/manyvars/CMakeLists.txt;0;")
