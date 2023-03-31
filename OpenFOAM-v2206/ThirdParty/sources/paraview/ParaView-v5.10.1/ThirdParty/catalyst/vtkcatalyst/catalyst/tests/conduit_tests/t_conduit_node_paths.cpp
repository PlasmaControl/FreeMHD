//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2014-2019, Lawrence Livermore National Security, LLC.
//
// Produced at the Lawrence Livermore National Laboratory
//
// LLNL-CODE-666778
//
// All rights reserved.
//
// This file is part of Conduit.
//
// For details, see: http://software.llnl.gov/conduit/.
//
// Please also read conduit/LICENSE
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the disclaimer below.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the disclaimer (as noted below) in the
//   documentation and/or other materials provided with the distribution.
//
// * Neither the name of the LLNS/LLNL nor the names of its contributors may
//   be used to endorse or promote products derived from this software without
//   specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL LAWRENCE LIVERMORE NATIONAL SECURITY,
// LLC, THE U.S. DEPARTMENT OF ENERGY OR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
// IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//-----------------------------------------------------------------------------
///
/// file: conduit_node_paths.cpp
///
//-----------------------------------------------------------------------------

#include "c_typedefs.hpp"
#include "catalyst_conduit.hpp"

#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include <vector>

using namespace conduit_cpp;

//-----------------------------------------------------------------------------
TEST(conduit_node_paths, simple_path)
{
  uint32 a_val = 10;
  uint32 b_val = 20;
  float64 c_val = 30.0;

  Node n;
  n["a"] = a_val;
  n["b"] = b_val;
  n["c"] = c_val;

  EXPECT_TRUE(n.has_path("a"));
  EXPECT_EQ(n["a"].as_uint32(), a_val);

  EXPECT_TRUE(n.has_path("b"));
  EXPECT_EQ(n["b"].as_uint32(), b_val);

  EXPECT_TRUE(n.has_path("c"));
  EXPECT_EQ(n["c"].as_float64(), c_val);

  Node n2;
  n2["g"]["a"] = a_val;
  n2["g"]["b"] = b_val;
  n2["g"]["c"] = c_val;

  EXPECT_TRUE(n2.has_path("g/a"));
  EXPECT_EQ(n2["g/a"].as_uint32(), a_val);
  EXPECT_TRUE(n2.has_path("g/b"));
  EXPECT_EQ(n2["g/b"].as_uint32(), b_val);
  EXPECT_TRUE(n2.has_path("g/c"));
  EXPECT_EQ(n2["g/c"].as_float64(), c_val);
  EXPECT_FALSE(n.has_path("g/d"));
}

//-----------------------------------------------------------------------------
TEST(conduit_node_paths, path_empty_slashes)
{
  Node n;

  n["a/b/c/d/e/f"] = 10;

  n.print();

  Node n_sub = n["a/b/c/d/e/f"];
  EXPECT_EQ(n_sub.to_int64(), 10);

  Node n_sub_2 = n["/a/b/c/d/e/f"];
  EXPECT_EQ(n_sub_2.to_int64(), 10);

  Node n_sub_3 = n["/////a/b/c/d/e/f"];
  EXPECT_EQ(n_sub_3.to_int64(), 10);

  Node n_sub_4 = n["/////a/b/c/////d/e/f"];
  EXPECT_EQ(n_sub_4.to_int64(), 10);

  n.print();
}
