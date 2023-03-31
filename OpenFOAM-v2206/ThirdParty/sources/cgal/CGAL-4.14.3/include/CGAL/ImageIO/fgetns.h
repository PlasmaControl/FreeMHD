// Copyright (c) 2005-2008 ASCLEPIOS Project, INRIA Sophia-Antipolis (France)
// All rights reserved.
//
// This file is part of the ImageIO Library, and as been adapted for
// CGAL (www.cgal.org).
// You can redistribute it and/or  modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation;
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// These files are provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: https://github.com/CGAL/cgal/blob/releases/CGAL-4.14.3/CGAL_ImageIO/include/CGAL/ImageIO/fgetns.h $
// $Id: fgetns.h 0698f79 2017-10-20T23:34:14+02:00 Sébastien Loriot
// SPDX-License-Identifier: LGPL-3.0+
//
//
// Author(s)     :  ASCLEPIOS Project (INRIA Sophia-Antipolis), Laurent Rineau

#ifndef FGETNS_H
#define FGETNS_H

#include <CGAL/ImageIO.h>

/* get a string from a file and discard the ending newline character
   if any */
char *fgetns(char *str, int n,  _image *im );

#ifdef CGAL_HEADER_ONLY
#include <CGAL/ImageIO/fgetns_impl.h>
#endif // CGAL_HEADER_ONLY

#endif // FGETNS_H
