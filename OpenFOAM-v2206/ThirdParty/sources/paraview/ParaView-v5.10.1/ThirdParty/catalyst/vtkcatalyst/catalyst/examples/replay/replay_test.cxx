/*
 * Distributed under OSI-approved BSD 3-Clause License. See
 * accompanying License.txt
 */

#include <catalyst.hpp>

#ifdef CATALYST_USE_MPI
#include "mpi.h"
#endif

#include <iostream>

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    return EXIT_FAILURE;
  }

#ifdef CATALYST_USE_MPI
  MPI_Init(&argc, &argv);
#endif

  conduit_cpp::Node node;
  int ret = EXIT_SUCCESS;
  enum catalyst_status err;

  int a = 10;
  node["data"].set_int32(a);
  node["stage"].set_char8_str("initialize");
#ifdef CATALYST_USE_MPI
  node["mpi_comm"].set_int64(static_cast<int64_t>(MPI_Comm_c2f(MPI_COMM_WORLD)));
#endif
  node["catalyst_load"]["implementation"].set_char8_str("example_replay_adaptor");
  node["catalyst_load"]["search_paths"]["example"].set_char8_str(argv[1]);
  err = catalyst_initialize(conduit_cpp::c_node(&node));
  if (err != catalyst_status_ok)
  {
    ret = EXIT_FAILURE;
  }

  node["stage"].set_char8_str("execute");
  node["data"] = ++a;
  for (unsigned i = 0; i < 3; i++)
  {
    err = catalyst_execute(conduit_cpp::c_node(&node));
    if (err != catalyst_status_ok)
    {
      ret = EXIT_FAILURE;
    }
  }

  node["stage"].set_char8_str("finalize");
  node["data"] = ++a;
  err = catalyst_finalize(conduit_cpp::c_node(&node));
  if (err != catalyst_status_ok)
  {
    ret = EXIT_FAILURE;
  }

  node["stage"].set_char8_str("about");
  err = catalyst_about(conduit_cpp::c_node(&node));
  if (err != catalyst_status_ok)
  {
    ret = EXIT_FAILURE;
  }

#ifdef CATALYST_USE_MPI
  MPI_Finalize();
#endif

  return EXIT_SUCCESS;
}
