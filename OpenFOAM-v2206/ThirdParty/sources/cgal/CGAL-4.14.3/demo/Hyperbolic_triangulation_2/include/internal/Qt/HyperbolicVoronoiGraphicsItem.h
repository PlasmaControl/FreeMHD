// Copyright (c) 2008  GeometryFactory Sarl (France).
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
// $URL: https://github.com/CGAL/cgal/blob/releases/CGAL-4.14.3/Hyperbolic_triangulation_2/demo/Hyperbolic_triangulation_2/include/internal/Qt/HyperbolicVoronoiGraphicsItem.h $
// $Id: HyperbolicVoronoiGraphicsItem.h 322ddd5 2018-12-19T20:31:49+01:00 Mael Rouxel-Labbé
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s)     : Andreas Fabri <Andreas.Fabri@geometryfactory.com>
//                 Laurent Rineau <Laurent.Rineau@geometryfactory.com>

#ifndef CGAL_QT_VORONOI_GRAPHICS_ITEM_H
#define CGAL_QT_VORONOI_GRAPHICS_ITEM_H



#include <CGAL/Qt/GraphicsItem.h>
#include <CGAL/Qt/PainterOstream.h>
#include <CGAL/Qt/utility.h>

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include <CGAL/intersection_2.h>

class QGraphicsSceneMouseEvent;


namespace CGAL {
namespace Qt {

template <typename DT>
class VoronoiGraphicsItem : public GraphicsItem
{
public:
  VoronoiGraphicsItem(DT  * dt_);


  QRectF 
  boundingRect() const;
  
  void 
  paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  
  void 
  modelChanged();

  const QPen& edgesPen() const
  {
    return edges_pen;
  }

  void setEdgesPen(const QPen& pen)
  {
    edges_pen = pen;
  }

private:
  DT * dt;
  QPen edges_pen;
};



template <typename DT>
VoronoiGraphicsItem<DT>::VoronoiGraphicsItem(DT * dt_)
  :  dt(dt_)
{
  setZValue(3);
}

template <typename DT>
QRectF 
VoronoiGraphicsItem<DT>::boundingRect() const
{
  QRectF rect = CGAL::Qt::viewportsBbox(scene());
  return rect;
}


template <typename DT>
void 
VoronoiGraphicsItem<DT>::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * /*w*/)
{
  QRectF rect = option->exposedRect;
  PainterOstream<typename DT::Geom_traits> pos(painter, rect);
  
  painter->setPen(edgesPen());
  
  // delete
  QPen temp = painter->pen();
  QPen old = temp;
  temp.setWidthF(0.01);
  painter->setPen(temp);
  //
  
  for(typename DT::All_edges_iterator eit = dt->all_edges_begin();
      eit != dt->all_edges_end();
      eit++) 
    {
      typename DT::Hyperbolic_segment s = dt->dual(*eit);
      pos << s;
    } 
 
  // delete
  painter->setPen(old);
}


template <typename T>
void 
VoronoiGraphicsItem<T>::modelChanged()
{
  update();
}

} // namespace Qt
} // namespace CGAL

#endif // CGAL_QT_VORONOI_GRAPHICS_ITEM_H
