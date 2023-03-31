/*=========================================================================

  Program:   ParaView
  Module:    pqDiscreteDoubleWidget.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "pqDiscreteDoubleWidget.h"

// Qt includes
#include <QDoubleValidator>
#include <QHBoxLayout>
#include <QSlider>

pqDiscreteDoubleWidget::pqDiscreteDoubleWidget(QWidget* parent)
  : Superclass(parent)
{
}

//-----------------------------------------------------------------------------
pqDiscreteDoubleWidget::~pqDiscreteDoubleWidget() = default;

//-----------------------------------------------------------------------------
std::vector<double> pqDiscreteDoubleWidget::values() const
{
  return std::vector<double>(this->Values.begin(), this->Values.end());
}

//-----------------------------------------------------------------------------
void pqDiscreteDoubleWidget::setValues(std::vector<double> values)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
  this->Values = QVector<double>::fromStdVector(values);
#else
  this->Values = QVector<double>(values.begin(), values.end());
#endif
  this->setSliderRange(0, this->Values.size() - 1);
  auto minMax = std::minmax_element(values.begin(), values.end());
  this->setValidator(new QDoubleValidator(*minMax.first, *minMax.second, 100));
}

//-----------------------------------------------------------------------------
int pqDiscreteDoubleWidget::valueToSliderPos(double val)
{
  return this->Values.indexOf(val);
}

//-----------------------------------------------------------------------------
double pqDiscreteDoubleWidget::sliderPosToValue(int pos)
{
  return this->Values.at(pos);
}
