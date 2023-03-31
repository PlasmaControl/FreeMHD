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
/**
 * @class   pqExtrusionPropertyWidgetDecorator
 * @brief   decorator used to hide/show range inputs for extrusion representation
 *
 * When auto-scaling or normalization is enabled, range inputs must be hidden.
 */

#ifndef pqExtrusionPropertyWidgetDecorator_h
#define pqExtrusionPropertyWidgetDecorator_h

#include "pqPropertyWidgetDecorator.h"
#include "vtkWeakPointer.h"

class vtkSMProperty;

class pqExtrusionPropertyWidgetDecorator : public pqPropertyWidgetDecorator
{
  Q_OBJECT
  typedef pqPropertyWidgetDecorator Superclass;

public:
  pqExtrusionPropertyWidgetDecorator(vtkPVXMLElement* config, pqPropertyWidget* parentObject);
  virtual ~pqExtrusionPropertyWidgetDecorator();

  virtual bool canShowWidget(bool show_advanced) const;

private:
  Q_DISABLE_COPY(pqExtrusionPropertyWidgetDecorator)

  vtkWeakPointer<vtkSMProperty> ObservedObject1, ObservedObject2;
  unsigned long ObserverId1, ObserverId2;
};

#endif
