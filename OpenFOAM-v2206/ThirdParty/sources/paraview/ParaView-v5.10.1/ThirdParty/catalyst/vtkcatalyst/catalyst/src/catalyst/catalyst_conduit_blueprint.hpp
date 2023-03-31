/*
Copyright (c) 2014-2020, Lawrence Livermore National Security, LLC.

Produced at the Lawrence Livermore National Laboratory

LLNL-CODE-666778

All rights reserved.

This file is part of Conduit.

For details, see: http://software.llnl.gov/conduit/.

Please also read conduit/LICENSE

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the disclaimer below.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the disclaimer (as noted below) in the
  documentation and/or other materials provided with the distribution.

* Neither the name of the LLNS/LLNL nor the names of its contributors may
  be used to endorse or promote products derived from this software without
  specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL LAWRENCE LIVERMORE NATIONAL SECURITY,
LLC, THE U.S. DEPARTMENT OF ENERGY OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

Additional BSD Notice

 1. This notice is required to be provided under our contract with the U.S.
    Department of Energy (DOE). This work was produced at Lawrence
    Livermore National Laboratory under Contract No. DE-AC52-07NA27344 with
    the DOE.

 2. Neither the United States Government nor Lawrence Livermore National
    Security, LLC nor any of their employees, makes any warranty, express
    or implied, or assumes any liability or responsibility for the
    accuracy, completeness, or usefulness of any information, apparatus,
    product, or process disclosed, or represents that its use would not
    infringe privately-owned rights.

 3. Also, reference herein to any specific commercial products, process,
    or services by trade name, trademark, manufacturer or otherwise does
    not necessarily constitute or imply its endorsement, recommendation,
    or favoring by the United States Government or Lawrence Livermore
    National Security, LLC. The views and opinions of authors expressed
    herein do not necessarily state or reflect those of the United
    States Government or Lawrence Livermore National Security, LLC, and
    shall not be used for advertising or product endorsement purposes.
*/

#ifndef catalyst_conduit_blueprint_hpp
#define catalyst_conduit_blueprint_hpp

#include <catalyst_conduit.hpp>
#include <catalyst_conduit_error.hpp>
#include <conduit_blueprint.h>

namespace conduit_cpp
{

class Blueprint
{
public:
  static int verify(const std::string& protocol, const Node& node, Node& info)
  {
    return conduit_blueprint_verify(protocol.c_str(), c_node(&node), c_node(&info));
  }
};

class BlueprintMcArray
{
public:
  static int verify(const Node& node, Node& info)
  {
    return conduit_blueprint_mcarray_verify(c_node(&node), c_node(&info));
  }

  static int verify_sub_protocol(const std::string& protocol, const Node& node, Node& info)
  {
    return conduit_blueprint_mcarray_verify_sub_protocol(
      protocol.c_str(), c_node(&node), c_node(&info));
  }

  static bool is_interleaved(const Node& node)
  {
    return conduit_blueprint_mcarray_is_interleaved(c_node(&node));
  }

  static int to_interleaved(const Node& node, Node& dest)
  {
    return conduit_blueprint_mcarray_to_interleaved(c_node(&node), c_node(&dest));
  }

  static int to_contiguous(const Node& node, Node& dest)
  {
    return conduit_blueprint_mcarray_to_contiguous(c_node(&node), c_node(&dest));
  }

  class Example
  {
  public:
    static void xyz(const std::string& type, conduit_index_t npts, Node& result)
    {
      conduit_blueprint_mcarray_examples_xyz(type.c_str(), npts, c_node(&result));
    }
  };
};

class BlueprintMesh
{
public:
  static int verify(const Node& node, Node& info)
  {
    return conduit_blueprint_mesh_verify(c_node(&node), c_node(&info));
  }

  static int verify_sub_protocol(const std::string& protocol, const Node& node, Node& info)
  {
    return conduit_blueprint_mesh_verify_sub_protocol(
      protocol.c_str(), c_node(&node), c_node(&info));
  }

  static void mesh_generate_index(
    const Node& mesh, const std::string& path, conduit_index_t ndomains, Node& index)
  {
    conduit_blueprint_mesh_generate_index(c_node(&mesh), path.c_str(), ndomains, c_node(&index));
  }

  class Example
  {
  public:
    static void basic(const std::string& type, conduit_index_t nx, conduit_index_t ny,
      conduit_index_t nz, Node& result)
    {
      conduit_blueprint_mesh_examples_basic(type.c_str(), nx, ny, nz, c_node(&result));
    }

    static void braid(const std::string& type, conduit_index_t nx, conduit_index_t ny,
      conduit_index_t nz, Node& result)
    {
      conduit_blueprint_mesh_examples_braid(type.c_str(), nx, ny, nz, c_node(&result));
    }

    static void julia(conduit_index_t nx, conduit_index_t ny, conduit_float64 x_min,
      conduit_float64 x_max, conduit_float64 y_min, conduit_float64 y_max, conduit_float64 c_re,
      conduit_float64 c_im, Node& result)
    {
      conduit_blueprint_mesh_examples_julia(
        nx, ny, x_min, x_max, y_min, y_max, c_re, c_im, c_node(&result));
    }

    static void spiral(conduit_index_t ndomains, Node& result)
    {
      conduit_blueprint_mesh_examples_spiral(ndomains, c_node(&result));
    }

    static void polytess(conduit_index_t nlevels, Node& result)
    {
      conduit_blueprint_mesh_examples_polytess(nlevels, c_node(&result));
    }

    static void misc(const std::string& type, conduit_index_t nx, conduit_index_t ny,
      conduit_index_t nz, Node& result)
    {
      conduit_blueprint_mesh_examples_misc(type.c_str(), nx, ny, nz, c_node(&result));
    }
  };
};

}

#endif
