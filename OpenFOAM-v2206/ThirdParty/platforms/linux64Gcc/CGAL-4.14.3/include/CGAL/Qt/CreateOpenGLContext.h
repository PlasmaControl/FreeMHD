// Copyright (c) 2015  GeometryFactory SARL (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: https://github.com/CGAL/cgal/blob/releases/CGAL-4.14.3/GraphicsView/include/CGAL/Qt/CreateOpenGLContext.h $
// $Id: CreateOpenGLContext.h 18e38c2 2018-04-04T16:53:33+02:00 Maxime Gimeno
// SPDX-License-Identifier: LGPL-3.0
// 
//
// Author(s)     : Laurent Rineau and Maxime Gimeno
#ifndef CGAL_QT_CREATE_OPENGL_CONTEXT_H
#define CGAL_QT_CREATE_OPENGL_CONTEXT_H

#include <QOpenGLContext>
#include <QGLContext>
namespace CGAL{
namespace Qt{
inline QGLContext* createOpenGLContext()
{
    QOpenGLContext *context = new QOpenGLContext();
    QSurfaceFormat format;
    format.setVersion(2,1);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    context->setFormat(format);
    QGLContext *result = QGLContext::fromOpenGLContext(context);
    result->create();
    return result;
}
} // namespace Qt
} // namespace CGAL
#endif
