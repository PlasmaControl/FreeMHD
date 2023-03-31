// Copyright (c) 2000  Max-Planck-Institute Saarbruecken (Germany).
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
// $URL: https://github.com/CGAL/cgal/blob/releases/CGAL-4.14.3/Partition_2/include/CGAL/Partition_2/Partition_opt_cvx_diagonal_list.h $
// $Id: Partition_opt_cvx_diagonal_list.h ee57fc2 2017-10-21T01:03:14+02:00 Sébastien Loriot
// SPDX-License-Identifier: GPL-3.0+
// 
//
// Author(s)     : Susan Hert <hert@mpi-sb.mpg.de>

#ifndef   CGAL_PARTITION_OPT_CVX_DIAGONAL_LIST_H
#define   CGAL_PARTITION_OPT_CVX_DIAGONAL_LIST_H

#include <CGAL/license/Partition_2.h>


#include <utility>
#include <list>
#include <iostream>

typedef std::pair<int, int>                   Partition_opt_cvx_diagonal;
typedef std::list<Partition_opt_cvx_diagonal> Partition_opt_cvx_diagonal_list;

inline
std::ostream& operator<<(std::ostream& os,
                         const Partition_opt_cvx_diagonal_list& d)
{
   Partition_opt_cvx_diagonal_list::const_iterator it;
   for (it = d.begin(); it != d.end(); it++)
   {
      os << "(" << (*it).first << ", " << (*it).second << ") ";
   }
   return os;
}

#endif // CGAL_PARTITION_OPT_CVX_DIAGONAL_LIST_H
