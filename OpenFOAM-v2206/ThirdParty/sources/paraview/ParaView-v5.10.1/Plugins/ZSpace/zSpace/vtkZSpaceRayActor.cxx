/*=========================================================================

Program:   Visualization Toolkit
Module:    vtkZSpaceRayActor.cxx

Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkZSpaceRayActor.h"

#include "vtkLineSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkTransform.h"

vtkStandardNewMacro(vtkZSpaceRayActor);

//------------------------------------------------------------------------------
vtkZSpaceRayActor::vtkZSpaceRayActor()
{
  // Line source oriented in +Z
  this->LineSource->SetPoint1(0.0, 0.0, 0.0);
  this->LineSource->SetPoint2(0.0, 0.0, -1.0);
  this->Mapper->SetInputConnection(this->LineSource->GetOutputPort());
  this->SetMapper(this->Mapper);

  this->GetProperty()->SetLineWidth(this->LineWidth);

  // The ray won't influence the scene bounds (and ResetCamera)
  this->UseBoundsOff();
  // Do not pick itself
  this->PickableOff();

  // Start by setting the length and color without pick
  this->SetNoPick();
}

//------------------------------------------------------------------------------
vtkZSpaceRayActor::~vtkZSpaceRayActor() = default;

//------------------------------------------------------------------------------
void vtkZSpaceRayActor::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  os << indent << "LineWidth: " << this->LineWidth << endl;
  os << indent << "NoPickColor = (" << this->NoPickColor[0] << ", " << this->NoPickColor[1] << ", "
     << this->NoPickColor[2] << endl;
  os << indent << "PickColor = (" << this->PickColor[0] << ", " << this->PickColor[1] << ", "
     << this->PickColor[2] << endl;
}

//------------------------------------------------------------------------------
void vtkZSpaceRayActor::SetNoPick()
{
  this->GetProperty()->SetColor(this->NoPickColor);
}

//------------------------------------------------------------------------------
void vtkZSpaceRayActor::SetPick()
{
  this->GetProperty()->SetColor(this->PickColor);
}
