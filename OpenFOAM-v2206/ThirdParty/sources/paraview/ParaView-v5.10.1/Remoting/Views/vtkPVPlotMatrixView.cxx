/*=========================================================================

  Program:   ParaView
  Module:    vtkPVPlotMatrixView.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "vtkPVPlotMatrixView.h"

#include "vtkAnnotationLink.h"
#include "vtkCommand.h"
#include "vtkContextScene.h"
#include "vtkContextView.h"
#include "vtkObjectFactory.h"
#include "vtkPVStringFormatter.h"
#include "vtkRenderWindow.h"
#include "vtkScatterPlotMatrix.h"
#include "vtkTextProperty.h"

#include "vtksys/SystemTools.hxx"

vtkStandardNewMacro(vtkPVPlotMatrixView);

// macros are split so that we pass HeaderTesting.py
//----------------------------------------------------------------------------
#define GENERATE_PLOT_TYPE_DEFINITION(name, type)                                                  \
  void vtkPVPlotMatrixView::SetScatterPlot##name(type value)                                       \
  {                                                                                                \
    Set##name(vtkScatterPlotMatrix::SCATTERPLOT, value);                                           \
  }                                                                                                \
  void vtkPVPlotMatrixView::SetHistogram##name(type value)                                         \
  {                                                                                                \
    Set##name(vtkScatterPlotMatrix::HISTOGRAM, value);                                             \
  }                                                                                                \
  void vtkPVPlotMatrixView::SetActivePlot##name(type value)                                        \
  {                                                                                                \
    Set##name(vtkScatterPlotMatrix::ACTIVEPLOT, value);                                            \
  }

#define GENERATE_PLOT_TYPE_DEFINITION2(name, type1, type2)                                         \
  void vtkPVPlotMatrixView::SetScatterPlot##name(type1 value1, type2 value2)                       \
  {                                                                                                \
    Set##name(vtkScatterPlotMatrix::SCATTERPLOT, value1, value2);                                  \
  }                                                                                                \
  void vtkPVPlotMatrixView::SetHistogram##name(type1 value1, type2 value2)                         \
  {                                                                                                \
    Set##name(vtkScatterPlotMatrix::HISTOGRAM, value1, value2);                                    \
  }                                                                                                \
  void vtkPVPlotMatrixView::SetActivePlot##name(type1 value1, type2 value2)                        \
  {                                                                                                \
    Set##name(vtkScatterPlotMatrix::ACTIVEPLOT, value1, value2);                                   \
  }

#define GENERATE_PLOT_TYPE_DEFINITION3(name, type1, type2, type3)                                  \
  void vtkPVPlotMatrixView::SetScatterPlot##name(type1 value1, type2 value2, type3 value3)         \
  {                                                                                                \
    Set##name(vtkScatterPlotMatrix::SCATTERPLOT, value1, value2, value3);                          \
  }                                                                                                \
  void vtkPVPlotMatrixView::SetHistogram##name(type1 value1, type2 value2, type3 value3)           \
  {                                                                                                \
    Set##name(vtkScatterPlotMatrix::HISTOGRAM, value1, value2, value3);                            \
  }                                                                                                \
  void vtkPVPlotMatrixView::SetActivePlot##name(type1 value1, type2 value2, type3 value3)          \
  {                                                                                                \
    Set##name(vtkScatterPlotMatrix::ACTIVEPLOT, value1, value2, value3);                           \
  }

#define GENERATE_PLOT_TYPE_DEFINITION4(name, type1, type2, type3, type4)                           \
  void vtkPVPlotMatrixView::SetScatterPlot##name(                                                  \
    type1 value1, type2 value2, type3 value3, type4 value4)                                        \
  {                                                                                                \
    Set##name(vtkScatterPlotMatrix::SCATTERPLOT, value1, value2, value3, value4);                  \
  }                                                                                                \
  void vtkPVPlotMatrixView::SetHistogram##name(                                                    \
    type1 value1, type2 value2, type3 value3, type4 value4)                                        \
  {                                                                                                \
    Set##name(vtkScatterPlotMatrix::HISTOGRAM, value1, value2, value3, value4);                    \
  }                                                                                                \
  void vtkPVPlotMatrixView::SetActivePlot##name(                                                   \
    type1 value1, type2 value2, type3 value3, type4 value4)                                        \
  {                                                                                                \
    Set##name(vtkScatterPlotMatrix::ACTIVEPLOT, value1, value2, value3, value4);                   \
  }

GENERATE_PLOT_TYPE_DEFINITION(GridVisibility, bool);
GENERATE_PLOT_TYPE_DEFINITION4(BackgroundColor, double, double, double, double);
GENERATE_PLOT_TYPE_DEFINITION3(AxisColor, double, double, double);
GENERATE_PLOT_TYPE_DEFINITION3(GridColor, double, double, double);
GENERATE_PLOT_TYPE_DEFINITION(AxisLabelVisibility, bool);
GENERATE_PLOT_TYPE_DEFINITION(AxisLabelFontFamily, const char*);
GENERATE_PLOT_TYPE_DEFINITION(AxisLabelFontFile, const char*);
GENERATE_PLOT_TYPE_DEFINITION(AxisLabelFontSize, int);
GENERATE_PLOT_TYPE_DEFINITION(AxisLabelBold, bool);
GENERATE_PLOT_TYPE_DEFINITION(AxisLabelItalic, bool);
GENERATE_PLOT_TYPE_DEFINITION3(AxisLabelColor, double, double, double);
GENERATE_PLOT_TYPE_DEFINITION(AxisLabelNotation, int);
GENERATE_PLOT_TYPE_DEFINITION(AxisLabelPrecision, int);
GENERATE_PLOT_TYPE_DEFINITION(TooltipNotation, int);
GENERATE_PLOT_TYPE_DEFINITION(TooltipPrecision, int);

//----------------------------------------------------------------------------
vtkPVPlotMatrixView::vtkPVPlotMatrixView()
{
  this->PlotMatrix = vtkScatterPlotMatrix::New();
  this->PlotMatrix->AddObserver(
    vtkCommand::SelectionChangedEvent, this, &vtkPVPlotMatrixView::PlotMatrixSelectionCallback);
  this->PlotMatrix->AddObserver(
    vtkCommand::AnnotationChangedEvent, this, &vtkPVPlotMatrixView::PlotMatrixSelectionCallback);
  this->PlotMatrix->AddObserver(
    vtkCommand::CreateTimerEvent, this, &vtkPVPlotMatrixView::PlotMatrixSelectionCallback);
  this->PlotMatrix->AddObserver(
    vtkCommand::AnimationCueTickEvent, this, &vtkPVPlotMatrixView::PlotMatrixSelectionCallback);

  this->ContextView->GetScene()->AddItem(this->PlotMatrix);
  // vtkScatterPlotMatrix always comes with a valid vtkAnnotationLink setup, so
  // we don't have do anything here.
}

//----------------------------------------------------------------------------
vtkPVPlotMatrixView::~vtkPVPlotMatrixView()
{
  this->PlotMatrix->Delete();
}

//----------------------------------------------------------------------------
vtkAbstractContextItem* vtkPVPlotMatrixView::GetContextItem()
{
  return this->PlotMatrix;
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetSelection(vtkChartRepresentation* repr, vtkSelection* selection)
{
  (void)repr;

  // we don't support multiple selection for now.
  this->PlotMatrix->GetAnnotationLink()->SetCurrentSelection(selection);
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::PlotMatrixSelectionCallback(vtkObject*, unsigned long event, void*)
{
  // forward the SelectionChangedEvent and InteractionEvent
  this->InvokeEvent(event);
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetActivePlot(int i, int j)
{
  if (this->PlotMatrix)
  {
    this->PlotMatrix->SetActivePlot(vtkVector2i(i, j));
  }
}

//----------------------------------------------------------------------------
int vtkPVPlotMatrixView::GetActiveRow()
{
  if (this->PlotMatrix)
  {
    return this->PlotMatrix->GetActivePlot().GetX();
  }
  return 0;
}

//----------------------------------------------------------------------------
int vtkPVPlotMatrixView::GetActiveColumn()
{
  if (this->PlotMatrix)
  {
    return this->PlotMatrix->GetActivePlot().GetY();
  }
  return 0;
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::ClearAnimationPath()
{
  this->PlotMatrix->ClearAnimationPath();
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::AddAnimationPath(int i, int j)
{
  this->PlotMatrix->AddAnimationPath(vtkVector2i(i, j));
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::StartAnimationPath()
{
  this->PlotMatrix->BeginAnimationPath(this->GetRenderWindow()->GetInteractor());
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::AdvanceAnimationPath()
{
  this->PlotMatrix->AdvanceAnimation();
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetTitleFont(const char* family, int pointSize, bool bold, bool italic)
{
  if (this->PlotMatrix)
  {
    vtkTextProperty* prop = this->PlotMatrix->GetTitleProperties();
    prop->SetFontFamilyAsString(family);
    prop->SetFontSize(pointSize);
    prop->SetBold(bold);
    prop->SetItalic(italic);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetTitleFontFamily(const char* family)
{
  if (this->PlotMatrix)
  {
    vtkTextProperty* prop = this->PlotMatrix->GetTitleProperties();
    prop->SetFontFamilyAsString(family);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetTitleFontSize(int pointSize)
{
  if (this->PlotMatrix)
  {
    vtkTextProperty* prop = this->PlotMatrix->GetTitleProperties();
    prop->SetFontSize(pointSize);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetTitleFontFile(const char* file)
{
  if (this->PlotMatrix)
  {
    vtkTextProperty* prop = this->PlotMatrix->GetTitleProperties();
    prop->SetFontFile(file);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetTitleBold(bool bold)
{
  if (this->PlotMatrix)
  {
    vtkTextProperty* prop = this->PlotMatrix->GetTitleProperties();
    prop->SetBold(bold);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetTitleItalic(bool italic)
{
  if (this->PlotMatrix)
  {
    vtkTextProperty* prop = this->PlotMatrix->GetTitleProperties();
    prop->SetItalic(italic);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetTitleColor(double red, double green, double blue)
{
  if (this->PlotMatrix)
  {
    vtkTextProperty* prop = this->PlotMatrix->GetTitleProperties();
    prop->SetColor(red, green, blue);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetTitleAlignment(int alignment)
{
  if (this->PlotMatrix)
  {
    vtkTextProperty* prop = this->PlotMatrix->GetTitleProperties();
    prop->SetJustification(alignment);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetGridVisibility(int plotType, bool visible)
{
  if (this->PlotMatrix)
  {
    this->PlotMatrix->SetGridVisibility(plotType, visible);
    this->PlotMatrix->UpdateChartSettings(plotType);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetNumberOfAnimationFrames(int value)
{
  if (this->PlotMatrix)
  {
    this->PlotMatrix->SetNumberOfFrames(value);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetGutter(float x, float y)
{
  if (this->PlotMatrix)
  {
    vtkVector2f gutter(x, y);
    this->PlotMatrix->SetGutter(gutter);
  }
}
//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetGutterX(float value)
{
  if (this->PlotMatrix)
  {
    this->PlotMatrix->SetGutterX(value);
  }
}
//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetGutterY(float value)
{
  if (this->PlotMatrix)
  {
    this->PlotMatrix->SetGutterY(value);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetBorders(int left, int bottom, int right, int top)
{
  if (this->PlotMatrix)
  {
    this->PlotMatrix->SetBorders(left, bottom, right, top);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetBorderLeft(int value)
{
  if (this->PlotMatrix)
  {
    this->PlotMatrix->SetBorderLeft(value);
  }
}
//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetBorderBottom(int value)
{
  if (this->PlotMatrix)
  {
    this->PlotMatrix->SetBorderBottom(value);
  }
}
//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetBorderRight(int value)
{
  if (this->PlotMatrix)
  {
    this->PlotMatrix->SetBorderRight(value);
  }
}
//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetBorderTop(int value)
{
  if (this->PlotMatrix)
  {
    this->PlotMatrix->SetBorderTop(value);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetBackgroundColor(
  int plotType, double red, double green, double blue, double alpha)
{
  if (this->PlotMatrix)
  {
    vtkColor4ub color(static_cast<unsigned char>(red * 255),
      static_cast<unsigned char>(green * 255), static_cast<unsigned char>(blue * 255),
      static_cast<unsigned char>(alpha * 255));
    this->PlotMatrix->SetBackgroundColor(plotType, color);
    this->PlotMatrix->UpdateChartSettings(plotType);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetAxisColor(int plotType, double red, double green, double blue)
{
  if (this->PlotMatrix)
  {
    vtkColor4ub color(static_cast<unsigned char>(red * 255),
      static_cast<unsigned char>(green * 255), static_cast<unsigned char>(blue * 255), 255);
    this->PlotMatrix->SetAxisColor(plotType, color);
    this->PlotMatrix->UpdateChartSettings(plotType);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetGridColor(int plotType, double red, double green, double blue)
{
  if (this->PlotMatrix)
  {
    vtkColor4ub color(static_cast<unsigned char>(red * 255),
      static_cast<unsigned char>(green * 255), static_cast<unsigned char>(blue * 255), 255);
    this->PlotMatrix->SetGridColor(plotType, color);
    this->PlotMatrix->UpdateChartSettings(plotType);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetAxisLabelVisibility(int plotType, bool visible)
{
  if (this->PlotMatrix)
  {
    this->PlotMatrix->SetAxisLabelVisibility(plotType, visible);
    this->PlotMatrix->UpdateChartSettings(plotType);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetAxisLabelFont(
  int plotType, const char* family, int pointSize, bool bold, bool italic)
{
  if (this->PlotMatrix)
  {
    vtkTextProperty* prop = this->PlotMatrix->GetAxisLabelProperties(plotType);
    prop->SetFontFamilyAsString(family);
    prop->SetFontSize(pointSize);
    prop->SetBold(bold);
    prop->SetItalic(italic);
    this->PlotMatrix->UpdateChartSettings(plotType);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetAxisLabelFontFamily(int plotType, const char* family)
{
  if (this->PlotMatrix)
  {
    vtkTextProperty* prop = this->PlotMatrix->GetAxisLabelProperties(plotType);
    prop->SetFontFamilyAsString(family);
    this->PlotMatrix->UpdateChartSettings(plotType);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetAxisLabelFontFile(int plotType, const char* file)
{
  if (this->PlotMatrix)
  {
    vtkTextProperty* prop = this->PlotMatrix->GetAxisLabelProperties(plotType);
    prop->SetFontFile(file);
    this->PlotMatrix->UpdateChartSettings(plotType);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetAxisLabelFontSize(int plotType, int pointSize)
{
  if (this->PlotMatrix)
  {
    vtkTextProperty* prop = this->PlotMatrix->GetAxisLabelProperties(plotType);
    prop->SetFontSize(pointSize);
    this->PlotMatrix->UpdateChartSettings(plotType);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetAxisLabelBold(int plotType, bool bold)
{
  if (this->PlotMatrix)
  {
    vtkTextProperty* prop = this->PlotMatrix->GetAxisLabelProperties(plotType);
    prop->SetBold(bold);
    this->PlotMatrix->UpdateChartSettings(plotType);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetAxisLabelItalic(int plotType, bool italic)
{
  if (this->PlotMatrix)
  {
    vtkTextProperty* prop = this->PlotMatrix->GetAxisLabelProperties(plotType);
    prop->SetItalic(italic);
    this->PlotMatrix->UpdateChartSettings(plotType);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetAxisLabelColor(int plotType, double red, double green, double blue)
{
  if (this->PlotMatrix)
  {
    vtkTextProperty* prop = this->PlotMatrix->GetAxisLabelProperties(plotType);
    prop->SetColor(red, green, blue);
    this->PlotMatrix->UpdateChartSettings(plotType);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetAxisLabelNotation(int plotType, int notation)
{
  if (this->PlotMatrix)
  {
    this->PlotMatrix->SetAxisLabelNotation(plotType, notation);
    this->PlotMatrix->UpdateChartSettings(plotType);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetAxisLabelPrecision(int plotType, int precision)
{
  if (this->PlotMatrix)
  {
    this->PlotMatrix->SetAxisLabelPrecision(plotType, precision);
    this->PlotMatrix->UpdateChartSettings(plotType);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetTooltipNotation(int plotType, int notation)
{
  if (this->PlotMatrix)
  {
    this->PlotMatrix->SetTooltipNotation(plotType, notation);
    this->PlotMatrix->UpdateChartSettings(plotType);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetTooltipPrecision(int plotType, int precision)
{
  if (this->PlotMatrix)
  {
    this->PlotMatrix->SetTooltipPrecision(plotType, precision);
    this->PlotMatrix->UpdateChartSettings(plotType);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetScatterPlotSelectedRowColumnColor(
  double red, double green, double blue, double alpha)
{
  if (this->PlotMatrix)
  {
    vtkColor4ub color(static_cast<unsigned char>(red * 255),
      static_cast<unsigned char>(green * 255), static_cast<unsigned char>(blue * 255),
      static_cast<unsigned char>(alpha * 255));
    this->PlotMatrix->SetScatterPlotSelectedRowColumnColor(color);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::SetScatterPlotSelectedActiveColor(
  double red, double green, double blue, double alpha)
{
  if (this->PlotMatrix)
  {
    vtkColor4ub color(static_cast<unsigned char>(red * 255),
      static_cast<unsigned char>(green * 255), static_cast<unsigned char>(blue * 255),
      static_cast<unsigned char>(alpha * 255));
    this->PlotMatrix->SetScatterPlotSelectedActiveColor(color);
  }
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  if (this->PlotMatrix)
  {
    this->PlotMatrix->PrintSelf(os, indent);
  }
}
//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::UpdateSettings()
{
  if (this->PlotMatrix)
  {
    this->PlotMatrix->UpdateSettings();
  }
}

//----------------------------------------------------------------------------
const char* vtkPVPlotMatrixView::GetTitleFontFamily()
{
  return this->PlotMatrix ? this->PlotMatrix->GetTitleProperties()->GetFontFamilyAsString()
                          : nullptr;
}

//----------------------------------------------------------------------------
int vtkPVPlotMatrixView::GetTitleFontSize()
{
  return this->PlotMatrix ? this->PlotMatrix->GetTitleProperties()->GetFontSize() : -1;
}

//----------------------------------------------------------------------------
int vtkPVPlotMatrixView::GetTitleFontBold()
{
  return this->PlotMatrix ? this->PlotMatrix->GetTitleProperties()->GetBold() : 0;
}

//----------------------------------------------------------------------------
int vtkPVPlotMatrixView::GetTitleFontItalic()
{
  return this->PlotMatrix ? this->PlotMatrix->GetTitleProperties()->GetItalic() : 0;
}

//----------------------------------------------------------------------------
double* vtkPVPlotMatrixView::GetTitleColor()
{
  // FIXME: Do we need this style of API? It means keeping around a double[3]
  // in order to support it. I don't like it, and would rather avoid it.
  return this->PlotMatrix ? this->PlotMatrix->GetTitleProperties()->GetColor() : nullptr;
}

//----------------------------------------------------------------------------
int vtkPVPlotMatrixView::GetTitleAlignment()
{
  return this->PlotMatrix ? this->PlotMatrix->GetTitleProperties()->GetJustification() : 0;
}

//----------------------------------------------------------------------------
int vtkPVPlotMatrixView::GetGridVisibility(int plotType)
{
  return this->PlotMatrix ? (this->PlotMatrix->GetGridVisibility(plotType) ? 1 : 0) : 0;
}

//----------------------------------------------------------------------------
bool vtkPVPlotMatrixView::GetBackgroundColor(int plotType, double color[3])
{
  if (!this->PlotMatrix)
  {
    return false;
  }

  auto tuple = this->PlotMatrix->GetBackgroundColor(plotType).Cast<double>();
  double* tmpColor = tuple.GetData();
  std::copy_n(tmpColor, 3, color);
  return true;
}

//----------------------------------------------------------------------------
bool vtkPVPlotMatrixView::GetAxisColor(int plotType, double color[3])
{
  if (!this->PlotMatrix)
  {
    return false;
  }

  auto tuple = this->PlotMatrix->GetAxisColor(plotType).Cast<double>();
  double* tmpColor = tuple.GetData();
  std::copy_n(tmpColor, 3, color);
  return true;
}

//----------------------------------------------------------------------------
bool vtkPVPlotMatrixView::GetGridColor(int plotType, double color[3])
{
  if (!this->PlotMatrix)
  {
    return false;
  }

  auto tuple = this->PlotMatrix->GetGridColor(plotType).Cast<double>();
  double* tmpColor = tuple.GetData();
  std::copy_n(tmpColor, 3, color);
  return true;
}

//----------------------------------------------------------------------------
int vtkPVPlotMatrixView::GetAxisLabelVisibility(int plotType)
{
  return this->PlotMatrix ? (this->PlotMatrix->GetAxisLabelVisibility(plotType) ? 1 : 0) : 0;
}

//----------------------------------------------------------------------------
const char* vtkPVPlotMatrixView::GetAxisLabelFontFamily(int plotType)
{
  return this->PlotMatrix
    ? this->PlotMatrix->GetAxisLabelProperties(plotType)->GetFontFamilyAsString()
    : nullptr;
}

//----------------------------------------------------------------------------
int vtkPVPlotMatrixView::GetAxisLabelFontSize(int plotType)
{
  return this->PlotMatrix ? this->PlotMatrix->GetAxisLabelProperties(plotType)->GetFontSize() : -1;
}

//----------------------------------------------------------------------------
int vtkPVPlotMatrixView::GetAxisLabelFontBold(int plotType)
{
  return this->PlotMatrix ? this->PlotMatrix->GetAxisLabelProperties(plotType)->GetBold() : 0;
}

//----------------------------------------------------------------------------
int vtkPVPlotMatrixView::GetAxisLabelFontItalic(int plotType)
{
  return this->PlotMatrix ? this->PlotMatrix->GetAxisLabelProperties(plotType)->GetItalic() : 0;
}

//----------------------------------------------------------------------------
bool vtkPVPlotMatrixView::GetAxisLabelColor(int plotType, double color[3])
{
  if (!this->PlotMatrix)
  {
    return false;
  }

  double* tmpColor = this->PlotMatrix->GetAxisLabelProperties(plotType)->GetColor();
  std::copy_n(tmpColor, 3, color);
  return true;
}

//----------------------------------------------------------------------------
int vtkPVPlotMatrixView::GetAxisLabelNotation(int plotType)
{
  return this->PlotMatrix ? this->PlotMatrix->GetAxisLabelNotation(plotType) : 0;
}

//----------------------------------------------------------------------------
int vtkPVPlotMatrixView::GetAxisLabelPrecision(int plotType)
{
  return this->PlotMatrix ? this->PlotMatrix->GetAxisLabelPrecision(plotType) : 0;
}

//----------------------------------------------------------------------------
int vtkPVPlotMatrixView::GetTooltipNotation(int plotType)
{
  return this->PlotMatrix ? this->PlotMatrix->GetTooltipNotation(plotType) : 0;
}
int vtkPVPlotMatrixView::GetTooltipPrecision(int plotType)
{
  return this->PlotMatrix ? this->PlotMatrix->GetTooltipPrecision(plotType) : 0;
}

//----------------------------------------------------------------------------
bool vtkPVPlotMatrixView::GetScatterPlotSelectedRowColumnColor(double color[3])
{
  if (!this->PlotMatrix)
  {
    return false;
  }

  auto tuple = this->PlotMatrix->GetScatterPlotSelectedRowColumnColor().Cast<double>();
  double* tmpColor = tuple.GetData();
  std::copy_n(tmpColor, 3, color);
  return true;
}

//----------------------------------------------------------------------------
bool vtkPVPlotMatrixView::GetScatterPlotSelectedActiveColor(double color[3])
{
  if (!this->PlotMatrix)
  {
    return false;
  }

  auto tuple = this->PlotMatrix->GetScatterPlotSelectedActiveColor().Cast<double>();
  double* tmpColor = tuple.GetData();
  std::copy_n(tmpColor, 3, color);
  return true;
}

//----------------------------------------------------------------------------
void vtkPVPlotMatrixView::Render(bool interactive)
{
  if (!this->PlotMatrix)
  {
    return;
  }

  // define scope with time
  PV_STRING_FORMATTER_NAMED_SCOPE("VIEW", fmt::arg("time", this->GetViewTime()));

  // check for old format
  if (this->GetTitle())
  {
    std::string formattedTitle = this->GetTitle();
    std::string possibleOldFormatString = formattedTitle;
    vtksys::SystemTools::ReplaceString(formattedTitle, "${TIME}", "{time}");
    if (possibleOldFormatString != formattedTitle)
    {
      vtkLogF(WARNING, "Legacy formatting pattern detected. Please replace '%s' with '%s'.",
        possibleOldFormatString.c_str(), formattedTitle.c_str());
    }
    this->SetTitle(formattedTitle.c_str());
  }

  this->PlotMatrix->SetTitle(this->GetFormattedTitle());

  this->Superclass::Render(interactive);
}

// Deprecated methods
#if !defined(VTK_LEGACY_REMOVE)
//----------------------------------------------------------------------------
double* vtkPVPlotMatrixView::GetBackgroundColor(int plotType)
{
  VTK_LEGACY_REPLACED_BODY("double* vtkPVPlotMatrixView::GetBackgroundColor(int plotType)",
    "ParaView 5.10", "bool vtkPVPlotMatrixView::GetBackgroundColor(int plotType, double color[3])");
  static double colorStore[3];
  return this->GetBackgroundColor(plotType, colorStore) ? colorStore : nullptr;
}

//----------------------------------------------------------------------------
double* vtkPVPlotMatrixView::GetAxisColor(int plotType)
{
  VTK_LEGACY_REPLACED_BODY("double* vtkPVPlotMatrixView::GetAxisColor(int plotType)",
    "ParaView 5.10", "bool vtkPVPlotMatrixView::GetAxisColor(int plotType, double color[3])");
  static double colorStore[3];
  return this->GetAxisColor(plotType, colorStore) ? colorStore : nullptr;
}

//----------------------------------------------------------------------------
double* vtkPVPlotMatrixView::GetGridColor(int plotType)
{
  VTK_LEGACY_REPLACED_BODY("double* vtkPVPlotMatrixView::GetGridColor(int plotType)",
    "ParaView 5.10", "bool vtkPVPlotMatrixView::GetGridColor(int plotType, double color[3])");
  static double colorStore[3];
  return this->GetGridColor(plotType, colorStore) ? colorStore : nullptr;
}

//----------------------------------------------------------------------------
double* vtkPVPlotMatrixView::GetAxisLabelColor(int plotType)
{
  VTK_LEGACY_REPLACED_BODY("double* vtkPVPlotMatrixView::GetAxisLabeldColor(int plotType)",
    "ParaView 5.10", "bool vtkPVPlotMatrixView::GetAxisLabelColor(int plotType, double color[3])");
  static double colorStore[3];
  return this->GetAxisLabelColor(plotType, colorStore) ? colorStore : nullptr;
}

//----------------------------------------------------------------------------
double* vtkPVPlotMatrixView::GetScatterPlotSelectedRowColumnColor()
{
  VTK_LEGACY_REPLACED_BODY("double* vtkPVPlotMatrixView::GetScatterPlotSelectedRowColumnColor()",
    "ParaView 5.10",
    "bool vtkPVPlotMatrixView::GetScatterPlotSelectedRowColumnColor(double color[3])");
  static double colorStore[3];
  return this->GetScatterPlotSelectedRowColumnColor(colorStore) ? colorStore : nullptr;
}

//----------------------------------------------------------------------------
double* vtkPVPlotMatrixView::GetScatterPlotSelectedActiveColor()
{
  VTK_LEGACY_REPLACED_BODY("double* vtkPVPlotMatrixView::GetScatterSelectedActiveColor()",
    "ParaView 5.10",
    "bool vtkPVPlotMatrixView::GetScatterPlotSelectedActiveColor(double color[3])");
  static double colorStore[3];
  return this->GetScatterPlotSelectedActiveColor(colorStore) ? colorStore : nullptr;
}
#endif
