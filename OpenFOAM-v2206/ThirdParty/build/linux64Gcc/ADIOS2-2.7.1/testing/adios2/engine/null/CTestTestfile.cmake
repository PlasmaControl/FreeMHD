# CMake generated Testfile for 
# Source directory: /home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/engine/null
# Build directory: /home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/testing/adios2/engine/null
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Engine.Null.NullWriteReadTests.NullWriteRead1D8.Serial "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Engine.Null.WriteRead.Serial" "--gtest_filter=NullWriteReadTests.NullWriteRead1D8")
set_tests_properties(Engine.Null.NullWriteReadTests.NullWriteRead1D8.Serial PROPERTIES  _BACKTRACE_TRIPLES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;64;gtest_add_tests;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/engine/null/CMakeLists.txt;6;gtest_add_tests_helper;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/engine/null/CMakeLists.txt;0;")
add_test(Engine.Null.NullWriteReadTests.NullWriteRead1D8.MPI "/usr/local/openmpi/4.1.0/gcc/bin/mpiexec" "-n" "40" "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Engine.Null.WriteRead.MPI" "--gtest_filter=NullWriteReadTests.NullWriteRead1D8")
set_tests_properties(Engine.Null.NullWriteReadTests.NullWriteRead1D8.MPI PROPERTIES  PROCESSORS "40" _BACKTRACE_TRIPLES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;79;gtest_add_tests;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/engine/null/CMakeLists.txt;6;gtest_add_tests_helper;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/engine/null/CMakeLists.txt;0;")