// Copyright (c) 2016  GeometryFactory (France).
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
// $URL: https://github.com/CGAL/cgal/blob/releases/CGAL-4.14.3/Surface_mesh_parameterization/include/CGAL/Surface_mesh_parameterization/internal/kernel_traits.h $
// $Id: kernel_traits.h 8cdfad0 2017-11-15T22:58:57+01:00 Sébastien Loriot
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s)     : Mael Rouxel-Labbé

#ifndef CGAL_SURFACE_MESH_PARAMETERIZATION_INTERNAL_KERNEL_TRAITS_H
#define CGAL_SURFACE_MESH_PARAMETERIZATION_INTERNAL_KERNEL_TRAITS_H

#include <CGAL/license/Surface_mesh_parameterization.h>

#include <CGAL/boost/graph/properties.h>
#include <CGAL/Kernel_traits.h>

namespace CGAL {

namespace Surface_mesh_parameterization {

namespace internal {

template <class TM>
class Kernel_traits
{
public:
  typedef typename boost::property_map<TM, CGAL::vertex_point_t>::const_type  PPM;
  typedef typename boost::property_traits<PPM>::value_type                    Point_3;
  typedef typename CGAL::Kernel_traits<Point_3>::Kernel                       Kernel;
};

} // namespace internal

} // namespace Surface_mesh_parameterization

} // namespace CGAL

#endif // CGAL_SURFACE_MESH_PARAMETERIZATION_INTERNAL_KERNEL_TRAITS_H
