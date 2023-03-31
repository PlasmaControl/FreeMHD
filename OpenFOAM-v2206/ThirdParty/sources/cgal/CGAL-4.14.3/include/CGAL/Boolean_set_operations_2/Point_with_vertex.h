// Copyright (c) 2005  Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: https://github.com/CGAL/cgal/blob/releases/CGAL-4.14.3/Boolean_set_operations_2/include/CGAL/Boolean_set_operations_2/Point_with_vertex.h $
// $Id: Point_with_vertex.h ee57fc2 2017-10-21T01:03:14+02:00 Sébastien Loriot
// SPDX-License-Identifier: GPL-3.0+
// 
//
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>

#ifndef CGAL_POINT_WITH_VERTEX_H
#define CGAL_POINT_WITH_VERTEX_H

#include <CGAL/license/Boolean_set_operations_2.h>


namespace CGAL {

template <class Arrangement_>
class Point_with_vertex
{
protected:
  typedef typename Arrangement_::Vertex_handle        Vertex_handle;
  typedef typename Arrangement_::Vertex_const_handle  Vertex_const_handle;
  
public:
  Vertex_handle  m_v;

  Point_with_vertex()
  {};

  Point_with_vertex(Vertex_handle v) : m_v(v)
  {}

  Vertex_handle vertex() const
  {
    return (m_v);
  }

  Vertex_handle vertex()
  {
    return (m_v);
  }

  void set_halfedge(Vertex_handle v)
  {
    m_v = v;
  }
};

} //namespace CGAL
#endif
