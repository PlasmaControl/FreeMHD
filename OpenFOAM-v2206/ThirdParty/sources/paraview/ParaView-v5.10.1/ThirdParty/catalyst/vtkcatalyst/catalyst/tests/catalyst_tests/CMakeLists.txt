add_subdirectory(impls)

add_executable(test_double_impl test_double_impl.c)
target_link_libraries(test_double_impl catalyst::catalyst)
add_test(NAME catalyst-impl-double COMMAND test_double_impl)

add_executable(test_catalyst_results catalyst_results/test_catalyst_results.c)
target_link_libraries(test_catalyst_results catalyst::catalyst)
add_test(NAME catalyst-results COMMAND test_catalyst_results)

add_executable(test_catalyst_conduit catalyst_conduit/test_catalyst_conduit.cpp)
target_link_libraries(test_catalyst_conduit catalyst::catalyst)
add_test(NAME catalyst-conduit COMMAND test_catalyst_conduit)
