# CMake generated Testfile for 
# Source directory: /home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C
# Build directory: /home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/testing/adios2/bindings/C
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Bindings.C.ADIOS2_C_API.ADIOS2BPWriteTypes.Serial "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Bindings.C.WriteTypes.Serial" "--gtest_filter=ADIOS2_C_API.ADIOS2BPWriteTypes")
set_tests_properties(Bindings.C.ADIOS2_C_API.ADIOS2BPWriteTypes.Serial PROPERTIES  _BACKTRACE_TRIPLES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;64;gtest_add_tests;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;6;gtest_add_tests_helper;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;0;")
add_test(Bindings.C.ADIOS2_C_API_IO.Engine.Serial "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Bindings.C.WriteTypes.Serial" "--gtest_filter=ADIOS2_C_API_IO.Engine")
set_tests_properties(Bindings.C.ADIOS2_C_API_IO.Engine.Serial PROPERTIES  _BACKTRACE_TRIPLES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;64;gtest_add_tests;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;6;gtest_add_tests_helper;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;0;")
add_test(Bindings.C.ADIOS2_C_API_IO.EngineDefault.Serial "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Bindings.C.WriteTypes.Serial" "--gtest_filter=ADIOS2_C_API_IO.EngineDefault")
set_tests_properties(Bindings.C.ADIOS2_C_API_IO.EngineDefault.Serial PROPERTIES  _BACKTRACE_TRIPLES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;64;gtest_add_tests;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;6;gtest_add_tests_helper;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;0;")
add_test(Bindings.C.ADIOS2_C_API_IO.ReturnedStrings.Serial "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Bindings.C.WriteTypes.Serial" "--gtest_filter=ADIOS2_C_API_IO.ReturnedStrings")
set_tests_properties(Bindings.C.ADIOS2_C_API_IO.ReturnedStrings.Serial PROPERTIES  _BACKTRACE_TRIPLES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;64;gtest_add_tests;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;6;gtest_add_tests_helper;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;0;")
add_test(Bindings.C.ADIOS2_C_API.ADIOS2BPWriteTypes.MPI "/usr/local/openmpi/4.1.0/gcc/bin/mpiexec" "-n" "40" "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Bindings.C.WriteTypes.MPI" "--gtest_filter=ADIOS2_C_API.ADIOS2BPWriteTypes")
set_tests_properties(Bindings.C.ADIOS2_C_API.ADIOS2BPWriteTypes.MPI PROPERTIES  PROCESSORS "40" _BACKTRACE_TRIPLES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;79;gtest_add_tests;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;6;gtest_add_tests_helper;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;0;")
add_test(Bindings.C.ADIOS2_C_API_IO.Engine.MPI "/usr/local/openmpi/4.1.0/gcc/bin/mpiexec" "-n" "40" "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Bindings.C.WriteTypes.MPI" "--gtest_filter=ADIOS2_C_API_IO.Engine")
set_tests_properties(Bindings.C.ADIOS2_C_API_IO.Engine.MPI PROPERTIES  PROCESSORS "40" _BACKTRACE_TRIPLES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;79;gtest_add_tests;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;6;gtest_add_tests_helper;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;0;")
add_test(Bindings.C.ADIOS2_C_API_IO.EngineDefault.MPI "/usr/local/openmpi/4.1.0/gcc/bin/mpiexec" "-n" "40" "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Bindings.C.WriteTypes.MPI" "--gtest_filter=ADIOS2_C_API_IO.EngineDefault")
set_tests_properties(Bindings.C.ADIOS2_C_API_IO.EngineDefault.MPI PROPERTIES  PROCESSORS "40" _BACKTRACE_TRIPLES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;79;gtest_add_tests;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;6;gtest_add_tests_helper;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;0;")
add_test(Bindings.C.ADIOS2_C_API_IO.ReturnedStrings.MPI "/usr/local/openmpi/4.1.0/gcc/bin/mpiexec" "-n" "40" "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Bindings.C.WriteTypes.MPI" "--gtest_filter=ADIOS2_C_API_IO.ReturnedStrings")
set_tests_properties(Bindings.C.ADIOS2_C_API_IO.ReturnedStrings.MPI PROPERTIES  PROCESSORS "40" _BACKTRACE_TRIPLES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;79;gtest_add_tests;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;6;gtest_add_tests_helper;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;0;")
add_test(Bindings.C.BPWriteReadMultiblockCC.ZeroSizeBlocks.Serial "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Bindings.C.WriteReadMultiblock.Serial" "--gtest_filter=BPWriteReadMultiblockCC.ZeroSizeBlocks")
set_tests_properties(Bindings.C.BPWriteReadMultiblockCC.ZeroSizeBlocks.Serial PROPERTIES  _BACKTRACE_TRIPLES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;64;gtest_add_tests;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;7;gtest_add_tests_helper;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;0;")
add_test(Bindings.C.BPWriteReadMultiblockCC.ZeroSizeBlocks.MPI "/usr/local/openmpi/4.1.0/gcc/bin/mpiexec" "-n" "40" "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Bindings.C.WriteReadMultiblock.MPI" "--gtest_filter=BPWriteReadMultiblockCC.ZeroSizeBlocks")
set_tests_properties(Bindings.C.BPWriteReadMultiblockCC.ZeroSizeBlocks.MPI PROPERTIES  PROCESSORS "40" _BACKTRACE_TRIPLES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;79;gtest_add_tests;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;7;gtest_add_tests_helper;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;0;")
add_test(Bindings.C.NullWriteReadTests_C_API.NullWriteRead1D8.Serial "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Bindings.C.NullWriteRead.Serial" "--gtest_filter=NullWriteReadTests_C_API.NullWriteRead1D8")
set_tests_properties(Bindings.C.NullWriteReadTests_C_API.NullWriteRead1D8.Serial PROPERTIES  _BACKTRACE_TRIPLES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;64;gtest_add_tests;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;8;gtest_add_tests_helper;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;0;")
add_test(Bindings.C.NullWriteReadTests_C_API.NullWriteRead1D8.MPI "/usr/local/openmpi/4.1.0/gcc/bin/mpiexec" "-n" "40" "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Bindings.C.NullWriteRead.MPI" "--gtest_filter=NullWriteReadTests_C_API.NullWriteRead1D8")
set_tests_properties(Bindings.C.NullWriteReadTests_C_API.NullWriteRead1D8.MPI PROPERTIES  PROCESSORS "40" _BACKTRACE_TRIPLES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;79;gtest_add_tests;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;8;gtest_add_tests_helper;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;0;")
add_test(Bindings.C.*/BPWriteAggregateReadLocalTest.Aggregate1D/*.MPI "/usr/local/openmpi/4.1.0/gcc/bin/mpiexec" "-n" "40" "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Bindings.C.WriteAggregateReadLocal.MPI" "--gtest_filter=*/BPWriteAggregateReadLocalTest.Aggregate1D/*")
set_tests_properties(Bindings.C.*/BPWriteAggregateReadLocalTest.Aggregate1D/*.MPI PROPERTIES  PROCESSORS "40" _BACKTRACE_TRIPLES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;79;gtest_add_tests;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;9;gtest_add_tests_helper;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;0;")
add_test(Bindings.C.*/BPWriteAggregateReadLocalTest.Aggregate1DBlock0/*.MPI "/usr/local/openmpi/4.1.0/gcc/bin/mpiexec" "-n" "40" "/home/bw0594/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/Test.Bindings.C.WriteAggregateReadLocal.MPI" "--gtest_filter=*/BPWriteAggregateReadLocalTest.Aggregate1DBlock0/*")
set_tests_properties(Bindings.C.*/BPWriteAggregateReadLocalTest.Aggregate1DBlock0/*.MPI PROPERTIES  PROCESSORS "40" _BACKTRACE_TRIPLES "/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/cmake/upstream/GoogleTest.cmake;225;add_test;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/CMakeLists.txt;79;gtest_add_tests;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;9;gtest_add_tests_helper;/home/bw0594/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/testing/adios2/bindings/C/CMakeLists.txt;0;")