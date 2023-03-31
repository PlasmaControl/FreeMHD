/*
 * Distributed under OSI-approved BSD 3-Clause License. See
 * accompanying License.txt
 */
#include <catalyst.hpp>
#include <catalyst_stub.h>
#include <sstream>
#include <stdexcept>

#ifdef REPLAY_ADAPTOR_USE_MPI
#include <mpi.h>
#endif

#include "catalyst_impl_example_replay_adaptor.h"

// Call the stub implementation and check the node passed in.

//-----------------------------------------------------------------------------
enum catalyst_status catalyst_initialize_example_replay_adaptor(const conduit_node* params)
{
  enum catalyst_status err = catalyst_stub_initialize(params);
  conduit_cpp::Node n = conduit_cpp::cpp_node(const_cast<conduit_node*>(params));

  if (n["stage"].as_string() != "initialize")
  {
    std::stringstream msg;
    msg << "ERROR: Expected node stage to be \"initialize\", got " << n["stage"].as_string()
        << std::endl;

    throw std::runtime_error(msg.str());
  }

  if (n["data"].as_int32() != 10)
  {
    std::stringstream msg;
    msg << "ERROR: Expected node data to be equal to 10, got " << n["data"].as_int32() << std::endl;
    throw std::runtime_error(msg.str());
  }

#ifdef REPLAY_ADAPTOR_USE_MPI
  if (!n.has_path("mpi_comm"))
  {
    throw std::runtime_error("missing mpi_comm!");
  }

  auto comm = MPI_Comm_f2c(n["mpi_comm"].to_int());
  int num_ranks;
  if (MPI_Comm_rank(comm, &num_ranks) != MPI_SUCCESS)
  {
    throw std::runtime_error("invalid mpi_comm!");
  }

#else
  if (n.has_path("mpi_comm"))
  {
    throw std::runtime_error("unexpected mpi_comm!");
  }
#endif

  return err;
}

//-----------------------------------------------------------------------------
enum catalyst_status catalyst_execute_example_replay_adaptor(const conduit_node* params)
{
  enum catalyst_status err = catalyst_stub_execute(params);
  conduit_cpp::Node n = conduit_cpp::cpp_node(const_cast<conduit_node*>(params));

  if (n["stage"].as_string() != "execute")
  {
    std::stringstream msg;
    msg << "ERROR: Expected node stage to be \"execute\", got " << n["stage"].as_string()
        << std::endl;

    throw std::runtime_error(msg.str());
  }

  if (n["data"].as_int32() != 11)
  {
    std::stringstream msg;
    msg << "ERROR: Expected node data to be equal to " << 11 << ", got " << n["data"].as_int32()
        << std::endl;
    throw std::runtime_error(msg.str());
  }

  return err;
}

//-----------------------------------------------------------------------------
enum catalyst_status catalyst_finalize_example_replay_adaptor(const conduit_node* params)
{
  enum catalyst_status err = catalyst_stub_finalize(params);
  conduit_cpp::Node n = conduit_cpp::cpp_node(const_cast<conduit_node*>(params));

  if (n["stage"].as_string() != "finalize")
  {
    std::stringstream msg;
    msg << "ERROR: Expected node stage to be \"finalize\", got " << n["stage"].as_string()
        << std::endl;

    throw std::runtime_error(msg.str());
  }

  if (n["data"].as_int32() != 12)
  {
    std::stringstream msg;
    msg << "ERROR: Expected node data to be equal to 12, got " << n["data"].as_int32() << std::endl;

    throw std::runtime_error(msg.str());
  }

  return err;
}

//-----------------------------------------------------------------------------
enum catalyst_status catalyst_results_example_replay_adaptor(conduit_node* params)
{
  return catalyst_stub_results(params);
}

//-----------------------------------------------------------------------------
enum catalyst_status catalyst_about_example_replay_adaptor(conduit_node* params)
{
  return catalyst_stub_about(params);
}
