/*=========================================================================

  Program:   ParaView
  Module:    vtkSMSpreadSheetViewProxy.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkSMSpreadSheetViewProxy.h"

#include "vtkClientServerStream.h"
#include "vtkDataObject.h"
#include "vtkObjectFactory.h"
#include "vtkPVDataInformation.h"
#include "vtkPVDataSetAttributesInformation.h"
#include "vtkSMPropertyHelper.h"
#include "vtkSMSourceProxy.h"

vtkStandardNewMacro(vtkSMSpreadSheetViewProxy);
//----------------------------------------------------------------------------
vtkSMSpreadSheetViewProxy::vtkSMSpreadSheetViewProxy() = default;

//----------------------------------------------------------------------------
vtkSMSpreadSheetViewProxy::~vtkSMSpreadSheetViewProxy() = default;

//----------------------------------------------------------------------------
void vtkSMSpreadSheetViewProxy::CreateVTKObjects()
{
  if (this->ObjectsCreated)
  {
    return;
  }

  this->Superclass::CreateVTKObjects();
  if (!this->ObjectsCreated || this->Location == 0)
  {
    return;
  }

  vtkClientServerStream stream;
  stream << vtkClientServerStream::Invoke << VTKOBJECT(this) << "SetIdentifier"
         << this->GetGlobalID() << vtkClientServerStream::End;
  this->ExecuteStream(stream);
}

//----------------------------------------------------------------------------
void vtkSMSpreadSheetViewProxy::RepresentationVisibilityChanged(
  vtkSMProxy* repr, bool new_visibility)
{
  this->Superclass::RepresentationVisibilityChanged(repr, new_visibility);

  if (repr && new_visibility)
  {
    // when a new representation is being made visible, we try to pick
    // FieldAssociation that produces non-empty result.
    vtkSMPropertyHelper inputHelper(repr, "Input", true);
    if (auto input = vtkSMSourceProxy::SafeDownCast(inputHelper.GetAsProxy()))
    {
      auto dataInfo = input->GetDataInformation(inputHelper.GetOutputPort());
      vtkSMPropertyHelper fieldAssociationHelper(this, "FieldAssociation");
      if (dataInfo->GetNumberOfElements(fieldAssociationHelper.GetAsInt()) == 0)
      {
        // we define a list of attribute type in a different order than
        // the one described in vtkDataObject as we want the field data
        // to be considered if no other attribute is available.
        const std::vector<int> attributes = { vtkDataObject::POINT, vtkDataObject::CELL,
          vtkDataObject::VERTEX, vtkDataObject::EDGE, vtkDataObject::ROW, vtkDataObject::FIELD };

        // let's try to pick as association with non-empty tuples.
        for (int att : attributes)
        {
          if (dataInfo->GetNumberOfElements(att) > 0)
          {
            fieldAssociationHelper.Set(att);
            this->UpdateVTKObjects();
            break;
          }
        }
      }
    }
  }
}

//----------------------------------------------------------------------------
void vtkSMSpreadSheetViewProxy::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
