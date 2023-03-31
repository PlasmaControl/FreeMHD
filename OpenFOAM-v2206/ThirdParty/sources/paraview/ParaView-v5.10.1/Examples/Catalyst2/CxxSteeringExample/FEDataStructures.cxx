/*=========================================================================

  Program:   ParaView
  Module:    vtkDataObjectToConduit.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "FEDataStructures.h"

#include <iostream>
#include <iterator>
#include <mpi.h>

Grid::Grid() = default;

void Grid::Initialize(const unsigned int numPoints[3], const double spacing[3])
{
  if (numPoints[0] == 0 || numPoints[1] == 0 || numPoints[2] == 0)
  {
    std::cerr << "Must have a non-zero amount of points in each dimension.\n";
  }
  // in parallel, we do a simple partitioning in the x-direction.
  int mpiSize = 1;
  int mpiRank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpiRank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpiSize);

  unsigned int startXPoint = mpiRank * numPoints[0] / mpiSize;
  unsigned int endXPoint = (mpiRank + 1) * numPoints[0] / mpiSize;
  if (mpiSize != mpiRank + 1)
  {
    endXPoint++;
  }

  // create the points -- slowest in the x and fastest in the z directions
  double coord[3] = { 0, 0, 0 };
  for (unsigned int x = startXPoint; x < endXPoint; x++)
  {
    coord[0] = x * spacing[0];
    for (unsigned int y = 0; y < numPoints[1]; y++)
    {
      coord[1] = y * spacing[1];
      for (unsigned int z = 0; z < numPoints[2]; z++)
      {
        coord[2] = z * spacing[2];
        // add the coordinate to the end of the vector
        std::copy(coord, coord + 3, std::back_inserter(this->Points));
      }
    }
  }
  // create the hex cells
  unsigned int numXPoints = endXPoint - startXPoint;
  for (unsigned int i = 0; i < numXPoints - 1; i++)
  {
    for (unsigned int j = 0; j < numPoints[1] - 1; j++)
    {
      for (unsigned int k = 0; k < numPoints[2] - 1; k++)
      {
        unsigned int cellPoints[8] = { i * numPoints[1] * numPoints[2] + j * numPoints[2] + k,
          (i + 1) * numPoints[1] * numPoints[2] + j * numPoints[2] + k,
          (i + 1) * numPoints[1] * numPoints[2] + (j + 1) * numPoints[2] + k,
          i * numPoints[1] * numPoints[2] + (j + 1) * numPoints[2] + k,
          i * numPoints[1] * numPoints[2] + j * numPoints[2] + k + 1,
          (i + 1) * numPoints[1] * numPoints[2] + j * numPoints[2] + k + 1,
          (i + 1) * numPoints[1] * numPoints[2] + (j + 1) * numPoints[2] + k + 1,
          i * numPoints[1] * numPoints[2] + (j + 1) * numPoints[2] + k + 1 };
        std::copy(cellPoints, cellPoints + 8, std::back_inserter(this->Cells));
      }
    }
  }
}

size_t Grid::GetNumberOfPoints()
{
  return this->Points.size() / 3;
}

size_t Grid::GetNumberOfCells()
{
  return this->Cells.size() / 8;
}

double* Grid::GetPointsArray()
{
  if (this->Points.empty())
  {
    return nullptr;
  }
  return this->Points.data();
}

double* Grid::GetPoint(size_t pointId)
{
  if (pointId >= this->GetNumberOfPoints())
  {
    return nullptr;
  }
  return this->Points.data() + pointId * 3;
}

unsigned int* Grid::GetCellPoints(size_t cellId)
{
  if (cellId >= this->GetNumberOfCells())
  {
    return nullptr;
  }
  return this->Cells.data() + cellId * 8;
}

Attributes::Attributes()
{
  this->GridPtr = nullptr;
}

void Attributes::Initialize(Grid* grid)
{
  this->GridPtr = grid;
}

void Attributes::UpdateFields(double time)
{
  size_t numPoints = this->GridPtr->GetNumberOfPoints();
  this->Velocity.resize(numPoints * 3);
  for (size_t pt = 0; pt < numPoints; pt++)
  {
    double* coord = this->GridPtr->GetPoint(pt);
    this->Velocity[pt] = coord[1] * time;
  }
  std::fill(this->Velocity.begin() + numPoints, this->Velocity.end(), 0.);
  size_t numCells = this->GridPtr->GetNumberOfCells();
  this->Pressure.resize(numCells);
  std::fill(this->Pressure.begin(), this->Pressure.end(), 1.f);
}

double* Attributes::GetVelocityArray()
{
  if (this->Velocity.empty())
  {
    return nullptr;
  }
  return this->Velocity.data();
}

float* Attributes::GetPressureArray()
{
  if (this->Pressure.empty())
  {
    return nullptr;
  }
  return this->Pressure.data();
}
