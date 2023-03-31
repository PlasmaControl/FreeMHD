/*=========================================================================

  Program:   ParaView
  Module:    vtkConduitSource.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkConduitSource.h"

#include "vtkCellArray.h"
#include "vtkCellArrayIterator.h"
#include "vtkConduitArrayUtilities.h"
#include "vtkDataArray.h"
#include "vtkDataAssembly.h"
#include "vtkDataSetAttributes.h"
#include "vtkDoubleArray.h"
#include "vtkImageData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkIntArray.h"
#include "vtkLogger.h"
#include "vtkNew.h"
#include "vtkObjectFactory.h"
#include "vtkPartitionedDataSet.h"
#include "vtkPartitionedDataSetCollection.h"
#include "vtkPoints.h"
#include "vtkRectilinearGrid.h"
#include "vtkSmartPointer.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkStringArray.h"
#include "vtkStructuredGrid.h"
#include "vtkUnstructuredGrid.h"

#include <catalyst_conduit.hpp>
#include <catalyst_conduit_blueprint.hpp>

#include <algorithm>
#include <functional>
#include <map>

namespace detail
{

//----------------------------------------------------------------------------
int GetAssociation(const std::string& assoc)
{
  if (assoc == "element")
  {
    return vtkDataObject::CELL;
  }
  else if (assoc == "vertex")
  {
    return vtkDataObject::POINT;
  }

  throw std::runtime_error("unsupported association " + assoc);
}

//----------------------------------------------------------------------------
int GetCellType(const std::string& shape)
{
  if (shape == "point")
  {
    return VTK_VERTEX;
  }
  else if (shape == "line")
  {
    return VTK_LINE;
  }
  else if (shape == "tri")
  {
    return VTK_TRIANGLE;
  }
  else if (shape == "quad")
  {
    return VTK_QUAD;
  }
  else if (shape == "tet")
  {
    return VTK_TETRA;
  }
  else if (shape == "hex")
  {
    return VTK_HEXAHEDRON;
  }
  else if (shape == "polyhedral")
  {
    return VTK_POLYHEDRON;
  }
  else if (shape == "polygonal")
  {
    return VTK_POLYGON;
  }
  else
  {
    throw std::runtime_error("unsupported shape " + shape);
  }
}

//----------------------------------------------------------------------------
// internal: get number of points in VTK cell type.
static vtkIdType GetNumberOfPointsInCellType(int vtk_cell_type)
{
  switch (vtk_cell_type)
  {
    case VTK_VERTEX:
      return 1;
    case VTK_LINE:
      return 2;
    case VTK_TRIANGLE:
      return 3;
    case VTK_QUAD:
    case VTK_TETRA:
      return 4;
    case VTK_HEXAHEDRON:
      return 8;
    default:
      throw std::runtime_error("unsupported cell type " + std::to_string(vtk_cell_type));
  }
}

//----------------------------------------------------------------------------
vtkSmartPointer<vtkPoints> CreatePoints(const conduit_cpp::Node& coords)
{
  if (coords["type"].as_string() != "explicit")
  {
    throw std::runtime_error("invalid node!");
  }

  conduit_cpp::Node values = coords["values"];
  auto array = vtkConduitArrayUtilities::MCArrayToVTKArray(conduit_cpp::c_node(&values), "coords");
  if (array == nullptr)
  {
    throw std::runtime_error("failed to convert to VTK array!");
  }
  if (array->GetNumberOfComponents() < 3)
  {
    array = vtkConduitArrayUtilities::SetNumberOfComponents(array, 3);
  }
  else if (array->GetNumberOfComponents() > 3)
  {
    throw std::runtime_error("points cannot have more than 3 components!");
  }

  vtkNew<vtkPoints> pts;
  pts->SetData(array);
  return pts;
}

//----------------------------------------------------------------------------
void SetPolyhedralCells(
  vtkUnstructuredGrid* grid, vtkCellArray* elements, vtkCellArray* subelements)
{
  vtkNew<vtkCellArray> connectivity;
  vtkNew<vtkIdTypeArray> faces;
  vtkNew<vtkIdTypeArray> faceLocations;

  connectivity->AllocateEstimate(elements->GetNumberOfCells(), 10);
  faces->Allocate(subelements->GetConnectivityArray()->GetNumberOfTuples());
  faceLocations->Allocate(elements->GetNumberOfCells());

  auto eIter = vtk::TakeSmartPointer(elements->NewIterator());
  auto seIter = vtk::TakeSmartPointer(subelements->NewIterator());

  std::vector<vtkIdType> cellPoints;
  for (eIter->GoToFirstCell(); !eIter->IsDoneWithTraversal(); eIter->GoToNextCell())
  {
    // init;
    cellPoints.clear();

    // get cell from 'elements'.
    vtkIdType size;
    vtkIdType const* seIds;
    eIter->GetCurrentCell(size, seIds);

    faceLocations->InsertNextValue(faces->GetNumberOfTuples());
    faces->InsertNextValue(size); // number-of-cell-faces.
    for (vtkIdType fIdx = 0; fIdx < size; ++fIdx)
    {
      seIter->GoToCell(seIds[fIdx]);

      vtkIdType ptSize;
      vtkIdType const* ptIds;
      seIter->GetCurrentCell(ptSize, ptIds);
      faces->InsertNextValue(ptSize); // number-of-face-points.
      for (vtkIdType ptIdx = 0; ptIdx < ptSize; ++ptIdx)
      {
        faces->InsertNextValue(ptIds[ptIdx]);
      }

      // accumulate pts from all faces in this cell to build the 'connectivity' array.
      std::copy(ptIds, ptIds + ptSize, std::back_inserter(cellPoints));
    }

    connectivity->InsertNextCell(
      static_cast<vtkIdType>(cellPoints.size()), cellPoints.empty() ? nullptr : &cellPoints[0]);
  }

  connectivity->Squeeze();
  faces->Squeeze();
  faceLocations->Squeeze();

  vtkNew<vtkUnsignedCharArray> cellTypes;
  cellTypes->SetNumberOfTuples(connectivity->GetNumberOfCells());
  cellTypes->FillValue(static_cast<unsigned char>(VTK_POLYHEDRON));
  grid->SetCells(cellTypes, connectivity, faceLocations, faces);
}

//----------------------------------------------------------------------------
vtkSmartPointer<vtkDataSet> GetMesh(
  const conduit_cpp::Node& topologyNode, const conduit_cpp::Node& coordsets)
{
  // get the coordset for this topology element.
  auto coords = coordsets[topologyNode["coordset"].as_string()];
  if (topologyNode["type"].as_string() == "uniform" && coords["type"].as_string() == "uniform")
  {
    vtkNew<vtkImageData> img;
    int dims[3] = { 1, 1, 1 };
    const char* dims_paths[] = { "dims/i", "dims/j", "dims/k" };
    double origin[3] = { 0, 0, 0 };
    const char* origin_paths[] = { "origin/x", "origin/y", "origin/z" };
    double spacing[3] = { 1, 1, 1 };
    const char* spacing_paths[] = { "spacing/dx", "spacing/dy", "spacing/dz" };
    for (int cc = 0; cc < 3; ++cc)
    {
      if (coords.has_path(dims_paths[cc]))
      {
        dims[cc] = coords[dims_paths[cc]].to_int32();
      }
      if (coords.has_path(origin_paths[cc]))
      {
        origin[cc] = coords[origin_paths[cc]].to_double();
      }
      if (coords.has_path(spacing_paths[cc]))
      {
        spacing[cc] = coords[spacing_paths[cc]].to_double();
      }
    }
    img->SetOrigin(origin);
    img->SetSpacing(spacing);
    img->SetDimensions(dims);
    return img;
  }
  else if (topologyNode["type"].as_string() == "rectilinear" &&
    coords["type"].as_string() == "rectilinear")
  {
    vtkNew<vtkRectilinearGrid> rg;
    conduit_cpp::Node values_x = coords["values/x"];
    conduit_cpp::Node values_y = coords["values/y"];
    conduit_cpp::Node values_z = coords["values/z"];
    auto xArray =
      vtkConduitArrayUtilities::MCArrayToVTKArray(conduit_cpp::c_node(&values_x), "xcoords");
    auto yArray =
      vtkConduitArrayUtilities::MCArrayToVTKArray(conduit_cpp::c_node(&values_y), "ycoords");
    auto zArray =
      vtkConduitArrayUtilities::MCArrayToVTKArray(conduit_cpp::c_node(&values_z), "zcoords");
    rg->SetDimensions(
      xArray->GetNumberOfTuples(), yArray->GetNumberOfTuples(), zArray->GetNumberOfTuples());
    rg->SetXCoordinates(xArray);
    rg->SetYCoordinates(xArray);
    rg->SetZCoordinates(xArray);
    return rg;
  }
  else if (topologyNode["type"].as_string() == "structured" &&
    coords["type"].as_string() == "explicit")
  {
    vtkNew<vtkStructuredGrid> sg;
    sg->SetPoints(CreatePoints(coords));
    sg->SetDimensions(
      topologyNode.has_path("elements/dims/i") ? topologyNode["elements/dims/i"].to_int32() + 1 : 1,
      topologyNode.has_path("elements/dims/j") ? topologyNode["elements/dims/j"].to_int32() + 1 : 1,
      topologyNode.has_path("elements/dims/k") ? topologyNode["elements/dims/k"].to_int32() + 1
                                               : 1);
    return sg;
  }
  else if (topologyNode["type"].as_string() == "unstructured" &&
    coords["type"].as_string() == "explicit")
  {
    vtkNew<vtkUnstructuredGrid> ug;
    conduit_cpp::Node connectivity = topologyNode["elements/connectivity"];
    const conduit_cpp::DataType dtype0 = connectivity.dtype();
    const auto nb_cells = dtype0.number_of_elements();
    if (nb_cells > 0)
    {
      ug->SetPoints(CreatePoints(coords));
      const auto vtk_cell_type = GetCellType(topologyNode["elements/shape"].as_string());
      if (vtk_cell_type == VTK_POLYHEDRON)
      {
        // polyhedra uses O2M and not M2C arrays, so need to process it
        // differently.
        conduit_cpp::Node t_elements = topologyNode["elements"];
        conduit_cpp::Node t_subelements = topologyNode["subelements"];
        auto elements = vtkConduitArrayUtilities::O2MRelationToVTKCellArray(
          conduit_cpp::c_node(&t_elements), "connectivity");
        auto subelements = vtkConduitArrayUtilities::O2MRelationToVTKCellArray(
          conduit_cpp::c_node(&t_subelements), "connectivity");

        // currently, this is an ugly deep-copy. Once vtkUnstructuredGrid is modified
        // as proposed here (vtk/vtk#18190), this will get simpler.
        SetPolyhedralCells(ug, elements, subelements);
      }
      else if (vtk_cell_type == VTK_POLYGON)
      {
        // polygons use O2M and not M2C arrays, so need to process it
        // differently.
        conduit_cpp::Node t_elements = topologyNode["elements"];
        auto cellArray = vtkConduitArrayUtilities::O2MRelationToVTKCellArray(
          conduit_cpp::c_node(&t_elements), "connectivity");
        ug->SetCells(vtk_cell_type, cellArray);
      }
      else
      {
        const auto cell_size = GetNumberOfPointsInCellType(vtk_cell_type);
        auto cellArray = vtkConduitArrayUtilities::MCArrayToVTKCellArray(
          cell_size, conduit_cpp::c_node(&connectivity));
        ug->SetCells(vtk_cell_type, cellArray);
      }
    }
    // if there are no cells in the Conduit mesh, return an empty ug
    return ug;
  }
  else
  {
    throw std::runtime_error("unsupported topology or coordset");
  }
}

//----------------------------------------------------------------------------
bool RequestMesh(vtkPartitionedDataSet* output, const conduit_cpp::Node& node)
{
  conduit_cpp::Node info;
  if (!conduit_cpp::BlueprintMesh::verify(node, info))
  {
    vtkLogF(ERROR, "Mesh blueprint verification failed!");
    return false;
  }

  std::map<std::string, vtkSmartPointer<vtkDataSet>> datasets;

  // process "topologies".
  auto topologies = node["topologies"];
  conduit_index_t nchildren = topologies.number_of_children();
  for (conduit_index_t i = 0; i < nchildren; ++i)
  {
    auto child = topologies.child(i);
    try
    {
      if (auto ds = detail::GetMesh(child, node["coordsets"]))
      {
        auto idx = output->GetNumberOfPartitions();
        output->SetPartition(idx, ds);
        output->GetMetaData(idx)->Set(vtkCompositeDataSet::NAME(), child.name().c_str());
        datasets[child.name()] = ds;
      }
    }
    catch (std::exception& e)
    {
      vtkLogF(ERROR, "failed to process '../topologies/%s'.", child.name().c_str());
      vtkLogF(ERROR, "ERROR: \n%s\n", e.what());
      return false;
    }
  }

  // process "fields"
  if (!node.has_path("fields"))
  {
    return true;
  }

  auto fields = node["fields"];
  nchildren = fields.number_of_children();
  for (conduit_index_t i = 0; i < nchildren; ++i)
  {
    auto fieldNode = fields.child(i);
    const auto fieldname = fieldNode.name();
    try
    {
      auto dataset = datasets.at(fieldNode["topology"].as_string());
      const auto vtk_association = detail::GetAssociation(fieldNode["association"].as_string());
      auto dsa = dataset->GetAttributes(vtk_association);
      auto values = fieldNode["values"];
      size_t dataset_size;
      if (values.number_of_children() == 0)
      {
        dataset_size = values.dtype().number_of_elements();
      }
      else
      {
        dataset_size = values.child(0).dtype().number_of_elements();
      }
      if (dataset_size > 0)
      {
        auto array =
          vtkConduitArrayUtilities::MCArrayToVTKArray(conduit_cpp::c_node(&values), fieldname);
        if (array->GetNumberOfTuples() != dataset->GetNumberOfElements(vtk_association))
        {
          throw std::runtime_error("mismatched tuple count!");
        }
        dsa->AddArray(array);
      }
    }
    catch (std::exception& e)
    {
      vtkLogF(ERROR, "failed to process '../fields/%s'.", fieldname.c_str());
      vtkLogF(ERROR, "ERROR: \n%s\n", e.what());
      return false;
    }
  }

  return true;
}

bool AddGlobalData(vtkDataObject* output, const conduit_cpp::Node& globalFields)
{
  auto fd = output->GetFieldData();

  // this can be made very generic. For now, I am only processing known
  // fields.
  if (globalFields.has_path("time"))
  {
    // for compatibility with older Catalyst scripts.
    vtkNew<vtkDoubleArray> timeValue;
    timeValue->SetName("TimeValue");
    timeValue->SetNumberOfTuples(1);
    timeValue->SetTypedComponent(0, 0, globalFields["time"].to_float64());
    fd->AddArray(timeValue);

    // "time" is a better name than "TimeValue"
    vtkNew<vtkDoubleArray> time;
    time->SetName("time");
    time->SetNumberOfTuples(1);
    time->SetTypedComponent(0, 0, globalFields["time"].to_float64());
    fd->AddArray(time);

    // let's also set DATA_TIME_STEP.
    output->GetInformation()->Set(
      vtkDataObject::DATA_TIME_STEP(), globalFields["time"].to_float64());
  }
  if (globalFields.has_path("cycle"))
  {
    vtkNew<vtkIntArray> cycle;
    cycle->SetName("cycle");
    cycle->SetNumberOfTuples(1);
    cycle->SetTypedComponent(0, 0, globalFields["cycle"].to_int64());
    fd->AddArray(cycle);
  }
  if (globalFields.has_path("timestep"))
  {
    vtkNew<vtkIntArray> timestep;
    timestep->SetName("timestep");
    timestep->SetNumberOfTuples(1);
    timestep->SetTypedComponent(0, 0, globalFields["timestep"].to_int64());
    fd->AddArray(timestep);
  }
  if (globalFields.has_path("channel"))
  {
    vtkNew<vtkStringArray> channel;
    channel->SetName("__CatalystChannel__");
    channel->InsertNextValue(globalFields["channel"].as_string().c_str());
    fd->AddArray(channel);
  }
  return true;
}

} // namespace detail

class vtkConduitSource::vtkInternals
{
public:
  conduit_cpp::Node Node;
  conduit_cpp::Node GlobalFieldsNode;
  conduit_cpp::Node AssemblyNode;
  bool GlobalFieldsNodeValid{ false };
  bool AssemblyNodeValid{ false };
};

vtkStandardNewMacro(vtkConduitSource);
//----------------------------------------------------------------------------
vtkConduitSource::vtkConduitSource()
  : Internals(new vtkConduitSource::vtkInternals())
  , UseMultiMeshProtocol(false)
{
  this->SetNumberOfInputPorts(0);
  this->SetNumberOfOutputPorts(1);
}

//----------------------------------------------------------------------------
vtkConduitSource::~vtkConduitSource() = default;

//----------------------------------------------------------------------------
void vtkConduitSource::SetNode(const conduit_node* node)
{
  if (conduit_cpp::c_node(&this->Internals->Node) == node)
  {
    return;
  }
  this->Internals->Node = conduit_cpp::cpp_node(const_cast<conduit_node*>(node));
  this->Modified();
}

//----------------------------------------------------------------------------
void vtkConduitSource::SetGlobalFieldsNode(const conduit_node* node)
{
  if (this->Internals->GlobalFieldsNodeValid &&
    conduit_cpp::c_node(&this->Internals->GlobalFieldsNode) == node)
  {
    return;
  }

  if (node)
  {
    this->Internals->GlobalFieldsNode = conduit_cpp::cpp_node(const_cast<conduit_node*>(node));
  }
  this->Internals->GlobalFieldsNodeValid = (node != nullptr);
  this->Modified();
}

//----------------------------------------------------------------------------
void vtkConduitSource::SetAssemblyNode(const conduit_node* node)
{
  if (this->Internals->AssemblyNodeValid &&
    conduit_cpp::c_node(&this->Internals->AssemblyNode) == node)
  {
    return;
  }

  if (node)
  {
    this->Internals->AssemblyNode = conduit_cpp::cpp_node(const_cast<conduit_node*>(node));
  }
  this->Internals->AssemblyNodeValid = (node != nullptr);
  this->Modified();
}

//----------------------------------------------------------------------------
int vtkConduitSource::RequestDataObject(
  vtkInformation*, vtkInformationVector**, vtkInformationVector* outputVector)
{
  const int dataType =
    this->UseMultiMeshProtocol ? VTK_PARTITIONED_DATA_SET_COLLECTION : VTK_PARTITIONED_DATA_SET;
  return this->SetOutputDataObject(dataType, outputVector->GetInformationObject(0), /*exact=*/true)
    ? 1
    : 0;
}

//----------------------------------------------------------------------------
int vtkConduitSource::RequestData(
  vtkInformation*, vtkInformationVector**, vtkInformationVector* outputVector)
{
  auto& internals = (*this->Internals);
  if (this->UseMultiMeshProtocol)
  {
    auto output = vtkPartitionedDataSetCollection::GetData(outputVector, 0);
    assert(output != nullptr);

    const auto& node = internals.Node;
    const auto count = node.number_of_children();
    output->SetNumberOfPartitionedDataSets(static_cast<unsigned int>(count));

    std::map<std::string, unsigned int> name_map;
    for (conduit_index_t cc = 0; cc < count; ++cc)
    {
      const auto child = node.child(cc);
      auto pd = output->GetPartitionedDataSet(static_cast<unsigned int>(cc));
      assert(pd != nullptr);
      if (!detail::RequestMesh(pd, child))
      {
        vtkLogF(ERROR, "Failed reading mesh '%s'", child.name().c_str());
        output->Initialize();
        return 0;
      }

      // set the mesh name.
      output->GetMetaData(cc)->Set(vtkCompositeDataSet::NAME(), child.name().c_str());
      name_map[child.name()] = static_cast<unsigned int>(cc);
    }

    if (internals.AssemblyNodeValid)
    {
      vtkNew<vtkDataAssembly> assembly;
      std::function<void(int, const conduit_cpp::Node&)> helper;
      helper = [&name_map, &assembly, &helper](int parent, const conduit_cpp::Node& node) {
        if (node.dtype().is_object())
        {
          for (conduit_index_t cc = 0; cc < node.number_of_children(); ++cc)
          {
            auto child = node.child(cc);
            auto nodeName = vtkDataAssembly::MakeValidNodeName(child.name().c_str());
            auto childId = assembly->AddNode(nodeName.c_str(), parent);
            assembly->SetAttribute(childId, "label", child.name().c_str());
            helper(childId, child);
          }
        }
        else if (node.dtype().is_list())
        {
          for (conduit_index_t cc = 0; cc < node.number_of_children(); ++cc)
          {
            auto child = node.child(cc);
            if (!child.dtype().is_string())
            {
              vtkLogF(ERROR, "list cannot have non-string items!");
              continue;
            }
            helper(parent, node.child(cc));
          }
        }
        else if (node.dtype().is_string())
        {
          auto value = node.as_string();
          auto iter = name_map.find(node.as_string());
          if (iter != name_map.end())
          {
            assembly->AddDataSetIndex(parent, iter->second);
          }
          else
          {
            vtkLogF(ERROR, "Assembly referring to unknown node '%s'. Skipping.", value.c_str());
          }
        }
      };
      // assembly->SetRootNodeName(....); What should this be?
      helper(assembly->GetRootNode(), internals.AssemblyNode);
      output->SetDataAssembly(assembly);
    }
  }
  else
  {
    auto output = vtkPartitionedDataSet::GetData(outputVector, 0);
    assert(output != nullptr);
    if (!detail::RequestMesh(output, internals.Node))
    {
      return 0;
    }
  }

  auto dobj = vtkDataObject::GetData(outputVector, 0);
  if (internals.GlobalFieldsNodeValid)
  {
    detail::AddGlobalData(dobj, internals.GlobalFieldsNode);
  }
  return 1;
}

//----------------------------------------------------------------------------
int vtkConduitSource::RequestInformation(
  vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  if (!this->Superclass::RequestInformation(request, inputVector, outputVector))
  {
    return 0;
  }

  vtkInformation* outInfo = outputVector->GetInformationObject(0);
  outInfo->Set(CAN_HANDLE_PIECE_REQUEST(), 1);

  if (!this->Internals->GlobalFieldsNodeValid)
  {
    return 1;
  }

  auto& node = this->Internals->GlobalFieldsNode;
  if (node.has_path("time"))
  {
    double time = node["time"].to_float64();
    double timesteps[2] = { time, time };
    outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_STEPS(), &time, 1);
    outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_RANGE(), timesteps, 2);
  }
  else
  {
    outInfo->Remove(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
    outInfo->Remove(vtkStreamingDemandDrivenPipeline::TIME_RANGE());
  }

  return 1;
}

//----------------------------------------------------------------------------
void vtkConduitSource::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
