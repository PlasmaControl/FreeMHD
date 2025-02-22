/*=========================================================================

   Program: ParaView
   Module:  pqSelectionQueryPropertyWidget.h

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
#ifndef pqSelectionQueryPropertyWidget_h
#define pqSelectionQueryPropertyWidget_h

#include "pqApplicationComponentsModule.h"
#include "pqPropertyWidget.h"

#include <QScopedPointer> // for QScopedPointer.

class QStringList;

/**
 * @class pqSelectionQueryPropertyWidget
 * @brief property-widget for creating selection queries.
 *
 * pqSelectionQueryPropertyWidget is a pqPropertyWidget subclass that can be
 * used for a property accepting a string representing a selection query. The
 * widget allows users to build selection queries easily.
 *
 * To use this widget on a property with vtkSMSelectionQueryDomain, set the
 * `panel_widget` to `selection_query` (@sa pqStandardPropertyWidgetInterface).
 *
 * @section Caveats Caveats
 *
 * Currently, this widget can faithfully present GUI for query strings that
 * follow a specific form that is generated by this property itself. Any other
 * forms, though valid queries, will not be represented properly in the GUI.
 */
class PQAPPLICATIONCOMPONENTS_EXPORT pqSelectionQueryPropertyWidget : public pqPropertyWidget
{
  Q_OBJECT
  typedef pqPropertyWidget Superclass;

  Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged);

public:
  pqSelectionQueryPropertyWidget(vtkSMProxy* proxy, vtkSMProperty* property, QWidget* parent = 0);
  ~pqSelectionQueryPropertyWidget() override;

  //@{
  /**
   * Get/Set the query string.
   */
  void setQuery(const QString&);
  const QString& query() const;
  //@}

Q_SIGNALS:
  void queryChanged();

private:
  Q_DISABLE_COPY(pqSelectionQueryPropertyWidget);

  class pqInternals;
  QScopedPointer<pqInternals> Internals;

  class pqValueWidget;
  class pqQueryWidget;

  friend class pqInternals;
  friend class pqValueWidget;
  friend class pqQueryWidget;
};

#endif
