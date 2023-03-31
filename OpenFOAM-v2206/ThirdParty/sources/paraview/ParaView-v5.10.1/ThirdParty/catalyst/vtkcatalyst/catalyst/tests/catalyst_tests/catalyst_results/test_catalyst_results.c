/*
 * Distributed under OSI-approved BSD 3-Clause License. See
 * accompanying License.txt
 */

#include <catalyst.h>

#include <stdio.h>
#include <stdlib.h>

int test_simple_catalyst_results_call()
{
  int ret = EXIT_SUCCESS;
  enum catalyst_status err;

  conduit_node* params = conduit_node_create();
  conduit_node_set_path_double(params, "data", 1.);
  conduit_node_set_path_char8_str(params, "catalyst_load/implementation", "double");
  err = catalyst_initialize(params);
  conduit_node_destroy(params);
  if (err != catalyst_status_ok)
  {
    fprintf(stderr, "failed to initialize: %d\n", err);
    ret = EXIT_FAILURE;
    return ret;
  }

  conduit_node* about = conduit_node_create();
  err = catalyst_about(about);
  if (err != catalyst_status_ok)
  {
    fprintf(stderr, "failed to call `about`: %d\n", err);
    ret = EXIT_FAILURE;
  }
  if (conduit_node_fetch_path_as_double(about, "data") != 1.)
  {
    fprintf(stderr, "failed to get `data` from `about`: %d\n", err);
    ret = EXIT_FAILURE;
  }
  conduit_node_destroy(about);

  conduit_node* exec = conduit_node_create();
  conduit_node_set_path_double(exec, "data", 2.);
  err = catalyst_execute(exec);
  if (err != catalyst_status_ok)
  {
    fprintf(stderr, "failed to call `execute`: %d\n", err);
    ret = EXIT_FAILURE;
  }
  conduit_node_destroy(exec);

  conduit_node* results = conduit_node_create();
  err = catalyst_results(results);
  if (err != catalyst_status_ok)
  {
    fprintf(stderr, "failed to call `results`: %d\n", err);
    ret = EXIT_FAILURE;
  }
  if (conduit_node_fetch_path_as_double(results, "data") != 2.)
  {
    fprintf(stderr, "failed to get `data` from `results`: %d\n", err);
    ret = EXIT_FAILURE;
  }
  conduit_node_destroy(results);

  conduit_node* final = conduit_node_create();
  err = catalyst_finalize(final);
  if (err != catalyst_status_ok)
  {
    fprintf(stderr, "failed to call `finalize`: %d\n", err);
    ret = EXIT_FAILURE;
  }
  if (conduit_node_fetch_path_as_double(final, "data") != 2.)
  {
    fprintf(stderr, "failed to get `data` from `finalize`: %d\n", err);
    ret = EXIT_FAILURE;
  }
  conduit_node_destroy(final);

  return ret;
}

int main(int argc, char* argv[])
{
  int ret = EXIT_SUCCESS;

  int test_ret = test_simple_catalyst_results_call();
  if (test_ret != EXIT_SUCCESS)
  {
    ret = test_ret;
  }

  return ret;
}
