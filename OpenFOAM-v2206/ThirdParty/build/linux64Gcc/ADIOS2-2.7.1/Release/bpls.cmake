cmake_minimum_required(VERSION 3.6)

if(OUTPUT_FILE)
  set(output_arg OUTPUT_FILE "${OUTPUT_FILE}")
endif()
if(ERROR_FILE)
  set(error_arg ERROR_FILE "${ERROR_FILE}")
endif()

execute_process(
  COMMAND /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1/bin/bpls ${INPUT_FILE} ${ARG1} ${ARG2} ${ARG3} ${ARG4} ${ARG5} ${ARG6} ${ARG7} ${ARG8} ${ARG9} ${ARG10} ${ARG11} ${ARG12} ${ARG13} ${ARG14} ${ARG15} ${ARG16} ${ARG17} ${ARG18} ${ARG19} ${ARG20}
  RESULT_VARIABLE result
  ${output_arg}
  ${error_arg}
)

if(NOT result EQUAL 0)
  message(FATAL_ERROR "result: ${result}")
endif()
