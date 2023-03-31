#ifndef FEDATASTRUCTURES_HEADER
#define FEDATASTRUCTURES_HEADER

typedef struct Grid
{
  unsigned int NumberOfPoints;
  double* Points;
  unsigned int NumberOfCells;
  unsigned int* Cells;
} Grid;

void InitializeGrid(Grid* grid, const unsigned int numPoints[3], const double spacing[3]);
void FinalizeGrid(Grid*);

typedef struct Attributes
{
  // A structure for generating and storing point and cell fields.
  // Velocity is stored at the points and pressure is stored
  // for the cells. The current velocity profile is for a
  // shearing flow with U(y,t) = y*t, V = 0 and W = 0.
  // Pressure is constant through the domain.
  double* Velocity;
  float* Pressure;
  Grid* GridPtr;
} Attributes;

void InitializeAttributes(Attributes* attributes, Grid* grid);
void FinalizeAttributes(Attributes* attributes);
void UpdateFields(Attributes* attributes, double time);
#endif
