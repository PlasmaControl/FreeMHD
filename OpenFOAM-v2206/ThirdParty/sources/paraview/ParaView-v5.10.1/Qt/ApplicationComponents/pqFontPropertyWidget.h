/*=========================================================================

   Program: ParaView
   Module:    $RCSfile$

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

========================================================================*/
#ifndef pqFontPropertyWidget_h
#define pqFontPropertyWidget_h

#include "pqApplicationComponentsModule.h"
#include "pqPropertyGroupWidget.h"

class QActionGroup;
class vtkSMPropertyGroup;

/**
 * pqFontPropertyWidget is a pqPropertyWidget that can be used to set
 * properties relating to fonts. The widget expects the property-group to have
 * properties with functions set to "Color", "Opacity", "Family", "Size",
 * "Bold", "Italics" and "Shadow". If any property is missing, the
 * corresponding widget will be hidden.
 */
class PQAPPLICATIONCOMPONENTS_EXPORT pqFontPropertyWidget : public pqPropertyGroupWidget
{
  Q_OBJECT
  Q_PROPERTY(
    QString HorizontalJustification READ HorizontalJustification WRITE setHorizontalJustification)
  Q_PROPERTY(
    QString VerticalJustification READ VerticalJustification WRITE setVerticalJustification)

  typedef pqPropertyGroupWidget Superclass;

public:
  pqFontPropertyWidget(vtkSMProxy* proxy, vtkSMPropertyGroup* smgroup, QWidget* parent = 0);
  ~pqFontPropertyWidget() override;

  QString HorizontalJustification() const;
  QString VerticalJustification() const;
Q_SIGNALS:
  void horizontalJustificationChanged(QString&);
  void verticalJustificationChanged(QString&);

protected:
  void setHorizontalJustification(QString&);
  void setVerticalJustification(QString&);
  void setupHorizontalJustificationButton();
  void setupVerticalJustificationButton();
  void UpdateToolButtonIcon(QString& str, QToolButton* justification);
  QActionGroup* CreateFontActionGroup(QToolButton* justification);

protected Q_SLOTS:
  void changeHorizontalJustificationIcon(QAction*);
  void changeVerticalJustificationIcon(QAction*);
  void onFontFamilyChanged();

private:
  Q_DISABLE_COPY(pqFontPropertyWidget)

  class pqInternals;
  pqInternals* Internals;
};

#endif
