/*=========================================================================

  Program:   ParaView
  Module:    vtkPythonAnnotationFilter.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkPython.h" // has to be first!

#include "vtkPythonAnnotationFilter.h"

#include "vtkIdTypeArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMultiProcessController.h"
#include "vtkObjectFactory.h"
#include "vtkPVStringFormatter.h"
#include "vtkPythonInterpreter.h"
#include "vtkPythonUtil.h"
#include "vtkSmartPyObject.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkStringArray.h"
#include "vtkTable.h"

#include <cassert>
#include <sstream>
#include <vector>
#include <vtksys/SystemTools.hxx>

namespace
{
bool CheckAndFlushPythonErrors()
{
  if (PyErr_Occurred())
  {
    PyErr_Print();
    PyErr_Clear();
    return true;
  }
  return false;
}
}

vtkStandardNewMacro(vtkPythonAnnotationFilter);

//----------------------------------------------------------------------------
vtkPythonAnnotationFilter::vtkPythonAnnotationFilter()
  : Expression(nullptr)
  , ComputedAnnotationValue(nullptr)
  , ArrayAssociation(vtkDataObject::FIELD)
  , DataTimeValid(false)
  , DataTime(0.0)
  , NumberOfTimeSteps(0)
  , TimeSteps(nullptr)
  , TimeRangeValid(false)
  , CurrentInputDataObject(nullptr)
{
  this->SetNumberOfInputPorts(1);
  this->TimeRange[0] = this->TimeRange[1] = 0.0;
}

//----------------------------------------------------------------------------
vtkPythonAnnotationFilter::~vtkPythonAnnotationFilter()
{
  this->SetExpression(nullptr);
  this->SetComputedAnnotationValue(nullptr);
}

//----------------------------------------------------------------------------
void vtkPythonAnnotationFilter::SetComputedAnnotationValue(const char* value)
{
  delete[] this->ComputedAnnotationValue;
  // SystemTools handles nullptr strings.
  this->ComputedAnnotationValue = vtksys::SystemTools::DuplicateString(value);
  // don't call this->Modified. This method gets called in RequestData().
}

//----------------------------------------------------------------------------
int vtkPythonAnnotationFilter::RequestData(vtkInformation* vtkNotUsed(request),
  vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  vtkDataObject* input = vtkDataObject::GetData(inputVector[0], 0);
  assert(input != nullptr);

  // initialize variables.
  this->DataTimeValid = false;
  this->DataTime = 0.0;
  this->TimeSteps = nullptr;
  this->NumberOfTimeSteps = 0;
  this->TimeRangeValid = false;
  this->TimeRange[0] = this->TimeRange[1] = 0.0;
  this->SetComputedAnnotationValue(nullptr);
  this->CurrentInputDataObject = input;

  // Extract time information
  if (vtkInformation* dataInformation = input->GetInformation())
  {
    if (dataInformation->Has(vtkDataObject::DATA_TIME_STEP()))
    {
      this->DataTimeValid = true;
      this->DataTime = dataInformation->Get(vtkDataObject::DATA_TIME_STEP());
    }
  }

  std::vector<double> timeSteps;
  vtkInformation* inputInfo = inputVector[0]->GetInformationObject(0);
  if (inputInfo->Has(vtkStreamingDemandDrivenPipeline::TIME_STEPS()))
  {
    this->NumberOfTimeSteps = inputInfo->Length(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
    this->TimeSteps = inputInfo->Get(vtkStreamingDemandDrivenPipeline::TIME_STEPS());

    timeSteps.insert(timeSteps.begin(), this->TimeSteps, this->TimeSteps + this->NumberOfTimeSteps);
  }

  std::vector<double> timeRange;
  if (inputInfo->Has(vtkStreamingDemandDrivenPipeline::TIME_RANGE()))
  {
    this->TimeRangeValid = true;
    inputInfo->Get(vtkStreamingDemandDrivenPipeline::TIME_RANGE(), this->TimeRange);

    timeRange.insert(timeRange.begin(), this->TimeRange, this->TimeRange + 2);
  }

  int timeIndex = 0;
  if (this->GetDataTimeValid() && this->GetNumberOfTimeSteps() > 0)
  {
    for (int i = 0; i < this->NumberOfTimeSteps; ++i)
    {
      if (timeSteps[i] == this->DataTime)
      {
        timeIndex = i;
      }
    }
  }

  // define annotate scope
  PV_STRING_FORMATTER_NAMED_SCOPE("ANNOTATE", fmt::arg("timevalue", this->DataTime),
    fmt::arg("timesteps", timeSteps), fmt::arg("timerange", timeRange),
    fmt::arg("timeindex", timeIndex));

  char* cachedExpression = vtksys::SystemTools::DuplicateString(this->Expression);

  std::string formattableExpression = this->Expression ? this->Expression : std::string();
  delete[] this->Expression;
  this->Expression = vtksys::SystemTools::DuplicateString(
    vtkPVStringFormatter::Format(formattableExpression).c_str());

  this->EvaluateExpression();

  // restore cached expression
  delete[] this->Expression;
  this->Expression = vtksys::SystemTools::DuplicateString(cachedExpression);

  // Make sure a valid ComputedAnnotationValue is available
  if (this->ComputedAnnotationValue == nullptr)
  {
    this->SetComputedAnnotationValue("(error)");
  }

  // Update the output data
  vtkStringArray* data = vtkStringArray::New();
  data->SetName("Text");
  data->SetNumberOfComponents(1);
  data->InsertNextValue(this->ComputedAnnotationValue);

  vtkTable* output = vtkTable::GetData(outputVector);
  output->AddColumn(data);
  data->FastDelete();
  this->CurrentInputDataObject = nullptr;

  if (vtkMultiProcessController::GetGlobalController() &&
    vtkMultiProcessController::GetGlobalController()->GetLocalProcessId() > 0)
  {
    // reset output on all ranks except the 0 root node.
    output->Initialize();
  }
  return 1;
}

//----------------------------------------------------------------------------
int vtkPythonAnnotationFilter::FillInputPortInformation(int vtkNotUsed(port), vtkInformation* info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataObject");
  return 1;
}

//----------------------------------------------------------------------------
void vtkPythonAnnotationFilter::EvaluateExpression()
{
  // ensure Python is initialized (safe to call many times)
  vtkPythonInterpreter::Initialize();

  vtkPythonScopeGilEnsurer gilEnsurer;
  vtkSmartPyObject modAnnotation(PyImport_ImportModule("paraview.detail.annotation"));
  CheckAndFlushPythonErrors();
  if (!modAnnotation)
  {
    vtkErrorMacro("Failed to import `paraview.detail.annotation` module.");
    return;
  }

  vtkSmartPyObject self(vtkPythonUtil::GetObjectFromPointer(this));
  vtkSmartPyObject fname(PyString_FromString("execute"));

  // call `paraview.detail.annotation.execute(self)`
  vtkSmartPyObject retVal(
    PyObject_CallMethodObjArgs(modAnnotation, fname.GetPointer(), self.GetPointer(), nullptr));

  CheckAndFlushPythonErrors();
  // at some point we may want to check retval
  (void)retVal;
}

//----------------------------------------------------------------------------
void vtkPythonAnnotationFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
