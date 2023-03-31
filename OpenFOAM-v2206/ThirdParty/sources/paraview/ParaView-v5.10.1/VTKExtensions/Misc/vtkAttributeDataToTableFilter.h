/*=========================================================================

  Program:   ParaView
  Module:    vtkAttributeDataToTableFilter.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkAttributeDataToTableFilter
 * @brief   this filter produces a
 * vtkTable from the chosen attribute in the input data object.
 *
 * vtkAttributeDataToTableFilter is a filter that produces a vtkTable from the
 * chosen attribute in the input dataobject. This filter can accept composite
 * datasets. If the input is a composite dataset, the output is a multiblock
 * with vtkTable leaves.
 */

#ifndef vtkAttributeDataToTableFilter_h
#define vtkAttributeDataToTableFilter_h

#include "vtkPVVTKExtensionsMiscModule.h" // needed for export macro
#include "vtkTableAlgorithm.h"

class vtkDataSet;
class vtkFieldData;
class vtkIdTypeArray;

class VTKPVVTKEXTENSIONSMISC_EXPORT vtkAttributeDataToTableFilter : public vtkTableAlgorithm
{
public:
  static vtkAttributeDataToTableFilter* New();
  vtkTypeMacro(vtkAttributeDataToTableFilter, vtkTableAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Select the attribute type. Accepted values are
   * \li vtkDataObject::FIELD_ASSOCIATION_POINTS,
   * \li vtkDataObject::FIELD_ASSOCIATION_CELLS,
   * \li vtkDataObject::FIELD_ASSOCIATION_NONE,
   * \li vtkDataObject::FIELD_ASSOCIATION_VERTICES,
   * \li vtkDataObject::FIELD_ASSOCIATION_EDGES,
   * \li vtkDataObject::FIELD_ASSOCIATION_ROWS
   * If value is vtkDataObject::FIELD_ASSOCIATION_NONE, then FieldData
   * associated with the input dataobject is extracted.
   */
  vtkSetMacro(FieldAssociation, int);
  vtkGetMacro(FieldAssociation, int);
  //@}

  //@{
  /**
   * It is possible for this filter to add additional meta-data to the field
   * data such as point coordinates (when point attributes are selected and
   * input is pointset) or structured coordinates etc. To enable this addition
   * of extra information, turn this flag on. Off by default.
   */
  vtkSetMacro(AddMetaData, bool);
  vtkGetMacro(AddMetaData, bool);
  vtkBooleanMacro(AddMetaData, bool);
  //@}

  //@{
  /**
   * When set (default) the vtkOriginalIndices array will be added to the
   * output. Can be overridden by setting this flag to 0.
   * This is only respected when AddMetaData is true.
   */
  vtkSetMacro(GenerateOriginalIds, bool);
  vtkGetMacro(GenerateOriginalIds, bool);
  //@}

  //@{
  /**
   * When set (not the default) the connectivity of each cell will be added by
   * adding a new column for each point.
   */
  vtkSetMacro(GenerateCellConnectivity, bool);
  vtkGetMacro(GenerateCellConnectivity, bool);
  //@}

protected:
  vtkAttributeDataToTableFilter();
  ~vtkAttributeDataToTableFilter() override;

  // Overridden to indicate to the executive that we accept non-composite
  // datasets. We let the executive manage the looping over the composite
  // dataset leaves.
  int FillInputPortInformation(int port, vtkInformation* info) override;

  /**
   * Perform the data processing
   */
  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  /**
   * Create a default executive.
   */
  vtkExecutive* CreateDefaultExecutive() override;

  /**
   * Called when AddMetaData is true. Adds meta-data to the output.
   */
  void Decorate(vtkTable* output, vtkDataObject* input);

  void PassFieldData(vtkFieldData* output, vtkFieldData* input);
  void AddCellTypeAndConnectivity(vtkTable* output, vtkDataSet* ds);

  int FieldAssociation;
  bool AddMetaData;
  bool GenerateOriginalIds;
  bool GenerateCellConnectivity;

private:
  vtkAttributeDataToTableFilter(const vtkAttributeDataToTableFilter&) = delete;
  void operator=(const vtkAttributeDataToTableFilter&) = delete;

  /**
   * If original Ids for points are available (due to selection filtering),
   * re-map the indices to original Ids.
   */
  void ConvertToOriginalIds(vtkDataSet* inputDS, vtkIdTypeArray* indices);
};

#endif
