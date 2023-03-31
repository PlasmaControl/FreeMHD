/*
 * Distributed under OSI-approved BSD 3-Clause License. See
 * accompanying License.txt
 */

#include "catalyst_stub.h"
#include "catalyst_dump_node.hpp"
#include "catalyst_version.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum catalyst_status catalyst_stub_initialize(const conduit_node* params)
{
  dump_node(params, "initialize", 0, false);
  return catalyst_status_ok;
}

enum catalyst_status catalyst_stub_finalize(const conduit_node* params)
{
  dump_node(params, "finalize", 0, false);
  return catalyst_status_ok;
}

enum catalyst_status catalyst_stub_execute(const conduit_node* params)
{
  static unsigned long invocations = 0;
  dump_node(params, "execute", invocations, true);
  invocations++;
  return catalyst_status_ok;
}

enum catalyst_status catalyst_stub_about(conduit_node* params)
{
  conduit_node_set_path_char8_str(params, "catalyst/version", CATALYST_VERSION);
  conduit_node_set_path_char8_str(params, "catalyst/abi_version", CATALYST_ABI_VERSION);
  conduit_node_set_path_char8_str(params, "catalyst/implementation", "stub");
#if defined(CATALYST_USE_MPI)
  conduit_node_set_path_int32(params, "catalyst/use_mpi", 1);
#else
  conduit_node_set_path_int32(params, "catalyst/use_mpi", 0);
#endif

  // add information about all third-party dependencies.
  conduit_node* conduit_info = conduit_node_create();
  conduit_about(conduit_info);
  conduit_node_set_path_node(params, "catalyst/tpl/conduit", conduit_info);
  conduit_node_destroy(conduit_info);

  return catalyst_status_ok;
}

enum catalyst_status catalyst_stub_results(conduit_node* params)
{
  return catalyst_status_ok;
}
