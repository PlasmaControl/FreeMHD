/*
 * Distributed under OSI-approved BSD 3-Clause License. See
 * accompanying License.txt
 */

#include <catalyst_conduit.hpp>

#include <stdio.h>
#include <stdlib.h>

int test_cpp_node_wrapping_data()
{
  int ret = EXIT_SUCCESS;

  conduit_cpp::Node first_node;
  conduit_node* first_node_c_node = conduit_cpp::c_node(&first_node);
  conduit_cpp::Node second_node = conduit_cpp::cpp_node(first_node_c_node);
  second_node["test"] = 2;

  if (!first_node.has_child("test"))
  {
    ret = EXIT_FAILURE;
  }
  else
  {
    auto child_node = first_node["test"];
    if (!child_node.dtype().is_int())
    {
      ret = EXIT_FAILURE;
    }
    else
    {
      int value = child_node.as_int();
      if (value != 2)
      {
        ret = EXIT_FAILURE;
      }
    }
  }

  return ret;
}

int test_cpp_node_wrapping_on_child_node()
{
  int ret = EXIT_SUCCESS;

  conduit_cpp::Node first_node;
  auto child_node = first_node["test"];
  conduit_node* child_node_c_node = conduit_cpp::c_node(&child_node);
  conduit_cpp::Node child_node_wrapping = conduit_cpp::cpp_node(child_node_c_node);

  child_node_wrapping["sub_key"] = 5;

  if (!first_node.has_path("test/sub_key"))
  {
    ret = EXIT_FAILURE;
  }
  else
  {
    auto sub_node = first_node["test/sub_key"];
    if (!sub_node.dtype().is_int())
    {
      ret = EXIT_FAILURE;
    }
    else
    {
      int value = sub_node.as_int();
      if (value != 5)
      {
        ret = EXIT_FAILURE;
      }
    }
  }

  return ret;
}

int test_cpp_node_wrapping_destructor()
{
  int ret = EXIT_SUCCESS;

  conduit_cpp::Node* first_node = new conduit_cpp::Node();
  conduit_node* first_node_c_node = conduit_cpp::c_node(first_node);
  conduit_cpp::Node second_node = conduit_cpp::cpp_node(first_node_c_node);
  second_node["test"] = 2;

  delete first_node;

  //  // second_node has invalid data, the following lines crash.
  //  auto value = second_node["test"].as_int();
  //  if (value != 2)
  //  {
  //    ret = EXIT_FAILURE;
  //  }

  return ret;
}

int test_cpp_node_copy()
{
  int ret = EXIT_SUCCESS;

  conduit_cpp::Node n;
  conduit_cpp::Node clone = n;

  return ret;
}

int main(int argc, char* argv[])
{
  int ret = EXIT_SUCCESS;

  int test_ret = test_cpp_node_wrapping_data();
  if (test_ret != EXIT_SUCCESS)
  {
    ret = test_ret;
  }

  if (ret == EXIT_SUCCESS)
  {
    test_ret = test_cpp_node_wrapping_on_child_node();
    if (test_ret != EXIT_SUCCESS)
    {
      ret = test_ret;
    }
  }

  if (ret == EXIT_SUCCESS)
  {
    test_ret = test_cpp_node_wrapping_destructor();
    if (test_ret != EXIT_SUCCESS)
    {
      ret = test_ret;
    }
  }

  if (ret == EXIT_SUCCESS)
  {
    test_ret = test_cpp_node_copy();
    if (test_ret != EXIT_SUCCESS)
    {
      ret = test_ret;
    }
  }

  return ret;
}
