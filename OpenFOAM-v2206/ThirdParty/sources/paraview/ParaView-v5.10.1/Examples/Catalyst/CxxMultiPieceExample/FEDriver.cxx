#include "FEDataStructures.h"
#include <algorithm>
#include <iostream>
#include <mpi.h>
#include <vector>

#ifdef USE_CATALYST
#include "FEAdaptor.h"
#endif

// Example of a C++ adaptor for a simulation code
// where the simulation code has a fixed topology
// grid. We treat the grid as a partitioned
// regular grid. This is stored as a set of
// vtkImageDatas in a vtkMultiPieceDataSet.
// Also, the points are stored in an inconsistent
// manner with respect to the velocity vector.
// This is purposefully done to demonstrate
// the different approaches for getting data
// into Catalyst. Note that through configuration
// that the driver can be run without linking
// to Catalyst.

int main(int argc, char* argv[])
{
  MPI_Init(&argc, &argv);
  Grid grid;
  unsigned int numPoints[3] = { 70, 60, 44 };
  double spacing[3] = { 1, 1.1, 1.3 };
  grid.Initialize(numPoints, spacing);
  Attributes attributes;
  attributes.Initialize(&grid);

#ifdef USE_CATALYST
  // The first argument is the program name
  FEAdaptor::Initialize(argc - 1, argv + 1);
#endif
  unsigned int numberOfTimeSteps = 100;
  for (unsigned int timeStep = 0; timeStep < numberOfTimeSteps; timeStep++)
  {
    // use a time step length of 0.1
    double time = timeStep * 0.1;
    attributes.UpdateFields(time);
#ifdef USE_CATALYST
    FEAdaptor::CoProcess(grid, attributes, time, timeStep, timeStep == numberOfTimeSteps - 1);
#endif
  }

#ifdef USE_CATALYST
  FEAdaptor::Finalize();
#endif
  MPI_Finalize();

  return 0;
}
