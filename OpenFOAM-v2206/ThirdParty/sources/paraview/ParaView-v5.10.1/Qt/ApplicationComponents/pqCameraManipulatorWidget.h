/*=========================================================================

   Program: ParaView
   Module:  pqCameraManipulatorWidget.h

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
#ifndef pqCameraManipulatorWidget_h
#define pqCameraManipulatorWidget_h

#include "pqApplicationComponentsModule.h"
#include "pqPropertyWidget.h"

/**
 * pqCameraManipulatorWidget used on "Camera2DManipulators" and
 * "Camera3DManipulators" property on a RenderView proxy.
 */
class PQAPPLICATIONCOMPONENTS_EXPORT pqCameraManipulatorWidget : public pqPropertyWidget
{
  Q_OBJECT;
  typedef pqPropertyWidget Superclass;
  Q_PROPERTY(QList<QVariant> manipulatorTypes READ manipulatorTypes WRITE setManipulatorTypes);

public:
  pqCameraManipulatorWidget(vtkSMProxy* proxy, vtkSMProperty* smproperty, QWidget* parent = 0);
  ~pqCameraManipulatorWidget() override;

  /**
   * returns a list for the selected manipulator types. This has exactly 9
   * items always.
   */
  QList<QVariant> manipulatorTypes() const;

public Q_SLOTS:
  /**
   * Set the manipulator types selection. This must have exactly 9 items or
   * less. Any missing items are treated as "None".
   */
  void setManipulatorTypes(const QList<QVariant>& value);

Q_SIGNALS:
  void manipulatorTypesChanged();

private:
  Q_DISABLE_COPY(pqCameraManipulatorWidget)
  class pqInternals;
  pqInternals* Internals;
  class PropertyLinksConnection;
};

#endif
