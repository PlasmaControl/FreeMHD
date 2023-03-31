// Copyright (c) 2015 GeometryFactory (France).
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
// $URL: https://github.com/CGAL/cgal/blob/releases/CGAL-4.14.3/Polygon_mesh_processing/include/CGAL/Polygon_mesh_processing/internal/named_function_params.h $
// $Id: named_function_params.h 560c77f 2019-07-14T22:54:16+02:00 Sébastien Loriot
// SPDX-License-Identifier: GPL-3.0+
//
//
// Author(s)     : Jane Tournois

#ifndef CGAL_PMP_BGL_NAMED_FUNCTION_PARAMS_H
#define CGAL_PMP_BGL_NAMED_FUNCTION_PARAMS_H

#include <CGAL/license/Polygon_mesh_processing/core.h>


#include <CGAL/boost/graph/Named_function_parameters.h>

#define CGAL_PMP_NP_TEMPLATE_PARAMETERS CGAL_BGL_NP_TEMPLATE_PARAMETERS
#define CGAL_PMP_NP_CLASS CGAL_BGL_NP_CLASS

namespace CGAL { namespace Polygon_mesh_processing { namespace parameters = CGAL::parameters; } }

#endif //CGAL_PMP_BGL_NAMED_FUNCTION_PARAMS_H
