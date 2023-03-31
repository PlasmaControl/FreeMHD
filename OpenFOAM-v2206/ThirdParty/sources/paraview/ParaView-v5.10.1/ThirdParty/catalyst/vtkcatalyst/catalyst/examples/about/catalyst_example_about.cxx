/*
 * Distributed under OSI-approved BSD 3-Clause License. See
 * accompanying License.txt
 */
#include <cstdlib>
#include <iostream>

#include <catalyst.h>

conduit_node* create_mesh(double time)
{
  auto mesh = conduit_node_create();

  conduit_node_set_path_char8_str(mesh, "coordsets/coords/type", "uniform");
  conduit_node_set_path_int64(mesh, "coordsets/coords/dims/i", 10);
  conduit_node_set_path_int64(mesh, "coordsets/coords/dims/j", 10);
  conduit_node_set_path_int64(mesh, "coordsets/coords/dims/k", 10);
  // add origin and spacing to the coordset (optional)
  conduit_node_set_path_double(mesh, "coordsets/coords/origin/x", -10.0);
  conduit_node_set_path_double(mesh, "coordsets/coords/origin/y", -10.0);
  conduit_node_set_path_double(mesh, "coordsets/coords/spacing/dx", 0.5);
  conduit_node_set_path_double(mesh, "coordsets/coords/spacing/dy", 0.5);
  // add the topology
  // this case is simple b/c it's implicitly derived from the coordinate set
  conduit_node_set_path_char8_str(mesh, "topologies/topo/type", "uniform");

  // reference the coordinate set by name
  conduit_node_set_path_char8_str(mesh, "topologies/topo/coordset", "coords");
  return mesh;
}

int main(int argc, char* argv[])
{
  auto node = conduit_node_create();
  conduit_node_set_path_char8_str(node, "catalyst_load/implementation", "stub");
  conduit_node_set_path_char8_str(node, "catalyst/paraview/scripts/sample", "/tmp/sample.py");
  if (catalyst_initialize(node) != catalyst_status_ok)
  {
    conduit_node_destroy(node);
    return EXIT_FAILURE;
  }
  if (catalyst_about(node) != catalyst_status_ok)
  {
    conduit_node_destroy(node);
    return EXIT_FAILURE;
  }
  // conduit_node_print_detailed(node);

  const int max = 10;
  double time = 0.0;
  for (int ts = 0; ts < max; ++ts, time += 1.0 / (max - 1))
  {
    auto n2 = conduit_node_create();
    conduit_node_set_path_double(n2, "catalyst/time", time);
    conduit_node_set_path_int64(n2, "catalyst/timestep", ts);

    auto mesh = create_mesh(time);
    conduit_node_set_path_char8_str(n2, "catalyst/channels/input/type", "mesh");
    conduit_node_set_path_node(n2, "catalyst/channels/input/data", mesh);
    catalyst_execute(n2);
    conduit_node_destroy(n2);
  }

  catalyst_finalize(node);
  conduit_node_destroy(node);
  return EXIT_SUCCESS;
}
