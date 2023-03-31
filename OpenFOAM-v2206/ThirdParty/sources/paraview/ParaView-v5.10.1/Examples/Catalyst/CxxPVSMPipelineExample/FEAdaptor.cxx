#include "FEAdaptor.h"
#include "FEDataStructures.h"
#include "vtkCPPVSMPipeline.h"
#include <iostream>

#include <vtkCPDataDescription.h>
#include <vtkCPInputDataDescription.h>
#include <vtkCPProcessor.h>
#include <vtkCellData.h>
#include <vtkCellType.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkNew.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkSMSession.h>
#include <vtkUnstructuredGrid.h>

namespace
{
vtkCPProcessor* Processor = nullptr;
vtkUnstructuredGrid* VTKGrid;

void BuildVTKGrid(Grid& grid)
{
  // create the points information
  vtkNew<vtkDoubleArray> pointArray;
  pointArray->SetNumberOfComponents(3);
  pointArray->SetArray(
    grid.GetPointsArray(), static_cast<vtkIdType>(grid.GetNumberOfPoints() * 3), 1);
  vtkNew<vtkPoints> points;
  points->SetData(pointArray);
  VTKGrid->SetPoints(points);

  // create the cells
  size_t numCells = grid.GetNumberOfCells();
  VTKGrid->Allocate(static_cast<vtkIdType>(numCells * 9));
  for (size_t cell = 0; cell < numCells; cell++)
  {
    unsigned int* cellPoints = grid.GetCellPoints(cell);
    vtkIdType tmp[8] = { cellPoints[0], cellPoints[1], cellPoints[2], cellPoints[3], cellPoints[4],
      cellPoints[5], cellPoints[6], cellPoints[7] };
    VTKGrid->InsertNextCell(VTK_HEXAHEDRON, 8, tmp);
  }
}

void UpdateVTKAttributes(Grid& grid, Attributes& attributes)
{
  if (VTKGrid->GetPointData()->GetNumberOfArrays() == 0)
  {
    // velocity array
    vtkNew<vtkDoubleArray> velocity;
    velocity->SetName("velocity");
    velocity->SetNumberOfComponents(3);
    velocity->SetNumberOfTuples(static_cast<vtkIdType>(grid.GetNumberOfPoints()));
    VTKGrid->GetPointData()->AddArray(velocity);
  }
  if (VTKGrid->GetCellData()->GetNumberOfArrays() == 0)
  {
    // pressure array
    vtkNew<vtkFloatArray> pressure;
    pressure->SetName("pressure");
    pressure->SetNumberOfComponents(1);
    VTKGrid->GetCellData()->AddArray(pressure);
  }
  vtkDoubleArray* velocity =
    vtkDoubleArray::SafeDownCast(VTKGrid->GetPointData()->GetArray("velocity"));
  // The velocity array is ordered as vx0,vx1,vx2,..,vy0,vy1,vy2,..,vz0,vz1,vz2,..
  // so we need to create a full copy of it with VTK's ordering of
  // vx0,vy0,vz0,vx1,vy1,vz1,..
  double* velocityData = attributes.GetVelocityArray();
  vtkIdType numTuples = velocity->GetNumberOfTuples();
  for (vtkIdType i = 0; i < numTuples; i++)
  {
    double values[3] = { velocityData[i], velocityData[i + numTuples],
      velocityData[i + 2 * numTuples] };
    velocity->SetTypedTuple(i, values);
  }

  vtkFloatArray* pressure =
    vtkFloatArray::SafeDownCast(VTKGrid->GetCellData()->GetArray("pressure"));
  // The pressure array is a scalar array so we can reuse
  // memory as long as we ordered the points properly.
  float* pressureData = attributes.GetPressureArray();
  pressure->SetArray(pressureData, static_cast<vtkIdType>(grid.GetNumberOfCells()), 1);
}

void BuildVTKDataStructures(Grid& grid, Attributes& attributes)
{
  if (VTKGrid == nullptr)
  {
    // The grid structure isn't changing so we only build it
    // the first time it's needed. If we needed the memory
    // we could delete it and rebuild as necessary.
    VTKGrid = vtkUnstructuredGrid::New();
    BuildVTKGrid(grid);
  }
  UpdateVTKAttributes(grid, attributes);
}
}

namespace FEAdaptor
{
void Initialize(int outputFrequency, std::string fileName)
{
  if (Processor == nullptr)
  {
    Processor = vtkCPProcessor::New();
    Processor->Initialize();
  }
  vtkSMSession::ConnectToSelf();
  vtkNew<vtkCPPVSMPipeline> pipeline;
  pipeline->Initialize(outputFrequency, fileName);
  Processor->AddPipeline(pipeline);
}

void Finalize()
{
  if (Processor)
  {
    Processor->Delete();
    Processor = nullptr;
  }
  if (VTKGrid)
  {
    VTKGrid->Delete();
    VTKGrid = nullptr;
  }
}

void CoProcess(
  Grid& grid, Attributes& attributes, double time, unsigned int timeStep, bool lastTimeStep)
{
  vtkNew<vtkCPDataDescription> dataDescription;
  dataDescription->AddInput("input");
  dataDescription->SetTimeData(time, timeStep);
  if (lastTimeStep == true)
  {
    // assume that we want to all the pipelines to execute if it
    // is the last time step.
    dataDescription->ForceOutputOn();
  }
  if (Processor->RequestDataDescription(dataDescription) != 0)
  {
    BuildVTKDataStructures(grid, attributes);
    dataDescription->GetInputDescriptionByName("input")->SetGrid(VTKGrid);
    Processor->CoProcess(dataDescription);
  }
}
} // end of Catalyst namespace
