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
// $URL: https://github.com/CGAL/cgal/blob/releases/CGAL-4.14.3/Envelope_3/include/CGAL/Envelope_3/Envelope_base.h $
// $Id: Envelope_base.h ee57fc2 2017-10-21T01:03:14+02:00 Sébastien Loriot
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s)     : Michal Meyerovitch     <gorgymic@post.tau.ac.il>
//                 Baruch Zukerman        <baruchzu@post.tau.ac.il>
//                 Efi Fogel              <efif@post.tau.ac.il>

#ifndef CGAL_ENVELOPE_BASE_H
#define CGAL_ENVELOPE_BASE_H

#include <CGAL/license/Envelope_3.h>


namespace CGAL {

// Envelope types:
enum Envelope_type {
  ENVELOPE_LOWER = 1,
  ENVELOPE_UPPER = 2
};

// Types of intersection curve between 2 xy-monotone surfaces:
enum Intersection_type {
  INTERSECTION_UNKNOWN = 0,
  INTERSECTION_TRANSVERSAL = 1,
  INTERSECTION_TANGENT = 2
};

// Decision mark for DCEL features:
enum Dac_decision
{
  DAC_DECISION_FIRST = -1,
  DAC_DECISION_BOTH,
  DAC_DECISION_SECOND,
  DAC_DECISION_NOT_SET
};

} //namespace CGAL

#endif // CGAL_ENVELOPE_BASE_3_H
