/*=========================================================================

  Program:   ParaView
  Module:    vtkCleanUnstructuredGridCells.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// Programmed 2010 by Dominik Szczerba <dominik@itis.ethz.ch>
//
/**
 * @class   vtkCleanUnstructuredGridCells
 * @brief   remove duplicate/degenerate cells
 *
 *
 * Merges degenerate cells. Assumes the input grid does not contain duplicate
 * points. You may want to run vtkCleanUnstructuredGrid first to assert it. If
 * duplicated cells are found they are removed in the output. The filter also
 * handles the case, where a cell may contain degenerate nodes (i.e. one and
 * the same node is referenced by a cell more than once).
 *
 * @sa
 * vtkCleanPolyData
 */

#ifndef vtkCleanUnstructuredGridCells_h
#define vtkCleanUnstructuredGridCells_h

#include "vtkPVVTKExtensionsFiltersGeneralModule.h" //needed for exports
#include "vtkUnstructuredGridAlgorithm.h"

class VTKPVVTKEXTENSIONSFILTERSGENERAL_EXPORT vtkCleanUnstructuredGridCells
  : public vtkUnstructuredGridAlgorithm
{
public:
  static vtkCleanUnstructuredGridCells* New();

  vtkTypeMacro(vtkCleanUnstructuredGridCells, vtkUnstructuredGridAlgorithm);

  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  vtkCleanUnstructuredGridCells();
  ~vtkCleanUnstructuredGridCells() override;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  int FillInputPortInformation(int port, vtkInformation* info) override;

private:
  vtkCleanUnstructuredGridCells(const vtkCleanUnstructuredGridCells&) = delete;
  void operator=(const vtkCleanUnstructuredGridCells&) = delete;
};

#endif
