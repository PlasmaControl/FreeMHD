/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkVRPNCallBackHandlers.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkVRPNCallBackHandlers.h"

#include "pqActiveObjects.h"
#include "pqDataRepresentation.h"
#include "pqVRPNConnection.h"
#include "pqView.h"
#include "vtkMath.h"
#include "vtkSMDoubleVectorProperty.h"
#include "vtkSMPropertyHelper.h"
#include "vtkSMRenderViewProxy.h"
#include "vtkSMRepresentationProxy.h"

#include <iostream>
#include <vector>

#include <vrpn_Analog.h>
#include <vrpn_Button.h>
#include <vrpn_Dial.h>
#include <vrpn_Text.h>
#include <vrpn_Tracker.h>

// ----------------------------------------------------------------------------
void VRPN_CALLBACK handleAnalogChange(void* userdata, const vrpn_ANALOGCB analog_value)
{
  pqVRPNConnection* self = static_cast<pqVRPNConnection*>(userdata);
  self->newAnalogValue(analog_value);
}

// ----------------------------------------------------------------------------
void VRPN_CALLBACK handleButtonChange(
  void* userdata, vrpn_BUTTONCB button_value) // WRS-TODO: why no "const" here?
{
  pqVRPNConnection* self = static_cast<pqVRPNConnection*>(userdata);
  self->newButtonValue(button_value);
}

// ----------------------------------------------------------------------------
void VRPN_CALLBACK handleTrackerChange(void* userdata, const vrpn_TRACKERCB tracker_value)
{
  pqVRPNConnection* self = static_cast<pqVRPNConnection*>(userdata);
  self->newTrackerValue(tracker_value);
}
