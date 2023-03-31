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
// $URL: https://github.com/CGAL/cgal/blob/releases/CGAL-4.14.3/Partition_2/include/CGAL/Partition_2/Turn_reverser.h $
// $Id: Turn_reverser.h ee57fc2 2017-10-21T01:03:14+02:00 Sébastien Loriot
// SPDX-License-Identifier: GPL-3.0+
// 
//
// Author(s)     : Susan Hert <hert@mpi-sb.mpg.de>

#ifndef   CGAL_TURN_REVERSER_H
#define   CGAL_TURN_REVERSER_H

#include <CGAL/license/Partition_2.h>


namespace CGAL {

template <class Point_2, class TurnPredicate>
class Turn_reverser 
{
public:
   Turn_reverser() {}
   Turn_reverser( const TurnPredicate& t ): turn(t) {}

   bool operator() (const Point_2& p1, 
                    const Point_2& p2, 
                    const Point_2& p3) const
   {   return turn(p2, p1, p3); }

private:
   TurnPredicate turn;
};


}

#endif // CGAL_TURN_REVERSER_H
