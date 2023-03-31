/*=========================================================================

  Program:   ParaView
  Module:    vtkCleanArrays.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkCleanArrays
 * @brief   filter used to remove partial arrays across processes.
 *
 * vtkCleanArrays is a filter used to remove (or fill up) partial arrays in a
 * vtkDataSet (or a vtkCompositeDataSet) across processes (and blocks).
 * Empty dataset on any processes is skipped and doesn't affect the array pruned
 * (or filled) in the output. This filter also handles certain non-composite
 * data objects such a tables.
 *
 */

#ifndef vtkCleanArrays_h
#define vtkCleanArrays_h

#include "vtkPVVTKExtensionsMiscModule.h" // needed for export macro
#include "vtkPassInputTypeAlgorithm.h"

class vtkMultiProcessController;
class VTKPVVTKEXTENSIONSMISC_EXPORT vtkCleanArrays : public vtkPassInputTypeAlgorithm
{
public:
  static vtkCleanArrays* New();
  vtkTypeMacro(vtkCleanArrays, vtkPassInputTypeAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * The user can set the controller used for inter-process communication. By
   * default set to the global communicator.
   */
  void SetController(vtkMultiProcessController* controller);
  vtkGetObjectMacro(Controller, vtkMultiProcessController);
  //@}

  //@{
  /**
   * When set to true (false by default), 0 filled array will be added for
   * missing arrays on this process (instead of removing partial arrays).
   */
  vtkSetMacro(FillPartialArrays, bool);
  vtkGetMacro(FillPartialArrays, bool);
  vtkBooleanMacro(FillPartialArrays, bool);
  //@}

  //@{
  /**
   * When FillPartialArrays is true, this filter can add a `vtkUnsignedCharArray`
   * for each partial array to indicate which entries are valid and which are
   * simply filled in with 0. To do that, set this flag to true.
   * Default is false.
   */
  vtkSetMacro(MarkFilledPartialArrays, bool);
  vtkGetMacro(MarkFilledPartialArrays, bool);
  vtkBooleanMacro(MarkFilledPartialArrays, bool);
  //@}

protected:
  vtkCleanArrays();
  ~vtkCleanArrays() override;

  int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) override;

  vtkMultiProcessController* Controller;
  bool FillPartialArrays;
  bool MarkFilledPartialArrays;

private:
  vtkCleanArrays(const vtkCleanArrays&) = delete;
  void operator=(const vtkCleanArrays&) = delete;

public:
  class vtkArrayData;
  class vtkArraySet;
};

#endif
