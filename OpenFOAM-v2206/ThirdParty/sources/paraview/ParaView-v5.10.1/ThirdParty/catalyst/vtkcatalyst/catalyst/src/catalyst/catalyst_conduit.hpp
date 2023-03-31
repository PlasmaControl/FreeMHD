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

#ifndef catalyst_conduit_hpp
#define catalyst_conduit_hpp

#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <catalyst_conduit_error.hpp>
#include <conduit.h>

namespace conduit_cpp
{

#define CATALYST_CONDUIT_ERROR(msg) throw Error(msg, std::string(__FILE__), __LINE__);

#define CATALYST_CONDUIT_WRAP_EXCEP(throwing_code)                                                 \
  try                                                                                              \
  {                                                                                                \
    throwing_code;                                                                                 \
  }                                                                                                \
  catch (...)                                                                                      \
  {                                                                                                \
    CATALYST_CONDUIT_ERROR("An error occurred at:")                                                \
  }

class Node;

class DataType
{
public:
  ~DataType() = default;

  enum class Id
  {
    int8,
    int16,
    int32,
    int64,

    uint8,
    uint16,
    uint32,
    uint64,

    float32,
    float64,

    unknown,
  };

  friend class Node;

  bool is_valid() const { return this->c_dtype != nullptr; }

  Id id() const
  {
    if (!this->is_valid())
    {
      return Id::unknown;
    }

    if (this->is_int8())
    {
      return Id::int8;
    }
    if (this->is_int16())
    {
      return Id::int16;
    }
    if (this->is_int32())
    {
      return Id::int32;
    }
    if (this->is_int64())
    {
      return Id::int64;
    }

    if (this->is_uint8())
    {
      return Id::uint8;
    }
    if (this->is_uint16())
    {
      return Id::uint16;
    }
    if (this->is_uint32())
    {
      return Id::uint32;
    }
    if (this->is_uint64())
    {
      return Id::uint64;
    }

    if (this->is_float32())
    {
      return Id::float32;
    }
    if (this->is_float64())
    {
      return Id::float64;
    }

    return Id::unknown;
  }

  bool is_empty() const { return conduit_datatype_is_empty(this->c_dtype); }
  bool is_object() const { return conduit_datatype_is_object(this->c_dtype); }
  bool is_list() const { return conduit_datatype_is_list(this->c_dtype); }

  conduit_index_t type_id() const { return conduit_datatype_id(this->c_dtype); }
  std::string name() const
  {
    char* c_name = conduit_datatype_name(this->c_dtype);
    std::string name = c_name;
    conduit_datatype_name_destroy(c_name);
    return name;
  }

  conduit_index_t number_of_elements() const
  {
    return conduit_datatype_number_of_elements(this->c_dtype);
  }
  conduit_index_t offset() const { return conduit_datatype_offset(this->c_dtype); }
  conduit_index_t stride() const { return conduit_datatype_stride(this->c_dtype); }
  conduit_index_t element_bytes() const { return conduit_datatype_element_bytes(this->c_dtype); }
  conduit_index_t endianness() const { return conduit_datatype_endianness(this->c_dtype); }
  conduit_index_t element_index(conduit_index_t idx) const
  {
    return conduit_datatype_element_index(this->c_dtype, idx);
  }

  bool is_number() const { return conduit_datatype_is_number(this->c_dtype); }
  bool is_floating_point() const { return conduit_datatype_is_floating_point(this->c_dtype); }
  bool is_integer() const { return conduit_datatype_is_integer(this->c_dtype); }
  bool is_signed_integer() const { return conduit_datatype_is_signed_integer(this->c_dtype); }
  bool is_unsigned_integer() const { return conduit_datatype_is_unsigned_integer(this->c_dtype); }

  bool is_int8() const { return conduit_datatype_is_int8(this->c_dtype); }
  bool is_int16() const { return conduit_datatype_is_int16(this->c_dtype); }
  bool is_int32() const { return conduit_datatype_is_int32(this->c_dtype); }
  bool is_int64() const { return conduit_datatype_is_int64(this->c_dtype); }

  bool is_uint8() const { return conduit_datatype_is_uint8(this->c_dtype); }
  bool is_uint16() const { return conduit_datatype_is_uint16(this->c_dtype); }
  bool is_uint32() const { return conduit_datatype_is_uint32(this->c_dtype); }
  bool is_uint64() const { return conduit_datatype_is_uint64(this->c_dtype); }

  bool is_float32() const { return conduit_datatype_is_float32(this->c_dtype); }
  bool is_float64() const { return conduit_datatype_is_float64(this->c_dtype); }

  bool is_char() const { return conduit_datatype_is_char(this->c_dtype); }
  bool is_short() const { return conduit_datatype_is_short(this->c_dtype); }
  bool is_int() const { return conduit_datatype_is_int(this->c_dtype); }
  bool is_long() const { return conduit_datatype_is_long(this->c_dtype); }

  bool is_unsigned_char() const { return conduit_datatype_is_unsigned_char(this->c_dtype); }
  bool is_unsigned_short() const { return conduit_datatype_is_unsigned_short(this->c_dtype); }
  bool is_unsigned_int() const { return conduit_datatype_is_unsigned_int(this->c_dtype); }
  bool is_unsigned_long() const { return conduit_datatype_is_unsigned_long(this->c_dtype); }

  bool is_float() const { return conduit_datatype_is_float(this->c_dtype); }
  bool is_double() const { return conduit_datatype_is_double(this->c_dtype); }

  bool is_string() const { return conduit_datatype_is_string(this->c_dtype); }
  bool is_char8_str() const { return conduit_datatype_is_char8_str(this->c_dtype); }

  bool is_little_endian() const { return conduit_datatype_is_little_endian(this->c_dtype); }
  bool is_big_endian() const { return conduit_datatype_is_big_endian(this->c_dtype); }
  bool endianness_matches_machine() const
  {
    return conduit_datatype_endianness_matches_machine(this->c_dtype);
  }

private:
  DataType(const conduit_datatype* dtype)
    : c_dtype(dtype)
  {
  }

  const conduit_datatype* c_dtype;
};

class Node
{
public:
  Node()
    : c_node(conduit_node_create())
  {
  }
  Node(const Node& other)
  {
    this->c_node = conduit_node_create();
    this->set(other);
  }
  Node(Node&& other)
    : c_node(other.c_node)
    , own_c_node(other.own_c_node)
  {
    other.c_node = nullptr;
    other.own_c_node = false;
  }
  ~Node()
  {
    if (this->c_node && this->own_c_node)
    {
      conduit_node_destroy(this->c_node);
    }
  }

  friend conduit_node* c_node(const Node* n);
  friend Node cpp_node(conduit_node* n);

  void set_node(const Node& data) { conduit_node_set_node(this->c_node, data.c_node); }

  void set(const Node& data) { this->set_node(data); }

#define catalyst_conduit_setter(dname)                                                             \
  void set_##dname(conduit_##dname data) { conduit_node_set_##dname(this->c_node, data); }         \
  void set(conduit_##dname data) { this->set_##dname(data); }

  catalyst_conduit_setter(int8);
  catalyst_conduit_setter(int16);
  catalyst_conduit_setter(int32);
  catalyst_conduit_setter(int64);

  catalyst_conduit_setter(uint8);
  catalyst_conduit_setter(uint16);
  catalyst_conduit_setter(uint32);
  catalyst_conduit_setter(uint64);

  catalyst_conduit_setter(float32);
  catalyst_conduit_setter(float64);

#undef catalyst_conduit_setter

#define catalyst_conduit_c_setter(cname, joined)                                                   \
  void set(cname data) { conduit_node_set_##joined(this->c_node, data); }

  catalyst_conduit_c_setter(char, char);
#ifndef CONDUIT_USE_CHAR
  catalyst_conduit_c_setter(signed char, signed_char);
  catalyst_conduit_c_setter(unsigned char, unsigned_char);
#endif
#ifndef CONDUIT_USE_SHORT
  catalyst_conduit_c_setter(signed short, signed_short);
  catalyst_conduit_c_setter(unsigned short, unsigned_short);
#endif
#ifndef CONDUIT_USE_INT
  catalyst_conduit_c_setter(signed int, signed_int);
  catalyst_conduit_c_setter(unsigned int, unsigned_int);
#endif
#ifndef CONDUIT_USE_LONG
  catalyst_conduit_c_setter(signed long, signed_long);
  catalyst_conduit_c_setter(unsigned long, unsigned_long);
#endif
#ifndef CONDUIT_USE_FLOAT
  catalyst_conduit_c_setter(float, float);
#endif
#ifndef CONDUIT_USE_DOUBLE
  catalyst_conduit_c_setter(double, double);
#endif

#undef catalyst_conduit_c_setter

  void set_string(const std::string& data)
  {
    conduit_node_set_char8_str(this->c_node, data.c_str());
  }

  void set(const std::string& data) { this->set_string(data); }

  void set_char8_str(const char* data) { conduit_node_set_char8_str(this->c_node, data); }

#define catalyst_conduit_setter_vector(dname)                                                      \
  void set_##dname##_vector(const std::vector<conduit_##dname>& data)                              \
  {                                                                                                \
    conduit_node_set_##dname##_ptr(                                                                \
      this->c_node, const_cast<conduit_##dname*>(data.data()), data.size());                       \
  }                                                                                                \
  void set(const std::vector<conduit_##dname>& data) { this->set_##dname##_vector(data); }

  catalyst_conduit_setter_vector(int8);
  catalyst_conduit_setter_vector(int16);
  catalyst_conduit_setter_vector(int32);
  catalyst_conduit_setter_vector(int64);

  catalyst_conduit_setter_vector(uint8);
  catalyst_conduit_setter_vector(uint16);
  catalyst_conduit_setter_vector(uint32);
  catalyst_conduit_setter_vector(uint64);

  catalyst_conduit_setter_vector(float32);
  catalyst_conduit_setter_vector(float64);

#undef catalyst_conduit_setter_vector

#define catalyst_conduit_c_setter_vector(cname, joined)                                            \
  void set(const std::vector<cname>& data)                                                         \
  {                                                                                                \
    conduit_node_set_##joined##_ptr(this->c_node, const_cast<cname*>(data.data()), data.size());   \
  }

  catalyst_conduit_c_setter_vector(char, char);
#ifndef CONDUIT_USE_CHAR
  catalyst_conduit_c_setter_vector(signed char, signed_char);
  catalyst_conduit_c_setter_vector(unsigned char, unsigned_char);
#endif
#ifndef CONDUIT_USE_SHORT
  catalyst_conduit_c_setter_vector(signed short, signed_short);
  catalyst_conduit_c_setter_vector(unsigned short, unsigned_short);
#endif
#ifndef CONDUIT_USE_INT
  catalyst_conduit_c_setter_vector(signed int, signed_int);
  catalyst_conduit_c_setter_vector(unsigned int, unsigned_int);
#endif
#ifndef CONDUIT_USE_LONG
  catalyst_conduit_c_setter_vector(signed long, signed_long);
  catalyst_conduit_c_setter_vector(unsigned long, unsigned_long);
#endif
#ifndef CONDUIT_USE_FLOAT
  catalyst_conduit_c_setter_vector(float, float);
#endif
#ifndef CONDUIT_USE_DOUBLE
  catalyst_conduit_c_setter_vector(double, double);
#endif

#undef catalyst_conduit_c_setter_vector

#define catalyst_conduit_setter_ptr(dname)                                                         \
  void set_##dname##_ptr(const conduit_##dname* data, conduit_index_t num_elements = 1,            \
    conduit_index_t offset = 0, conduit_index_t stride = sizeof(conduit_##dname),                  \
    conduit_index_t element_bytes = sizeof(conduit_##dname), conduit_index_t endianness = 0)       \
  {                                                                                                \
    conduit_node_set_##dname##_ptr_detailed(this->c_node, const_cast<conduit_##dname*>(data),      \
      num_elements, offset, stride, element_bytes, endianness);                                    \
  }                                                                                                \
  void set(const conduit_##dname* data, conduit_index_t num_elements = 1,                          \
    conduit_index_t offset = 0, conduit_index_t stride = sizeof(conduit_##dname),                  \
    conduit_index_t element_bytes = sizeof(conduit_##dname), conduit_index_t endianness = 0)       \
  {                                                                                                \
    this->set_##dname##_ptr(data, num_elements, offset, stride, element_bytes, endianness);        \
  }

  catalyst_conduit_setter_ptr(int8);
  catalyst_conduit_setter_ptr(int16);
  catalyst_conduit_setter_ptr(int32);
  catalyst_conduit_setter_ptr(int64);

  catalyst_conduit_setter_ptr(uint8);
  catalyst_conduit_setter_ptr(uint16);
  catalyst_conduit_setter_ptr(uint32);
  catalyst_conduit_setter_ptr(uint64);

  catalyst_conduit_setter_ptr(float32);
  catalyst_conduit_setter_ptr(float64);

#undef catalyst_conduit_setter_ptr

#define catalyst_conduit_c_setter_ptr(cname, joined)                                               \
  void set(const cname* data, conduit_index_t num_elements = 1, conduit_index_t offset = 0,        \
    conduit_index_t stride = sizeof(cname), conduit_index_t element_bytes = sizeof(cname),         \
    conduit_index_t endianness = 0)                                                                \
  {                                                                                                \
    conduit_node_set_##joined##_ptr_detailed(this->c_node, const_cast<cname*>(data), num_elements, \
      offset, stride, element_bytes, endianness);                                                  \
  }

  void set_char_ptr(const char* data, conduit_index_t num_elements = 1, conduit_index_t offset = 0,
    conduit_index_t stride = sizeof(CONDUIT_NATIVE_CHAR),
    conduit_index_t element_bytes = sizeof(CONDUIT_NATIVE_CHAR), conduit_index_t endianness = 0)
  {
    conduit_node_set_char_ptr_detailed(this->c_node, const_cast<char*>(data), num_elements, offset,
      stride, element_bytes, endianness);
  }
#ifndef CONDUIT_USE_CHAR
  catalyst_conduit_c_setter_ptr(signed char, signed_char);
  catalyst_conduit_c_setter_ptr(unsigned char, unsigned_char);
#endif
#ifndef CONDUIT_USE_SHORT
  catalyst_conduit_c_setter_ptr(signed short, signed_short);
  catalyst_conduit_c_setter_ptr(unsigned short, unsigned_short);
#endif
#ifndef CONDUIT_USE_INT
  catalyst_conduit_c_setter_ptr(signed int, signed_int);
  catalyst_conduit_c_setter_ptr(unsigned int, unsigned_int);
#endif
#ifndef CONDUIT_USE_LONG
  catalyst_conduit_c_setter_ptr(signed long, signed_long);
  catalyst_conduit_c_setter_ptr(unsigned long, unsigned_long);
#endif
#ifndef CONDUIT_USE_FLOAT
  catalyst_conduit_c_setter_ptr(float, float);
#endif
#ifndef CONDUIT_USE_DOUBLE
  catalyst_conduit_c_setter_ptr(double, double);
#endif

#undef catalyst_conduit_c_setter_ptr

  void set_path_node(const std::string& path, const Node& data)
  {
    conduit_node_set_path_node(this->c_node, path.c_str(), data.c_node);
  }

  void set_path(const std::string& path, const Node& data) { this->set_path_node(path, data); }

#define catalyst_conduit_path_setter(dname)                                                        \
  void set_path_##dname(const std::string& path, conduit_##dname data)                             \
  {                                                                                                \
    conduit_node_set_path_##dname(this->c_node, path.c_str(), data);                               \
  }                                                                                                \
  void set_path(const std::string& path, conduit_##dname data)                                     \
  {                                                                                                \
    this->set_path_##dname(path, data);                                                            \
  }

  catalyst_conduit_path_setter(int8);
  catalyst_conduit_path_setter(int16);
  catalyst_conduit_path_setter(int32);
  catalyst_conduit_path_setter(int64);

  catalyst_conduit_path_setter(uint8);
  catalyst_conduit_path_setter(uint16);
  catalyst_conduit_path_setter(uint32);
  catalyst_conduit_path_setter(uint64);

  catalyst_conduit_path_setter(float32);
  catalyst_conduit_path_setter(float64);

#undef catalyst_conduit_path_setter

#define catalyst_conduit_c_path_setter(cname, joined)                                              \
  void set_path(const std::string& path, cname data)                                               \
  {                                                                                                \
    conduit_node_set_path_##joined(this->c_node, path.c_str(), data);                              \
  }

  catalyst_conduit_c_path_setter(char, char);
#ifndef CONDUIT_USE_CHAR
  catalyst_conduit_c_path_setter(signed char, signed_char);
  catalyst_conduit_c_path_setter(unsigned char, unsigned_char);
#endif
#ifndef CONDUIT_USE_SHORT
  catalyst_conduit_c_path_setter(signed short, signed_short);
  catalyst_conduit_c_path_setter(unsigned short, unsigned_short);
#endif
#ifndef CONDUIT_USE_INT
  catalyst_conduit_c_path_setter(signed int, signed_int);
  catalyst_conduit_c_path_setter(unsigned int, unsigned_int);
#endif
#ifndef CONDUIT_USE_LONG
  catalyst_conduit_c_path_setter(signed long, signed_long);
  catalyst_conduit_c_path_setter(unsigned long, unsigned_long);
#endif
#ifndef CONDUIT_USE_FLOAT
  catalyst_conduit_c_path_setter(float, float);
#endif
#ifndef CONDUIT_USE_DOUBLE
  catalyst_conduit_c_path_setter(double, double);
#endif

#undef catalyst_conduit_c_path_setter

  void set_path_string(const std::string& path, const std::string& data)
  {
    conduit_node_set_path_char8_str(this->c_node, path.c_str(), data.c_str());
  }

  void set_path(const std::string& path, const std::string& data)
  {
    this->set_path_string(path, data);
  }

  void set_path_char8_str(const std::string& path, const char* data)
  {
    conduit_node_set_path_char8_str(this->c_node, path.c_str(), data);
  }

#define catalyst_conduit_path_setter_vector(dname)                                                 \
  void set_path_##dname##_vector(                                                                  \
    const std::string& path, const std::vector<conduit_##dname>& data)                             \
  {                                                                                                \
    conduit_node_set_path_##dname##_ptr(                                                           \
      this->c_node, path.c_str(), const_cast<conduit_##dname*>(data.data()), data.size());         \
  }                                                                                                \
  void set_path(const std::string& path, const std::vector<conduit_##dname>& data)                 \
  {                                                                                                \
    this->set_path_##dname##_vector(path, data);                                                   \
  }

  catalyst_conduit_path_setter_vector(int8);
  catalyst_conduit_path_setter_vector(int16);
  catalyst_conduit_path_setter_vector(int32);
  catalyst_conduit_path_setter_vector(int64);

  catalyst_conduit_path_setter_vector(uint8);
  catalyst_conduit_path_setter_vector(uint16);
  catalyst_conduit_path_setter_vector(uint32);
  catalyst_conduit_path_setter_vector(uint64);

  catalyst_conduit_path_setter_vector(float32);
  catalyst_conduit_path_setter_vector(float64);

#undef catalyst_conduit_path_setter_vector

#define catalyst_conduit_c_path_setter_vector(cname, joined)                                       \
  void set_path(const std::string& path, const std::vector<cname>& data)                           \
  {                                                                                                \
    conduit_node_set_path_##joined##_ptr(                                                          \
      this->c_node, path.c_str(), const_cast<cname*>(data.data()), data.size());                   \
  }

  catalyst_conduit_c_path_setter_vector(char, char);
#ifndef CONDUIT_USE_CHAR
  catalyst_conduit_c_path_setter_vector(signed char, signed_char);
  catalyst_conduit_c_path_setter_vector(unsigned char, unsigned_char);
#endif
#ifndef CONDUIT_USE_SHORT
  catalyst_conduit_c_path_setter_vector(signed short, signed_short);
  catalyst_conduit_c_path_setter_vector(unsigned short, unsigned_short);
#endif
#ifndef CONDUIT_USE_INT
  catalyst_conduit_c_path_setter_vector(signed int, signed_int);
  catalyst_conduit_c_path_setter_vector(unsigned int, unsigned_int);
#endif
#ifndef CONDUIT_USE_LONG
  catalyst_conduit_c_path_setter_vector(signed long, signed_long);
  catalyst_conduit_c_path_setter_vector(unsigned long, unsigned_long);
#endif
#ifndef CONDUIT_USE_FLOAT
  catalyst_conduit_c_path_setter_vector(float, float);
#endif
#ifndef CONDUIT_USE_DOUBLE
  catalyst_conduit_c_path_setter_vector(double, double);
#endif

#undef catalyst_conduit_c_path_setter_vector

#define catalyst_conduit_path_setter_ptr(dname)                                                    \
  void set_path_##dname##_ptr(const std::string& path, const conduit_##dname* data,                \
    conduit_index_t num_elements = 1, conduit_index_t offset = 0,                                  \
    conduit_index_t stride = sizeof(conduit_##dname),                                              \
    conduit_index_t element_bytes = sizeof(conduit_##dname), conduit_index_t endianness = 0)       \
  {                                                                                                \
    conduit_node_set_path_##dname##_ptr_detailed(this->c_node, path.c_str(),                       \
      const_cast<conduit_##dname*>(data), num_elements, offset, stride, element_bytes,             \
      endianness);                                                                                 \
  }                                                                                                \
  void set_path(const std::string& path, const conduit_##dname* data,                              \
    conduit_index_t num_elements = 1, conduit_index_t offset = 0,                                  \
    conduit_index_t stride = sizeof(conduit_##dname),                                              \
    conduit_index_t element_bytes = sizeof(conduit_##dname), conduit_index_t endianness = 0)       \
  {                                                                                                \
    this->set_path_##dname##_ptr(                                                                  \
      path, data, num_elements, offset, stride, element_bytes, endianness);                        \
  }

  catalyst_conduit_path_setter_ptr(int8);
  catalyst_conduit_path_setter_ptr(int16);
  catalyst_conduit_path_setter_ptr(int32);
  catalyst_conduit_path_setter_ptr(int64);

  catalyst_conduit_path_setter_ptr(uint8);
  catalyst_conduit_path_setter_ptr(uint16);
  catalyst_conduit_path_setter_ptr(uint32);
  catalyst_conduit_path_setter_ptr(uint64);

  catalyst_conduit_path_setter_ptr(float32);
  catalyst_conduit_path_setter_ptr(float64);

#undef catalyst_conduit_path_setter_ptr

#define catalyst_conduit_c_path_setter_ptr(cname, joined)                                          \
  void set_path(const std::string& path, const cname* data, conduit_index_t num_elements = 1,      \
    conduit_index_t offset = 0, conduit_index_t stride = sizeof(cname),                            \
    conduit_index_t element_bytes = sizeof(cname), conduit_index_t endianness = 0)                 \
  {                                                                                                \
    conduit_node_set_path_##joined##_ptr_detailed(this->c_node, path.c_str(),                      \
      const_cast<cname*>(data), num_elements, offset, stride, element_bytes, endianness);          \
  }

  void set_path_char_ptr(const std::string& path, const char* data,
    conduit_index_t num_elements = 1, conduit_index_t offset = 0,
    conduit_index_t stride = sizeof(char), conduit_index_t element_bytes = sizeof(char),
    conduit_index_t endianness = 0)
  {
    conduit_node_set_path_char_ptr_detailed(this->c_node, path.c_str(), const_cast<char*>(data),
      num_elements, offset, stride, element_bytes, endianness);
  }
#ifndef CONDUIT_USE_CHAR
  catalyst_conduit_c_path_setter_ptr(signed char, signed_char);
  catalyst_conduit_c_path_setter_ptr(unsigned char, unsigned_char);
#endif
#ifndef CONDUIT_USE_SHORT
  catalyst_conduit_c_path_setter_ptr(signed short, signed_short);
  catalyst_conduit_c_path_setter_ptr(unsigned short, unsigned_short);
#endif
#ifndef CONDUIT_USE_INT
  catalyst_conduit_c_path_setter_ptr(signed int, signed_int);
  catalyst_conduit_c_path_setter_ptr(unsigned int, unsigned_int);
#endif
#ifndef CONDUIT_USE_LONG
  catalyst_conduit_c_path_setter_ptr(signed long, signed_long);
  catalyst_conduit_c_path_setter_ptr(unsigned long, unsigned_long);
#endif
#ifndef CONDUIT_USE_FLOAT
  catalyst_conduit_c_path_setter_ptr(float, float);
#endif
#ifndef CONDUIT_USE_DOUBLE
  catalyst_conduit_c_path_setter_ptr(double, double);
#endif

#undef catalyst_conduit_c_path_setter_ptr

  void set_external_node(Node& data) { conduit_node_set_external_node(this->c_node, data.c_node); }

  void set_external(Node& data) { this->set_external_node(data); }

  void set_external_char8_str(char* data)
  {
    conduit_node_set_external_char8_str(this->c_node, data);
  }

#define catalyst_conduit_external_setter_vector(dname)                                             \
  void set_external_##dname##_vector(const std::vector<conduit_##dname>& data)                     \
  {                                                                                                \
    conduit_node_set_external_##dname##_ptr(                                                       \
      this->c_node, const_cast<conduit_##dname*>(data.data()), data.size());                       \
  }                                                                                                \
  void set_external(const std::vector<conduit_##dname>& data)                                      \
  {                                                                                                \
    this->set_external_##dname##_vector(data);                                                     \
  }

  catalyst_conduit_external_setter_vector(int8);
  catalyst_conduit_external_setter_vector(int16);
  catalyst_conduit_external_setter_vector(int32);
  catalyst_conduit_external_setter_vector(int64);

  catalyst_conduit_external_setter_vector(uint8);
  catalyst_conduit_external_setter_vector(uint16);
  catalyst_conduit_external_setter_vector(uint32);
  catalyst_conduit_external_setter_vector(uint64);

  catalyst_conduit_external_setter_vector(float32);
  catalyst_conduit_external_setter_vector(float64);

#undef catalyst_conduit_external_setter_vector

#define catalyst_conduit_c_external_setter_vector(cname, joined)                                   \
  void set_external(const std::vector<cname>& data)                                                \
  {                                                                                                \
    conduit_node_set_external_##joined##_ptr(                                                      \
      this->c_node, const_cast<cname*>(data.data()), data.size());                                 \
  }

  catalyst_conduit_c_external_setter_vector(char, char);
#ifndef CONDUIT_USE_CHAR
  catalyst_conduit_c_external_setter_vector(signed char, signed_char);
  catalyst_conduit_c_external_setter_vector(unsigned char, unsigned_char);
#endif
#ifndef CONDUIT_USE_SHORT
  catalyst_conduit_c_external_setter_vector(signed short, signed_short);
  catalyst_conduit_c_external_setter_vector(unsigned short, unsigned_short);
#endif
#ifndef CONDUIT_USE_INT
  catalyst_conduit_c_external_setter_vector(signed int, signed_int);
  catalyst_conduit_c_external_setter_vector(unsigned int, unsigned_int);
#endif
#ifndef CONDUIT_USE_LONG
  catalyst_conduit_c_external_setter_vector(signed long, signed_long);
  catalyst_conduit_c_external_setter_vector(unsigned long, unsigned_long);
#endif
#ifndef CONDUIT_USE_FLOAT
  catalyst_conduit_c_external_setter_vector(float, float);
#endif
#ifndef CONDUIT_USE_DOUBLE
  catalyst_conduit_c_external_setter_vector(double, double);
#endif

#undef catalyst_conduit_c_external_setter_vector

#define catalyst_conduit_external_setter_ptr(dname)                                                \
  void set_external_##dname##_ptr(const conduit_##dname* data, conduit_index_t num_elements = 1,   \
    conduit_index_t offset = 0, conduit_index_t stride = sizeof(conduit_##dname),                  \
    conduit_index_t element_bytes = sizeof(conduit_##dname), conduit_index_t endianness = 0)       \
  {                                                                                                \
    conduit_node_set_external_##dname##_ptr_detailed(this->c_node,                                 \
      const_cast<conduit_##dname*>(data), num_elements, offset, stride, element_bytes,             \
      endianness);                                                                                 \
  }                                                                                                \
  void set_external(const conduit_##dname* data, conduit_index_t num_elements = 1,                 \
    conduit_index_t offset = 0, conduit_index_t stride = sizeof(conduit_##dname),                  \
    conduit_index_t element_bytes = sizeof(conduit_##dname), conduit_index_t endianness = 0)       \
  {                                                                                                \
    this->set_external_##dname##_ptr(                                                              \
      data, num_elements, offset, stride, element_bytes, endianness);                              \
  }

  catalyst_conduit_external_setter_ptr(int8);
  catalyst_conduit_external_setter_ptr(int16);
  catalyst_conduit_external_setter_ptr(int32);
  catalyst_conduit_external_setter_ptr(int64);

  catalyst_conduit_external_setter_ptr(uint8);
  catalyst_conduit_external_setter_ptr(uint16);
  catalyst_conduit_external_setter_ptr(uint32);
  catalyst_conduit_external_setter_ptr(uint64);

  catalyst_conduit_external_setter_ptr(float32);
  catalyst_conduit_external_setter_ptr(float64);

#undef catalyst_conduit_external_setter_ptr

#define catalyst_conduit_c_external_setter_ptr(cname, joined)                                      \
  void set_external(cname* data, conduit_index_t num_elements = 1, conduit_index_t offset = 0,     \
    conduit_index_t stride = sizeof(cname), conduit_index_t element_bytes = sizeof(cname),         \
    conduit_index_t endianness = 0)                                                                \
  {                                                                                                \
    conduit_node_set_external_##joined##_ptr_detailed(                                             \
      this->c_node, data, num_elements, offset, stride, element_bytes, endianness);                \
  }

  void set_external_char_ptr(char* data, conduit_index_t num_elements = 1,
    conduit_index_t offset = 0, conduit_index_t stride = sizeof(char),
    conduit_index_t element_bytes = sizeof(char), conduit_index_t endianness = 0)
  {
    conduit_node_set_external_char_ptr_detailed(
      this->c_node, data, num_elements, offset, stride, element_bytes, endianness);
  }
#ifndef CONDUIT_USE_CHAR
  catalyst_conduit_c_external_setter_ptr(signed char, signed_char);
  catalyst_conduit_c_external_setter_ptr(unsigned char, unsigned_char);
#endif
#ifndef CONDUIT_USE_SHORT
  catalyst_conduit_c_external_setter_ptr(signed short, signed_short);
  catalyst_conduit_c_external_setter_ptr(unsigned short, unsigned_short);
#endif
#ifndef CONDUIT_USE_INT
  catalyst_conduit_c_external_setter_ptr(signed int, signed_int);
  catalyst_conduit_c_external_setter_ptr(unsigned int, unsigned_int);
#endif
#ifndef CONDUIT_USE_LONG
  catalyst_conduit_c_external_setter_ptr(signed long, signed_long);
  catalyst_conduit_c_external_setter_ptr(unsigned long, unsigned_long);
#endif
#ifndef CONDUIT_USE_FLOAT
  catalyst_conduit_c_external_setter_ptr(float, float);
#endif
#ifndef CONDUIT_USE_DOUBLE
  catalyst_conduit_c_external_setter_ptr(double, double);
#endif

#undef catalyst_conduit_c_external_setter_vector

  void set_path_external_node(const std::string& path, Node& data)
  {
    conduit_node_set_path_external_node(this->c_node, path.c_str(), data.c_node);
  }

  void set_path_external(const std::string& path, Node& data)
  {
    this->set_path_external_node(path, data);
  }

  void set_path_external_char8_str(const std::string& path, char* data)
  {
    conduit_node_set_path_external_char8_str(this->c_node, path.c_str(), data);
  }

#define catalyst_conduit_path_external_setter_vector(dname)                                        \
  void set_path_external_##dname##_vector(                                                         \
    const std::string& path, std::vector<conduit_##dname>& data)                                   \
  {                                                                                                \
    conduit_node_set_path_external_##dname##_ptr(                                                  \
      this->c_node, path.c_str(), data.data(), data.size());                                       \
  }                                                                                                \
  void set_path_external(const std::string& path, std::vector<conduit_##dname>& data)              \
  {                                                                                                \
    this->set_path_external_##dname##_vector(path, data);                                          \
  }

  catalyst_conduit_path_external_setter_vector(int8);
  catalyst_conduit_path_external_setter_vector(int16);
  catalyst_conduit_path_external_setter_vector(int32);
  catalyst_conduit_path_external_setter_vector(int64);

  catalyst_conduit_path_external_setter_vector(uint8);
  catalyst_conduit_path_external_setter_vector(uint16);
  catalyst_conduit_path_external_setter_vector(uint32);
  catalyst_conduit_path_external_setter_vector(uint64);

  catalyst_conduit_path_external_setter_vector(float32);
  catalyst_conduit_path_external_setter_vector(float64);

#undef catalyst_conduit_path_external_setter_vector

#define catalyst_conduit_c_path_external_setter_vector(cname, joined)                              \
  void set_path_external(const std::string& path, std::vector<cname>& data)                        \
  {                                                                                                \
    conduit_node_set_path_external_##joined##_ptr(                                                 \
      this->c_node, path.c_str(), data.data(), data.size());                                       \
  }

  catalyst_conduit_c_path_external_setter_vector(char, char);
#ifndef CONDUIT_USE_CHAR
  catalyst_conduit_c_path_external_setter_vector(signed char, signed_char);
  catalyst_conduit_c_path_external_setter_vector(unsigned char, unsigned_char);
#endif
#ifndef CONDUIT_USE_SHORT
  catalyst_conduit_c_path_external_setter_vector(signed short, signed_short);
  catalyst_conduit_c_path_external_setter_vector(unsigned short, unsigned_short);
#endif
#ifndef CONDUIT_USE_INT
  catalyst_conduit_c_path_external_setter_vector(signed int, signed_int);
  catalyst_conduit_c_path_external_setter_vector(unsigned int, unsigned_int);
#endif
#ifndef CONDUIT_USE_LONG
  catalyst_conduit_c_path_external_setter_vector(signed long, signed_long);
  catalyst_conduit_c_path_external_setter_vector(unsigned long, unsigned_long);
#endif
#ifndef CONDUIT_USE_FLOAT
  catalyst_conduit_c_path_external_setter_vector(float, float);
#endif
#ifndef CONDUIT_USE_DOUBLE
  catalyst_conduit_c_path_external_setter_vector(double, double);
#endif

#undef catalyst_conduit_c_path_external_setter_vector

#define catalyst_conduit_path_external_setter_ptr(dname)                                           \
  void set_path_external_##dname##_ptr(const std::string& path, conduit_##dname* data,             \
    conduit_index_t num_elements = 1, conduit_index_t offset = 0,                                  \
    conduit_index_t stride = sizeof(conduit_##dname),                                              \
    conduit_index_t element_bytes = sizeof(conduit_##dname), conduit_index_t endianness = 0)       \
  {                                                                                                \
    conduit_node_set_path_external_##dname##_ptr_detailed(                                         \
      this->c_node, path.c_str(), data, num_elements, offset, stride, element_bytes, endianness);  \
  }                                                                                                \
  void set_path_external(const std::string& path, conduit_##dname* data,                           \
    conduit_index_t num_elements = 1, conduit_index_t offset = 0,                                  \
    conduit_index_t stride = sizeof(conduit_##dname),                                              \
    conduit_index_t element_bytes = sizeof(conduit_##dname), conduit_index_t endianness = 0)       \
  {                                                                                                \
    this->set_path_external_##dname##_ptr(                                                         \
      path, data, num_elements, offset, stride, element_bytes, endianness);                        \
  }

  catalyst_conduit_path_external_setter_ptr(int8);
  catalyst_conduit_path_external_setter_ptr(int16);
  catalyst_conduit_path_external_setter_ptr(int32);
  catalyst_conduit_path_external_setter_ptr(int64);

  catalyst_conduit_path_external_setter_ptr(uint8);
  catalyst_conduit_path_external_setter_ptr(uint16);
  catalyst_conduit_path_external_setter_ptr(uint32);
  catalyst_conduit_path_external_setter_ptr(uint64);

  catalyst_conduit_path_external_setter_ptr(float32);
  catalyst_conduit_path_external_setter_ptr(float64);

#undef catalyst_conduit_path_external_setter_ptr

#define catalyst_conduit_c_path_external_setter_ptr(cname, joined)                                 \
  void set_path_external(const std::string& path, cname* data, conduit_index_t num_elements = 1,   \
    conduit_index_t offset = 0, conduit_index_t stride = sizeof(cname),                            \
    conduit_index_t element_bytes = sizeof(cname), conduit_index_t endianness = 0)                 \
  {                                                                                                \
    conduit_node_set_path_external_##joined##_ptr_detailed(                                        \
      this->c_node, path.c_str(), data, num_elements, offset, stride, element_bytes, endianness);  \
  }

  void set_path_external_char_ptr(const std::string& path, char* data,
    conduit_index_t num_elements = 1, conduit_index_t offset = 0,
    conduit_index_t stride = sizeof(CONDUIT_NATIVE_CHAR),
    conduit_index_t element_bytes = sizeof(CONDUIT_NATIVE_CHAR), conduit_index_t endianness = 0)
  {
    conduit_node_set_path_external_char_ptr_detailed(
      this->c_node, path.c_str(), data, num_elements, offset, stride, element_bytes, endianness);
  }
#ifndef CONDUIT_USE_CHAR
  catalyst_conduit_c_path_external_setter_ptr(signed char, signed_char);
  catalyst_conduit_c_path_external_setter_ptr(unsigned char, unsigned_char);
#endif
#ifndef CONDUIT_USE_SHORT
  catalyst_conduit_c_path_external_setter_ptr(signed short, signed_short);
  catalyst_conduit_c_path_external_setter_ptr(unsigned short, unsigned_short);
#endif
#ifndef CONDUIT_USE_INT
  catalyst_conduit_c_path_external_setter_ptr(signed int, signed_int);
  catalyst_conduit_c_path_external_setter_ptr(unsigned int, unsigned_int);
#endif
#ifndef CONDUIT_USE_LONG
  catalyst_conduit_c_path_external_setter_ptr(signed long, signed_long);
  catalyst_conduit_c_path_external_setter_ptr(unsigned long, unsigned_long);
#endif
#ifndef CONDUIT_USE_FLOAT
  catalyst_conduit_c_path_external_setter_ptr(float, float);
#endif
#ifndef CONDUIT_USE_DOUBLE
  catalyst_conduit_c_path_external_setter_ptr(double, double);
#endif

#undef catalyst_conduit_c_path_external_setter_ptr

  //-----------------------------------------------------------------------------
  // -- copy and move assignment operator for Node --
  //-----------------------------------------------------------------------------
  Node& operator=(const Node& other)
  {
    set(other);
    return *this;
  }

  Node& operator=(Node&& other)
  {
    this->c_node = other.c_node;
    this->own_c_node = other.own_c_node;
    other.c_node = nullptr;
    other.own_c_node = false;
    return *this;
  }
  //-----------------------------------------------------------------------------
  // --  assignment operators for scalar types ---
  //-----------------------------------------------------------------------------

#define catalyst_conduit_assign(dtype)                                                             \
  Node& operator=(dtype data)                                                                      \
  {                                                                                                \
    set(data);                                                                                     \
    return *this;                                                                                  \
  }

  //-----------------------------------------------------------------------------
  // signed integer scalar types
  //-----------------------------------------------------------------------------

  catalyst_conduit_assign(conduit_int8);
  catalyst_conduit_assign(conduit_int16);
  catalyst_conduit_assign(conduit_int32);
  catalyst_conduit_assign(conduit_int64);

  //-----------------------------------------------------------------------------
  // unsigned integer scalar types
  //-----------------------------------------------------------------------------

  catalyst_conduit_assign(conduit_uint8);
  catalyst_conduit_assign(conduit_uint16);
  catalyst_conduit_assign(conduit_uint32);
  catalyst_conduit_assign(conduit_uint64);

  //-----------------------------------------------------------------------------
  // floating point scalar types
  //-----------------------------------------------------------------------------

  catalyst_conduit_assign(conduit_float32);
  catalyst_conduit_assign(conduit_float64);

  //-----------------------------------------------------------------------------
  // assignment operator gap methods for scalar c-native types
  //-----------------------------------------------------------------------------

  catalyst_conduit_assign(char);

#ifndef CONDUIT_USE_CHAR
  catalyst_conduit_assign(signed char);
  catalyst_conduit_assign(unsigned char);
#endif

#ifndef CONDUIT_USE_SHORT
  catalyst_conduit_assign(signed short);
  catalyst_conduit_assign(unsigned short);
#endif

#ifndef CONDUIT_USE_INT
  catalyst_conduit_assign(signed int);
  catalyst_conduit_assign(unsigned int);
#endif

#ifndef CONDUIT_USE_LONG
  catalyst_conduit_assign(signed long);
  catalyst_conduit_assign(unsigned long);
#endif

#ifndef CONDUIT_USE_FLOAT
  catalyst_conduit_assign(float);
#endif

#ifndef CONDUIT_USE_DOUBLE
  catalyst_conduit_assign(double);
#endif

  //-----------------------------------------------------------------------------
  // -- assignment operators for string types --
  //-----------------------------------------------------------------------------

  catalyst_conduit_assign(const std::string&);
  catalyst_conduit_assign(const char*);

  //-----------------------------------------------------------------------------
  // -- assignment operators for std::vector types ---
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // signed integer array types via std::vector
  //-----------------------------------------------------------------------------

  catalyst_conduit_assign(const std::vector<conduit_int8>&);
  catalyst_conduit_assign(const std::vector<conduit_int16>&);
  catalyst_conduit_assign(const std::vector<conduit_int32>&);
  catalyst_conduit_assign(const std::vector<conduit_int64>&);

  //-----------------------------------------------------------------------------
  // unsigned integer array types via std::vector
  //-----------------------------------------------------------------------------

  catalyst_conduit_assign(const std::vector<conduit_uint8>&);
  catalyst_conduit_assign(const std::vector<conduit_uint16>&);
  catalyst_conduit_assign(const std::vector<conduit_uint32>&);
  catalyst_conduit_assign(const std::vector<conduit_uint64>&);

  //-----------------------------------------------------------------------------
  // floating point array types via std::vector
  //-----------------------------------------------------------------------------

  catalyst_conduit_assign(const std::vector<conduit_float32>&);
  catalyst_conduit_assign(const std::vector<conduit_float64>&);

  //-----------------------------------------------------------------------------
  // assignment operator gap methods for vector c-native types
  //-----------------------------------------------------------------------------

  catalyst_conduit_assign(const std::vector<char>&);

#ifndef CONDUIT_USE_CHAR
  catalyst_conduit_assign(const std::vector<signed char>&);
  catalyst_conduit_assign(const std::vector<unsigned char>&);
#endif

#ifndef CONDUIT_USE_SHORT
  catalyst_conduit_assign(const std::vector<signed short>&);
  catalyst_conduit_assign(const std::vector<unsigned short>&);
#endif

#ifndef CONDUIT_USE_INT
  catalyst_conduit_assign(const std::vector<signed int>&);
  catalyst_conduit_assign(const std::vector<unsigned int>&);
#endif

#ifndef CONDUIT_USE_LONG
  catalyst_conduit_assign(const std::vector<signed long>&);
  catalyst_conduit_assign(const std::vector<unsigned long>&);
#endif

#ifndef CONDUIT_USE_FLOAT
  catalyst_conduit_assign(const std::vector<float>&);
#endif

#ifndef CONDUIT_USE_DOUBLE
  catalyst_conduit_assign(const std::vector<double>&);
#endif

#undef catalyst_conduit_assign

  //-----------------------------------------------------------------------------
  //
  // -- end definition of Node assignment operators --
  //
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // -- leaf coercion methods ---
  //-----------------------------------------------------------------------------
  ///
  /// These methods allow you to coerce a leaf type to another type.
  ///

  /// scalar coercion

  /// convert to a signed integer types
  conduit_int8 to_int8() const { return this->to_type<conduit_int8, conduit_int16>(); }

  conduit_int16 to_int16() const { return this->to_type<conduit_int16>(); }

  conduit_int32 to_int32() const { return this->to_type<conduit_int32>(); }

  conduit_int64 to_int64() const { return this->to_type<conduit_int64>(); }

  /// convert to a unsigned integer types
  conduit_uint8 to_uint8() const { return this->to_type<conduit_uint8, conduit_uint16>(); }

  conduit_uint16 to_uint16() const { return this->to_type<conduit_uint16>(); }

  conduit_uint32 to_uint32() const { return this->to_type<conduit_uint32>(); }

  conduit_uint64 to_uint64() const { return this->to_type<conduit_uint64>(); }

  /// convert to a floating point type
  conduit_float32 to_float32() const { return this->to_type<conduit_float32>(); }

  conduit_float64 to_float64() const { return this->to_type<conduit_float64>(); }

  conduit_index_t to_index_t() const { return this->to_type<conduit_index_t>(); }

  /// convert to c integer types
  char to_char() const { return this->to_type<char, conduit_int16>(); }

  short to_short() const { return this->to_type<short>(); }

  int to_int() const { return this->to_type<int>(); }

  long to_long() const { return this->to_type<long>(); }

  /// convert to c signed integer types
  signed char to_signed_char() const { return this->to_type<signed char, conduit_int16>(); }

  signed short to_signed_short() const { return this->to_type<signed short>(); }

  signed int to_signed_int() const { return this->to_type<signed int>(); }

  signed long to_signed_long() const { return this->to_type<signed long>(); }

  /// convert to c unsigned integer types
  unsigned char to_unsigned_char() const { return this->to_type<unsigned char, conduit_uint16>(); }
  unsigned short to_unsigned_short() const { return this->to_type<unsigned short>(); }
  unsigned int to_unsigned_int() const { return this->to_type<unsigned int>(); }
  unsigned long to_unsigned_long() const { return this->to_type<unsigned long>(); }

  /// convert to c floating point types
  float to_float() const { return this->to_type<float>(); }

  double to_double() const { return this->to_type<double>(); }

  //-----------------------------------------------------------------------------
  //
  // -- begin declaration of Node information methods --
  //
  //-----------------------------------------------------------------------------
  //-----------------------------------------------------------------------------
  ///@name Node Information
  ///@{
  //-----------------------------------------------------------------------------
  /// description:
  ///  These methods provide general info about the node.
  //-----------------------------------------------------------------------------

  DataType dtype() const { return DataType(this->c_dtype()); }
  const conduit_datatype* c_dtype() const { return conduit_node_dtype(this->c_node); }

  //-------------------------------------------------------------------------
  /// contiguous checks
  //-------------------------------------------------------------------------

  /// Does this node has a contiguous data layout?
  bool is_contiguous() const { return conduit_node_is_contiguous(this->c_node); }

  /// true if node hierarchy's memory contiguously follows
  /// the given node's memory
  bool contiguous_with(const Node& n) const
  {
    return conduit_node_contiguous_with_node(this->c_node, n.c_node);
  }

  /// true if node hierarchy's memory contiguously follows
  /// the given address. Note: contiguous with NULL is false.
  bool contiguous_with(void* address) const
  {
    return conduit_node_contiguous_with_address(this->c_node, address);
  }

  /// is this node compatible with given node
  bool compatible(const Node& n) const { return conduit_node_compatible(this->c_node, n.c_node); }

  /// check for differences between this node and the given node, storing
  //  the results digest in the provided data node
  bool diff(const Node& n, Node& info, const conduit_float64 epsilon) const
  {
    CATALYST_CONDUIT_WRAP_EXCEP(
      return conduit_node_diff(this->c_node, n.c_node, info.c_node, epsilon));
  }

  /// diff this node to the given node for compatibility (i.e. validate it
  //  has everything that the instance node has), storing the results
  //  digest in the provided data node
  bool diff_compatible(const Node& n, Node& info, const conduit_float64 epsilon) const
  {
    CATALYST_CONDUIT_WRAP_EXCEP(
      return conduit_node_diff_compatible(this->c_node, n.c_node, info.c_node, epsilon));
  }

  ///
  /// info() creates a node that contains metadata about the current
  /// node's memory properties
  void info(Node& nres) const { conduit_node_info(this->c_node, nres.c_node); }

  Node info() const
  {
    Node ret;
    this->info(ret);
    return ret;
  }

  //-----------------------------------------------------------------------------
  // -- stdout print methods ---
  //-----------------------------------------------------------------------------
  /// print a simplified json representation of the this node to std out
  void print() const { conduit_node_print(this->c_node); }

  /// print a detailed json representation of the this node to std out.
  /// json output includes conduit schema constructs
  void print_detailed() const { conduit_node_print_detailed(this->c_node); }

  //-----------------------------------------------------------------------------
  ///@}
  //-----------------------------------------------------------------------------
  //
  // -- end declaration of Node information methods --
  //
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  //
  // -- begin declaration of Node entry access methods --
  //
  //-----------------------------------------------------------------------------
  //-----------------------------------------------------------------------------
  ///@name Node Entry Access Methods
  ///@{
  //-----------------------------------------------------------------------------
  /// description:
  ///  child access (for list or object types)
  //-----------------------------------------------------------------------------

  /// fetch the node at the given index
  Node child(conduit_index_t idx)
  {
    CATALYST_CONDUIT_WRAP_EXCEP(return Node(conduit_node_child(this->c_node, idx)));
  }

  const Node child(conduit_index_t idx) const
  {
    CATALYST_CONDUIT_WRAP_EXCEP(return Node(conduit_node_child(this->c_node, idx)));
  }

  /// access child node via a path (equivalent to fetch via path)
  Node operator[](const std::string& path)
  {
    CATALYST_CONDUIT_WRAP_EXCEP(return Node(conduit_node_fetch(this->c_node, path.c_str())));
  }

  // Don't modify underlying structure if path doesn't exist
  const Node operator[](const std::string& path) const
  {
    if (this->has_path(path))
    {
      CATALYST_CONDUIT_WRAP_EXCEP(return Node(conduit_node_fetch(this->c_node, path.c_str())));
    }
    CATALYST_CONDUIT_ERROR("Error fetching path " + path);
  }

  /// access child node via index (equivalent to fetch via index)
  Node operator[](conduit_index_t idx) { return child(idx); }
  const Node operator[](conduit_index_t idx) const { return child(idx); }

  /// returns the number of children (list and object interfaces)
  conduit_index_t number_of_children() const
  {
    return conduit_node_number_of_children(this->c_node);
  }

  // conduit_node_name uses strdup, which allocates memory
  // on the heap. Must free the result here after copying.
  std::string name() const
  {
    char* name = conduit_node_name(this->c_node);
    if (!name)
    {
      CATALYST_CONDUIT_ERROR("Error retrieving name from C API.")
    }
    std::string ret(name);
    free(name);

    return ret;
  }

  // conduit_node_path uses strdup, which allocates memory
  // on the heap. Must free the result here after copying.
  std::string path() const
  {
    char* path = conduit_node_path(this->c_node);
    if (!path)
    {
      CATALYST_CONDUIT_ERROR("Error retrieving path from C API.")
    }
    std::string ret(path);
    free(path);

    // Return copy
    return ret;
  }

  /// checks if a node has a direct child with given name
  bool has_child(const std::string& name) const
  {
    return conduit_node_has_child(this->c_node, name.c_str());
  }
  /// checks if given path exists in the Node hierarchy
  bool has_path(const std::string& path) const
  {
    return conduit_node_has_path(this->c_node, path.c_str());
  }

  Node append() { return Node(conduit_node_append(this->c_node)); }

  /// remove child at index (list and object interfaces)
  void remove(conduit_index_t idx)
  {
    CATALYST_CONDUIT_WRAP_EXCEP(conduit_node_remove_child(this->c_node, idx));
  }

  /// remove child at given path (object interface)
  void remove(const std::string& path)
  {
    CATALYST_CONDUIT_WRAP_EXCEP(conduit_node_remove_path(this->c_node, path.c_str()));
  }

  /// rename a child (object interface)
  void rename_child(const std::string& current_name, const std::string& new_name)
  {
    CATALYST_CONDUIT_WRAP_EXCEP(
      conduit_node_rename_child(this->c_node, current_name.c_str(), new_name.c_str()));
  }

  //-----------------------------------------------------------------------------
  ///@}
  //-----------------------------------------------------------------------------
  //
  // -- end declaration of Node entry access methods --
  //
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  //
  // -- begin declaration of Node value access methods --
  //
  //-----------------------------------------------------------------------------
  //-----------------------------------------------------------------------------
  ///@name Node Value Access Methods
  ///@{
  //-----------------------------------------------------------------------------
  /// description:
  ///  Direct access to data at leaf types.
  //-----------------------------------------------------------------------------

#define catalyst_conduit_as(dname)                                                                 \
  conduit_##dname as_##dname() const                                                               \
  {                                                                                                \
    CATALYST_CONDUIT_WRAP_EXCEP(return conduit_node_as_##dname(this->c_node));                     \
  }

  // signed integer scalars
  catalyst_conduit_as(int8);
  catalyst_conduit_as(int16);
  catalyst_conduit_as(int32);
  catalyst_conduit_as(int64);

  // unsigned integer scalars
  catalyst_conduit_as(uint8);
  catalyst_conduit_as(uint16);
  catalyst_conduit_as(uint32);
  catalyst_conduit_as(uint64);

  // floating point scalars
  catalyst_conduit_as(float32);
  catalyst_conduit_as(float64);

#undef catalyst_conduit_as

#define catalyst_conduit_as_ptr(dname)                                                             \
  conduit_##dname* as_##dname##_ptr()                                                              \
  {                                                                                                \
    CATALYST_CONDUIT_WRAP_EXCEP(return conduit_node_as_##dname##_ptr(this->c_node));               \
  }                                                                                                \
  const conduit_##dname* as_##dname##_ptr() const                                                  \
  {                                                                                                \
    CATALYST_CONDUIT_WRAP_EXCEP(return conduit_node_as_##dname##_ptr(this->c_node));               \
  }

  // signed integers via pointers
  catalyst_conduit_as_ptr(int8);
  catalyst_conduit_as_ptr(int16);
  catalyst_conduit_as_ptr(int32);
  catalyst_conduit_as_ptr(int64);

  // unsigned integers via pointers
  catalyst_conduit_as_ptr(uint8);
  catalyst_conduit_as_ptr(uint16);
  catalyst_conduit_as_ptr(uint32);
  catalyst_conduit_as_ptr(uint64);

  // floating point via pointers
  catalyst_conduit_as_ptr(float32);
  catalyst_conduit_as_ptr(float64);

#undef catalyst_conduit_as_ptr

  // char8_str cases
  char* as_char8_str()
  {
    CATALYST_CONDUIT_WRAP_EXCEP(return conduit_node_as_char8_str(this->c_node));
  }
  const char* as_char8_str() const
  {
    CATALYST_CONDUIT_WRAP_EXCEP(return conduit_node_as_char8_str(this->c_node));
  }
  std::string as_string() const
  {
    CATALYST_CONDUIT_WRAP_EXCEP(return std::string(conduit_node_as_char8_str(this->c_node)));
  }

  // direct data pointer access
  void* data_ptr() { return conduit_node_data_ptr(this->c_node); }
  const void* data_ptr() const { return conduit_node_data_ptr(this->c_node); }

  void* element_ptr(conduit_index_t idx) { return conduit_node_element_ptr(this->c_node, idx); }
  const void* element_ptr(conduit_index_t idx) const
  {
    return conduit_node_element_ptr(this->c_node, idx);
  }

  //-----------------------------------------------------------------------------
  /// description:
  ///  Direct access to data at leaf types (native c++ types)
  //-----------------------------------------------------------------------------`

#define catalyst_conduit_c_as(cname, joined)                                                       \
  cname as_##joined() const                                                                        \
  {                                                                                                \
    CATALYST_CONDUIT_WRAP_EXCEP(return conduit_node_as_##joined(this->c_node));                    \
  }

  // c style scalar
  catalyst_conduit_c_as(char, char);
  catalyst_conduit_c_as(short, short);
  catalyst_conduit_c_as(int, int);
  catalyst_conduit_c_as(long, long);

  // signed integer scalars
  catalyst_conduit_c_as(signed char, signed_char);
  catalyst_conduit_c_as(signed short, signed_short);
  catalyst_conduit_c_as(signed int, signed_int);
  catalyst_conduit_c_as(signed long, signed_long);

  // unsigned integer scalars
  catalyst_conduit_c_as(unsigned char, unsigned_char);
  catalyst_conduit_c_as(unsigned short, unsigned_short);
  catalyst_conduit_c_as(unsigned int, unsigned_int);
  catalyst_conduit_c_as(unsigned long, unsigned_long);

  // floating point scalars
  catalyst_conduit_c_as(float, float);
  catalyst_conduit_c_as(double, double);

#undef catalyst_conduit_c_as

#define catalyst_conduit_c_as_ptr(cname, joined)                                                   \
  cname* as_##joined##_ptr()                                                                       \
  {                                                                                                \
    CATALYST_CONDUIT_WRAP_EXCEP(return conduit_node_as_##joined##_ptr(this->c_node));              \
  }                                                                                                \
  const cname* as_##joined##_ptr() const                                                           \
  {                                                                                                \
    CATALYST_CONDUIT_WRAP_EXCEP(return conduit_node_as_##joined##_ptr(this->c_node));              \
  }

  // c style via pointer
  catalyst_conduit_c_as_ptr(char, char);
  catalyst_conduit_c_as_ptr(short, short);
  catalyst_conduit_c_as_ptr(int, int);
  catalyst_conduit_c_as_ptr(long, long);

  // signed integers via pointers
  catalyst_conduit_c_as_ptr(signed char, signed_char);
  catalyst_conduit_c_as_ptr(signed short, signed_short);
  catalyst_conduit_c_as_ptr(signed int, signed_int);
  catalyst_conduit_c_as_ptr(signed long, signed_long);

  // unsigned integers via pointers
  catalyst_conduit_c_as_ptr(unsigned char, unsigned_char);
  catalyst_conduit_c_as_ptr(unsigned short, unsigned_short);
  catalyst_conduit_c_as_ptr(unsigned int, unsigned_int);
  catalyst_conduit_c_as_ptr(unsigned long, unsigned_long);

  // floating point via pointers
  catalyst_conduit_c_as_ptr(float, float);
  catalyst_conduit_c_as_ptr(double, double);

#undef catalyst_conduit_c_as_ptr

private:
  conduit_node* c_node;
  bool own_c_node = true;

  explicit Node(conduit_node* other)
    : c_node(other)
    , own_c_node(false)
  {
  }

  template <typename T, typename Deser = T>
  T to_type() const
  {
    DataType const dtype = this->dtype();
    /* ints */
    if (dtype.is_int8())
    {
      return static_cast<T>(as_int8());
    }
    if (dtype.is_int16())
    {
      return static_cast<T>(as_int16());
    }
    if (dtype.is_int32())
    {
      return static_cast<T>(as_int32());
    }
    if (dtype.is_int64())
    {
      return static_cast<T>(as_int64());
    }
    /* conduit_uints */
    if (dtype.is_uint8())
    {
      return static_cast<T>(as_uint8());
    }
    if (dtype.is_uint16())
    {
      return static_cast<T>(as_uint16());
    }
    if (dtype.is_uint32())
    {
      return static_cast<T>(as_uint32());
    }
    if (dtype.is_uint64())
    {
      return static_cast<T>(as_uint64());
    }
    /* floats */
    if (dtype.is_float32())
    {
      return static_cast<T>(as_float32());
    }
    if (dtype.is_float64())
    {
      return static_cast<T>(as_float64());
    }
    // string case
    if (dtype.is_char8_str())
    {
      Deser res;
      std::stringstream ss(as_char8_str());
      if (ss >> res)
        return static_cast<T>(res);
    }
    return 0;
  }
};

inline conduit_node* c_node(const Node* n)
{
  return n ? n->c_node : nullptr;
}

inline Node cpp_node(conduit_node* n)
{
  return Node(n);
}

}
#endif
