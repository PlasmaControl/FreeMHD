/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMaxAccumulator.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "vtkMaxAccumulator.h"

#include "vtkFunctor.h"
#include "vtkObjectFactory.h"

#include <algorithm>
#include <cassert>
#include <limits>

vtkStandardNewMacro(vtkMaxAccumulator);

//----------------------------------------------------------------------------
vtkMaxAccumulator::vtkMaxAccumulator()
  : Value(-std::numeric_limits<double>::infinity())
{
}

//----------------------------------------------------------------------------
void vtkMaxAccumulator::Add(vtkAbstractAccumulator* accumulator)
{
  vtkMaxAccumulator* maxAccumulator = vtkMaxAccumulator::SafeDownCast(accumulator);
  assert(maxAccumulator && "Cannot accumulate different accumulators");
  this->Value = std::max(maxAccumulator->GetValue(), this->Value);
  this->Modified();
}

//----------------------------------------------------------------------------
void vtkMaxAccumulator::Add(double value, double vtkNotUsed(weight))
{
  this->Value = std::max(value, this->Value);
  this->Modified();
}

//----------------------------------------------------------------------------
void vtkMaxAccumulator::Initialize()
{
  this->Value = -std::numeric_limits<double>::infinity();
  this->Modified();
}

//----------------------------------------------------------------------------
double vtkMaxAccumulator::GetValue() const
{
  return this->Value;
}

//----------------------------------------------------------------------------
bool vtkMaxAccumulator::HasSameParameters(vtkAbstractAccumulator* accumulator) const
{
  vtkMaxAccumulator* acc = vtkMaxAccumulator::SafeDownCast(accumulator);
  return acc != nullptr;
}

//----------------------------------------------------------------------------
void vtkMaxAccumulator::ShallowCopy(vtkObject* accumulator)
{
  this->Superclass::ShallowCopy(accumulator);
  vtkMaxAccumulator* maxAccumulator = vtkMaxAccumulator::SafeDownCast(accumulator);
  if (maxAccumulator)
  {
    this->Value = maxAccumulator->GetValue();
  }
  else
  {
    vtkWarningMacro(<< "Could not ShallowCopy " << maxAccumulator->GetClassName() << " to "
                    << this->GetClassName());
  }
}

//----------------------------------------------------------------------------
void vtkMaxAccumulator::DeepCopy(vtkObject* accumulator)
{
  this->Superclass::DeepCopy(accumulator);
  vtkMaxAccumulator* maxAccumulator = vtkMaxAccumulator::SafeDownCast(accumulator);
  if (maxAccumulator)
  {
    this->Value = maxAccumulator->GetValue();
  }
  else
  {
    vtkWarningMacro(<< "Could not DeepCopy " << maxAccumulator->GetClassName() << " to "
                    << this->GetClassName());
  }
}

//----------------------------------------------------------------------------
void vtkMaxAccumulator::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
