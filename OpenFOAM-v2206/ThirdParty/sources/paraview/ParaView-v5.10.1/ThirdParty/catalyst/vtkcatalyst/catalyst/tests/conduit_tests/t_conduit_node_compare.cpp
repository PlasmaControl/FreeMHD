//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2014-2017, Lawrence Livermore National Security, LLC.
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
/// file: t_conduit_node_compare.cpp
///
//-----------------------------------------------------------------------------

#include "c_typedefs.hpp"
#include "catalyst_conduit.hpp"

#include "gtest/gtest.h"
#include <algorithm>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

using namespace conduit_cpp;

// TODO(JRC): Update these tests to more extensively test the 'Node:diff_compatible'
// function and its behavior to only compare against its own contents.

/// Helper Functions ///

std::string to_string(index_t index)
{
  std::ostringstream oss;
  oss << index;
  return oss.str();
}

/// Wrapper Functions ///

bool diff_nodes(const Node& lnode, const Node& rnode, Node& info)
{
  return lnode.diff(rnode, info, 0.0);
}

bool compatible_diff_nodes(const Node& lnode, const Node& rnode, Node& info)
{
  return lnode.diff_compatible(rnode, info, 0.0);
}

typedef bool (*NodeDiffFun)(const Node&, const Node&, Node&);
typedef int (*CheckDatatypeFun)(const conduit_datatype* cdatatype);

const NodeDiffFun NODE_DIFF_FUNS[2] = { diff_nodes, compatible_diff_nodes };

/// Testing Functions ///

//-----------------------------------------------------------------------------
TEST(conduit_node_compare, compare_basic)
{
  for (index_t fi = 0; fi < 2; fi++)
  {
    NodeDiffFun diff_nodes = NODE_DIFF_FUNS[fi];

    { // Self-Similarity Test //
      Node n, info;
      n.set("");
      EXPECT_FALSE(diff_nodes(n, n, info));
    }

    { // Basic Difference Test //
      Node n, o, info;
      n.set(1);
      o.set(2);
      EXPECT_TRUE(diff_nodes(n, o, info));
    }

    { // Complex Difference Test //
      Node n, o, info;

      int data[3] = { 1, 2, 3 };
      n.set(data, 2);
      o.set(data, 3);

      if (diff_nodes == compatible_diff_nodes)
      {
        EXPECT_FALSE(diff_nodes(n, o, info));
        EXPECT_TRUE(diff_nodes(o, n, info));
      }
      else
      {
        EXPECT_TRUE(diff_nodes(n, o, info));
      }
    }
  }
}
void compare_leaf_numeric(
  Node& n, Node& o, NodeDiffFun diff_nodes, CheckDatatypeFun is_correct_dtype)
{
  { // Leaf Similarity Test //
    Node info;
    EXPECT_FALSE(diff_nodes(n, o, info));
  }

  { // Leaf Difference Test //
    Node info;
    memset(o.element_ptr(0), 1, 1);
    memset(o.element_ptr(4), 1, 1);
    EXPECT_TRUE(diff_nodes(n, o, info));

    Node info_diff = info["value"];
    EXPECT_EQ(is_correct_dtype(info_diff.c_dtype()), 1);
  }
}
//-----------------------------------------------------------------------------
TEST(conduit_node_compare, compare_leaf_numeric)
{
  for (index_t fi = 0; fi < 2; fi++)
  {
    NodeDiffFun diff_nodes = NODE_DIFF_FUNS[fi];

    Node n, o;

    // int8
    n.set(std::vector<int8>(5, 0));
    o.set(n);
    compare_leaf_numeric(n, o, diff_nodes, &conduit_datatype_is_int8);
    for (index_t vi = 0; vi < 5; vi++)
    {
      bool should_uneq = vi == 0 || vi == 4;
      bool are_uneq = n.as_int8_ptr()[vi] != o.as_int8_ptr()[vi];
      EXPECT_EQ(are_uneq, should_uneq);
    }

    // int16
    n.set(std::vector<int16>(5, 0));
    o.set(n);
    compare_leaf_numeric(n, o, diff_nodes, &conduit_datatype_is_int16);
    for (index_t vi = 0; vi < 5; vi++)
    {
      bool should_uneq = vi == 0 || vi == 4;
      bool are_uneq = n.as_int16_ptr()[vi] != o.as_int16_ptr()[vi];
      EXPECT_EQ(are_uneq, should_uneq);
    }

    // int32
    n.set(std::vector<int32>(5, 0));
    o.set(n);
    compare_leaf_numeric(n, o, diff_nodes, &conduit_datatype_is_int32);
    for (index_t vi = 0; vi < 5; vi++)
    {
      bool should_uneq = vi == 0 || vi == 4;
      bool are_uneq = n.as_int32_ptr()[vi] != o.as_int32_ptr()[vi];
      EXPECT_EQ(are_uneq, should_uneq);
    }

    // int64
    n.set(std::vector<int64>(5, 0));
    o.set(n);
    compare_leaf_numeric(n, o, diff_nodes, &conduit_datatype_is_int64);
    for (index_t vi = 0; vi < 5; vi++)
    {
      bool should_uneq = vi == 0 || vi == 4;
      bool are_uneq = n.as_int64_ptr()[vi] != o.as_int64_ptr()[vi];
      EXPECT_EQ(are_uneq, should_uneq);
    }

    // uint8
    n.set(std::vector<uint8>(5, 0));
    o.set(n);
    compare_leaf_numeric(n, o, diff_nodes, &conduit_datatype_is_uint8);
    for (index_t vi = 0; vi < 5; vi++)
    {
      bool should_uneq = vi == 0 || vi == 4;
      bool are_uneq = n.as_uint8_ptr()[vi] != o.as_uint8_ptr()[vi];
      EXPECT_EQ(are_uneq, should_uneq);
    }

    // uint16
    n.set(std::vector<uint16>(5, 0));
    o.set(n);
    compare_leaf_numeric(n, o, diff_nodes, &conduit_datatype_is_uint16);
    for (index_t vi = 0; vi < 5; vi++)
    {
      bool should_uneq = vi == 0 || vi == 4;
      bool are_uneq = n.as_uint16_ptr()[vi] != o.as_uint16_ptr()[vi];
      EXPECT_EQ(are_uneq, should_uneq);
    }

    // uint32
    n.set(std::vector<uint32>(5, 0));
    o.set(n);
    compare_leaf_numeric(n, o, diff_nodes, &conduit_datatype_is_uint32);
    for (index_t vi = 0; vi < 5; vi++)
    {
      bool should_uneq = vi == 0 || vi == 4;
      bool are_uneq = n.as_uint32_ptr()[vi] != o.as_uint32_ptr()[vi];
      EXPECT_EQ(are_uneq, should_uneq);
    }

    // uint64
    n.set(std::vector<uint64>(5, 0));
    o.set(n);
    compare_leaf_numeric(n, o, diff_nodes, &conduit_datatype_is_uint64);
    for (index_t vi = 0; vi < 5; vi++)
    {
      bool should_uneq = vi == 0 || vi == 4;
      bool are_uneq = n.as_uint64_ptr()[vi] != o.as_uint64_ptr()[vi];
      EXPECT_EQ(are_uneq, should_uneq);
    }

    // float32
    n.set(std::vector<float32>(5, 0));
    o.set(n);
    compare_leaf_numeric(n, o, diff_nodes, &conduit_datatype_is_float32);
    for (index_t vi = 0; vi < 5; vi++)
    {
      bool should_uneq = vi == 0 || vi == 4;
      bool are_uneq = n.as_float32_ptr()[vi] != o.as_float32_ptr()[vi];
      EXPECT_EQ(are_uneq, should_uneq);
    }

    // float64
    n.set(std::vector<float64>(5, 0));
    o.set(n);
    compare_leaf_numeric(n, o, diff_nodes, &conduit_datatype_is_float64);
    for (index_t vi = 0; vi < 5; vi++)
    {
      bool should_uneq = vi == 0 || vi == 4;
      bool are_uneq = n.as_float64_ptr()[vi] != o.as_float64_ptr()[vi];
      EXPECT_EQ(are_uneq, should_uneq);
    }
  }
}

//-----------------------------------------------------------------------------
TEST(conduit_node_compare, compare_leaf_string)
{
  const std::string compare_strs[4] = { "I", "me", "You", "tHeM" };
  size_t leaf_buff_size = 10;
  char compare_buffs[5][leaf_buff_size];

  for (index_t fi = 0; fi < 2; fi++)
  {
    NodeDiffFun diff_nodes = NODE_DIFF_FUNS[fi];

    for (index_t ci = 0; ci < 4; ci++)
    {
      std::string leaf_str = compare_strs[ci];
      // NOTE(JRC): This test applies a buffer offset to the data being
      // tested to push the data to the end of the buffer. For an
      // example test phrase "me", this buffer looks like the following:
      //         leaf_buff
      //   [ _ _ _ _ _ _ _ m e / ]
      //     0 1 2 3 4 5 6 7 8 9

      char* leaf_buff = (char*)&compare_buffs[ci];
      char* leaf_cstr = (char*)&compare_buffs[ci + 1] - leaf_str.length() - 1;
      memset(leaf_buff, 0, leaf_buff_size);
      snprintf(leaf_cstr, leaf_buff_size, "%s", leaf_str.c_str());

      Node n;
      n.set_external_char_ptr(leaf_buff, leaf_str.length() + 1, 10 - leaf_str.length() - 1);

      { // String Similarity Test //
        Node o, info;
        o.set_external(n);
        EXPECT_FALSE(diff_nodes(n, o, info));
      }

      { // String Difference Test //
        char* diff_buff = (char*)&compare_buffs[4];
        memcpy(diff_buff, leaf_buff, leaf_buff_size);
        diff_buff[8] += 1;

        Node o, info;
        o.set_external_char_ptr(diff_buff, leaf_str.length() + 1, 10 - leaf_str.length() - 1);
        EXPECT_TRUE(diff_nodes(n, o, info));
      }
    }
  }
}

//-----------------------------------------------------------------------------
TEST(conduit_node_compare, compare_leaf_mismatch)
{
  for (index_t fi = 0; fi < 2; fi++)
  {
    NodeDiffFun diff_nodes = NODE_DIFF_FUNS[fi];

    Node n, o, info;

    n = (int32)1;
    o = (int64)1;
    EXPECT_TRUE(diff_nodes(n, o, info));
    EXPECT_TRUE(diff_nodes(o, n, info));

    n = (int64)1;
    o = (uint32)1;
    EXPECT_TRUE(diff_nodes(n, o, info));
    EXPECT_TRUE(diff_nodes(o, n, info));

    n = (uint32)1;
    o = (uint64)1;
    EXPECT_TRUE(diff_nodes(n, o, info));
    EXPECT_TRUE(diff_nodes(o, n, info));

    n = (uint64)1;
    o = (float32)1;
    EXPECT_TRUE(diff_nodes(n, o, info));
    EXPECT_TRUE(diff_nodes(o, n, info));

    n = (float32)1;
    o = (float64)1;
    EXPECT_TRUE(diff_nodes(n, o, info));
    EXPECT_TRUE(diff_nodes(o, n, info));

    n = (float64)1;
    o = (int32)1;
    EXPECT_TRUE(diff_nodes(n, o, info));
    EXPECT_TRUE(diff_nodes(o, n, info));
  }
}

//-----------------------------------------------------------------------------
TEST(conduit_node_compare, compare_object_item_diff)
{
  const index_t n_num_children = 5;

  Node n_ref, o_ref;
  for (index_t ci = 0; ci < n_num_children; ci++)
  {
    std::string cs = to_string(ci);
    n_ref[cs].set(ci);
    o_ref[cs].set(ci + (ci % 2));
  }

  for (index_t fi = 0; fi < 2; fi++)
  {
    NodeDiffFun diff_nodes = NODE_DIFF_FUNS[fi];

    const Node n(n_ref), o(o_ref);
    Node info;
    EXPECT_TRUE(diff_nodes(n, o, info));

    Node info_children = info["children/diff"];
    for (index_t ci = 0; ci < n_num_children; ci++)
    {
      std::string cs = to_string(ci);
      EXPECT_TRUE(info_children.has_child(cs));
      EXPECT_EQ(info_children[cs]["valid"].as_string(), (ci % 2 == 0) ? "true" : "false");
    }
  }
}

//-----------------------------------------------------------------------------
TEST(conduit_node_compare, compare_object_size_diff)
{
  const index_t n_num_children = 5;

  Node n_ref;
  for (index_t ci = 0; ci < n_num_children; ci++)
  {
    n_ref[to_string(ci)].set(ci);
  }

  for (index_t fi = 0; fi < 2; fi++)
  {
    NodeDiffFun diff_nodes = NODE_DIFF_FUNS[fi];

    { // Full vs. Empty Node Test //
      Node n(n_ref), o, info;
      // Add a node then immediately remove to make an
      // empty node with type object
      // TODO: Add dtypes to initialize directly from object?
      o["a"] = 5;
      o.remove("a");
      EXPECT_TRUE(diff_nodes(n, o, info));

      EXPECT_EQ(info["valid"].as_string(), "false");
      EXPECT_TRUE(info["children"].has_child("extra"));
      EXPECT_FALSE(info["children"].has_child("missing"));
      EXPECT_FALSE(info["children"].has_child("diff"));

      Node info_extra = info["children/extra"];
      EXPECT_EQ(info_extra.number_of_children(), n_num_children);
    }

    { // Equal Node Test //
      Node n(n_ref), o(n_ref), info;
      EXPECT_FALSE(diff_nodes(n, o, info));

      EXPECT_EQ(info["valid"].as_string(), "true");
      EXPECT_TRUE(info["children"].has_child("diff"));
      EXPECT_FALSE(info["children"].has_child("extra"));
      EXPECT_FALSE(info["children"].has_child("missing"));

      Node info_diff = info["children/diff"];
      EXPECT_EQ(info_diff.number_of_children(), n_num_children);

      for (index_t ci = 0; ci < n_num_children; ci++)
      {
        std::string cs = to_string(ci);
        EXPECT_TRUE(info_diff.has_child(cs));
      }
    }

    { // Half-Full Node Check //
      Node n(n_ref), o(n_ref), info;
      for (index_t ci = 0; ci < n_num_children; ci++)
      {
        if (ci % 2 == 1)
        {
          o.remove(to_string(ci));
        }
      }

      EXPECT_TRUE(diff_nodes(n, o, info));

      EXPECT_EQ(info["valid"].as_string(), "false");
      EXPECT_TRUE(info["children"].has_child("diff"));
      EXPECT_TRUE(info["children"].has_child("extra"));
      EXPECT_FALSE(info["children"].has_child("missing"));

      Node info_extra = info["children/extra"];
      EXPECT_EQ(info_extra.number_of_children(), n_num_children / 2);

      Node info_diff = info["children/diff"];
      for (index_t ci = 0; ci < n_num_children; ci++)
      {
        if (ci % 2 != 1)
        {
          EXPECT_TRUE(info_diff.has_child(to_string(ci)));
        }
      }
    }
  }
}

//-----------------------------------------------------------------------------
TEST(conduit_node_compare, compare_list_item_diff)
{
  const index_t n_num_children = 5;

  Node n_ref, o_ref, info;
  for (index_t ci = 0; ci < n_num_children; ci++)
  {
    n_ref.append().set(ci);
    o_ref.append().set(ci + (ci % 2));
  }

  for (index_t fi = 0; fi < 2; fi++)
  {
    NodeDiffFun diff_nodes = NODE_DIFF_FUNS[fi];

    const Node n(n_ref), o(o_ref);
    Node info;
    EXPECT_TRUE(diff_nodes(n, o, info));

    Node info_children = info["children/diff"];
    for (index_t ci = 0; ci < n_num_children; ci++)
    {
      EXPECT_EQ(info_children.child(ci)["valid"].as_string(), (ci % 2 == 0) ? "true" : "false");
    }
  }
}

//-----------------------------------------------------------------------------
TEST(conduit_node_compare, compare_list_size_diff)
{
  const index_t n_num_children = 5;

  Node n_ref;
  for (index_t ci = 0; ci < n_num_children; ci++)
  {
    n_ref.append().set(ci);
  }

  for (index_t fi = 0; fi < 2; fi++)
  {
    NodeDiffFun diff_nodes = NODE_DIFF_FUNS[fi];

    { // Full vs. Empty Node Check //
      Node n(n_ref), o, info;
      // Add a node then immediately remove to make an
      // empty node with type list
      // TODO: Add dtypes to initialize directly from list?
      o.append().set(1);
      o.remove(0);
      EXPECT_TRUE(diff_nodes(n, o, info));

      EXPECT_EQ(info["valid"].as_string(), "false");
      EXPECT_TRUE(info["children"].has_child("extra"));
      EXPECT_FALSE(info["children"].has_child("missing"));
      EXPECT_FALSE(info["children"].has_child("diff"));

      Node info_extra = info["children/extra"];
      EXPECT_EQ(info_extra.number_of_children(), n_num_children);
    }

    { // Equal Node Check //
      Node n(n_ref), o(n_ref), info;
      EXPECT_FALSE(diff_nodes(n, o, info));

      EXPECT_EQ(info["valid"].as_string(), "true");
      EXPECT_TRUE(info["children"].has_child("diff"));
      EXPECT_FALSE(info["children"].has_child("extra"));
      EXPECT_FALSE(info["children"].has_child("missing"));

      Node info_diff = info["children/diff"];
      EXPECT_EQ(info_diff.number_of_children(), n_num_children);
    }

    { // Half-Full Node Check //
      Node n(n_ref), o(n_ref), info;
      for (index_t ci = n_num_children - 1; ci >= n_num_children / 2; ci--)
      {
        o.remove(ci);
      }

      EXPECT_TRUE(diff_nodes(n, o, info));

      EXPECT_EQ(info["valid"].as_string(), "false");
      EXPECT_TRUE(info["children"].has_child("diff"));
      EXPECT_TRUE(info["children"].has_child("extra"));
      EXPECT_FALSE(info["children"].has_child("missing"));

      Node info_extra = info["children/extra"];
      EXPECT_EQ(info_extra.number_of_children(), n_num_children / 2 + 1);

      Node info_diff = info["children/diff"];
      EXPECT_EQ(info_diff.number_of_children(), n_num_children / 2);
    }
  }
}

// The following two tests were taken from conduit's
// t_conduit_schema testing file.
//-----------------------------------------------------------------------------
TEST(conduit_node_compare, compatible_nodes)
{
  Node n1, n2, n3;
  n1["a"].set(std::vector<int64>(10, 0));
  n1["b"].set(std::vector<float64>(20, 0));

  n2["a"].set(std::vector<int64>(10, 0));
  n2["b"].set(std::vector<float64>(20, 0));
  EXPECT_TRUE(n1.compatible(n2));

  n3["a"].set(std::vector<int64>(10, 0));
  n3["b"].set(std::vector<float64>(40, 0));
  EXPECT_FALSE(n1.compatible(n3));
  EXPECT_TRUE(n3.compatible(n1));
}

//-----------------------------------------------------------------------------
TEST(conduit_node_compare, compatible_nodes_with_lists)
{
  Node n1;
  Node n1_a = n1.append();
  Node n1_b = n1.append();

  n1_a.set(std::vector<int8>(0, 10));
  n1_b.set(std::vector<int8>(0, 10));

  Node n2;
  Node n2_a = n2.append();
  Node n2_b = n2.append();
  Node n2_c = n2.append();

  n2_a.set(std::vector<int8>(0, 10));
  n2_b.set(std::vector<int8>(0, 10));
  n2_c.set(std::vector<int8>(0, 10));

  EXPECT_FALSE(n1.compatible(n2));
  EXPECT_TRUE(n2.compatible(n1));

  EXPECT_TRUE(n1.compatible(n1));
}

//-----------------------------------------------------------------------------
TEST(conduit_node_compare, compatible_nodes_dtype)
{
  Node obj, list, leaf;
  obj["a"].set_int32(10);
  list.append().set_int32(10);
  leaf.set_int32(10);

  EXPECT_FALSE(obj.compatible(list));
  EXPECT_FALSE(obj.compatible(leaf));

  EXPECT_FALSE(list.compatible(obj));
  EXPECT_FALSE(list.compatible(leaf));

  EXPECT_FALSE(leaf.compatible(obj));
  EXPECT_FALSE(leaf.compatible(list));

  // Leaves with different types should also be incompatible
  Node leaf2;
  leaf2.set_float32(10);
  EXPECT_FALSE(leaf.compatible(leaf2));
  EXPECT_FALSE(leaf2.compatible(leaf));
}
