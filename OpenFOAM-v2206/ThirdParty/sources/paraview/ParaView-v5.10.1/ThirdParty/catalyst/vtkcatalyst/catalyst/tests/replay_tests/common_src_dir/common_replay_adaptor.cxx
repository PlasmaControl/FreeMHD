/*
 * Distributed under OSI-approved BSD 3-Clause License. See
 * accompanying License.txt
 */
#include <catalyst.hpp>
#include <catalyst_stub.h>
#include <sstream>
#include <stdexcept>

#include "catalyst_impl_replay.h"

// Call the stub implementation and check the node passed in.

//-----------------------------------------------------------------------------
enum catalyst_status catalyst_initialize_replay(const conduit_node* params)
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

  return err;
}

//-----------------------------------------------------------------------------
enum catalyst_status catalyst_execute_replay(const conduit_node* params)
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
enum catalyst_status catalyst_finalize_replay(const conduit_node* params)
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
enum catalyst_status catalyst_results_replay(conduit_node* params)
{
  return catalyst_stub_results(params);
}

//-----------------------------------------------------------------------------
enum catalyst_status catalyst_about_replay(conduit_node* params)
{
  return catalyst_stub_about(params);
}
