/*=========================================================================

  Program:   ParaView
  Module:    vtkCPFieldBuilder.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkCPFieldBuilder.h"

#include "vtkCPTensorFieldFunction.h"

vtkCxxSetObjectMacro(vtkCPFieldBuilder, TensorFieldFunction, vtkCPTensorFieldFunction);

//----------------------------------------------------------------------------
vtkCPFieldBuilder::vtkCPFieldBuilder()
{
  this->ArrayName = nullptr;
  this->TensorFieldFunction = nullptr;
}

//----------------------------------------------------------------------------
vtkCPFieldBuilder::~vtkCPFieldBuilder()
{
  this->SetArrayName(nullptr);
  this->SetTensorFieldFunction(nullptr);
}

//----------------------------------------------------------------------------
vtkCPTensorFieldFunction* vtkCPFieldBuilder::GetTensorFieldFunction()
{
  return this->TensorFieldFunction;
}

//----------------------------------------------------------------------------
void vtkCPFieldBuilder::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "ArrayName: " << this->ArrayName << endl;
  os << indent << "TensorFieldFunction: " << this->TensorFieldFunction << endl;
}
