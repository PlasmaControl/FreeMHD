// Copyright (c) Lawrence Livermore National Security, LLC and other Conduit
// Project developers. See top-level LICENSE AND COPYRIGHT files for dates and
// other details. No copyright assignment is required to contribute to Conduit.

//-----------------------------------------------------------------------------
///
/// file: conduit_to_string.cpp
///
//-----------------------------------------------------------------------------

#include "catalyst_conduit.hpp"

#include "gtest/gtest.h"
#include <iostream>
using namespace conduit_cpp;

//-----------------------------------------------------------------------------
TEST(conduit_to_string, simple_1)
{
  conduit_uint32 a_val = 10;
  conduit_uint32 b_val = 20;
  conduit_float64 c_val = 30.0;

  const std::string n_expected = "\n"
                                 "a: 10\n"
                                 "b: 20\n"
                                 "c: 30.0\n"
                                 "\n";

  const std::string n2_expected = "\n"
                                  "g: \n"
                                  "  a: 10\n"
                                  "  b: 20\n"
                                  "  c: 30.0\n"
                                  "\n";

  Node n;
  n["a"] = a_val;
  n["b"] = b_val;
  n["c"] = c_val;

  Node n2;
  n2["g"]["a"] = a_val;
  n2["g"]["b"] = b_val;
  n2["g"]["c"] = c_val;

  // Capture stdout using a stringstream
  std::stringstream buffer;
  std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

  // Check n
  n.print();
  EXPECT_EQ(std::string(n_expected), buffer.str());

  buffer.str(std::string());
  buffer.clear();

  // Check n2
  n2.print();
  EXPECT_EQ(std::string(n2_expected), buffer.str());

  std::cout.rdbuf(old_buf);

  n.print();
  n.print_detailed();
  n2.print();
  n2.print_detailed();
}
