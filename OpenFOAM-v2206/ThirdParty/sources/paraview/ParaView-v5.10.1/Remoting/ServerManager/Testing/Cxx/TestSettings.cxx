/*=========================================================================

Program:   ParaView
Module:    TestSettings.cxx

Copyright (c) Kitware, Inc.
All rights reserved.
See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkInitializationHelper.h"
#include "vtkNew.h"
#include "vtkProcessModule.h"
#include "vtkSMDoubleVectorProperty.h"
#include "vtkSMParaViewPipelineController.h"
#include "vtkSMPropertyHelper.h"
#include "vtkSMProxy.h"
#include "vtkSMProxyListDomain.h"
#include "vtkSMProxyProperty.h"
#include "vtkSMSession.h"
#include "vtkSMSessionProxyManager.h"
#include "vtkSMSettings.h"
#include "vtkSmartPointer.h"

#include <cassert>
#include <sstream>
#include <vtk_jsoncpp.h>
int TestSettings(int argc, char* argv[])
{
  (void)argc;
  vtkInitializationHelper::Initialize(argv[0], vtkProcessModule::PROCESS_CLIENT);

  // Set up settings
  const char* settingString = "{\n"
                              "  \"sources\" : {\n"
                              "    \"SphereSource\" : {\n"
                              "      \"Radius\" : 4.25,\n"
                              "      \"Center\" : [1, 2, 3]\n"
                              "    }\n"
                              "  }\n"
                              "}\n";
  cout << settingString;

  vtkSMSettings* settings = vtkSMSettings::GetInstance();
  // cleanup because vtkInitialixationHelper looks for settings on disk
  settings->ClearAllSettings();
  settings->AddCollectionFromString(settingString, 1.0);

  const char* higherPrioritySettingsString = "{\n"
                                             "  \"sources\" : {\n"
                                             "    \"SphereSource\" : {\n"
                                             "      \"Radius\" : 1.0\n"
                                             "    }\n"
                                             "  }\n"
                                             "}\n";

  settings->AddCollectionFromString(higherPrioritySettingsString, 2.0);

  vtkNew<vtkSMParaViewPipelineController> controller;

  // Create a new session.
  vtkSMSession* session = vtkSMSession::New();
  vtkSMSessionProxyManager* pxm = session->GetSessionProxyManager();
  if (!controller->InitializeSession(session))
  {
    cerr << "Failed to initialize ParaView session." << endl;
    return EXIT_FAILURE;
  }

  // Create sphere source
  vtkSmartPointer<vtkSMProxy> sphere;
  sphere.TakeReference(pxm->NewProxy("sources", "SphereSource"));
  controller->PreInitializeProxy(sphere);
  controller->PostInitializeProxy(sphere);

  // Check the sphere radius. It should be set from the settings.
  vtkSMDoubleVectorProperty* radiusProperty =
    vtkSMDoubleVectorProperty::SafeDownCast(sphere->GetProperty("Radius"));
  if (radiusProperty)
  {
    if (radiusProperty->GetElement(0) != 1.0)
    {
      cerr << "Failed at " << __LINE__ << endl;
      return EXIT_FAILURE;
    }

    // Check the sphere center. It should be set from the settings.
    vtkSMDoubleVectorProperty* centerProperty =
      vtkSMDoubleVectorProperty::SafeDownCast(sphere->GetProperty("Center"));
    if (centerProperty->GetElement(0) != 1.0 || centerProperty->GetElement(1) != 2.0 ||
      centerProperty->GetElement(2) != 3.0)
    {
      cerr << "Failed at " << __LINE__ << endl;
      return EXIT_FAILURE;
    }
  }
  else
  {
    cerr << "Could not get Radius property\n";
  }

  // Test saving different number of repeatable property values
  // NOTE: Contour property is not available in all editions, so it's possible the
  // Contour filter is not defined. Handle that case.
  if (pxm->HasDefinition("filters", "Contour"))
  {
    vtkSmartPointer<vtkSMProxy> contour;
    contour.TakeReference(pxm->NewProxy("filters", "Contour"));
    controller->PreInitializeProxy(contour);
    vtkSMPropertyHelper(contour, "Input").Set(sphere);
    controller->PostInitializeProxy(contour);

    vtkSMDoubleVectorProperty* contourValuesProperty =
      vtkSMDoubleVectorProperty::SafeDownCast(contour->GetProperty("ContourValues"));
    if (!contourValuesProperty)
    {
      std::cerr << "No contour values property in GenericContour\n";
      return EXIT_FAILURE;
    }

    // Double vector property resize
    contourValuesProperty->SetNumberOfElements(1);
    contourValuesProperty->SetElement(0, -1.0);
    settings->SetProxySettings(contour);

    contourValuesProperty->SetNumberOfElements(2);
    contourValuesProperty->SetElement(0, -2.0);
    contourValuesProperty->SetElement(1, -3.0);
    settings->SetProxySettings(contour);

    auto contourLocatorProperty = vtkSMProxyProperty::SafeDownCast(contour->GetProperty("Locator"));
    if (!contourLocatorProperty)
    {
      std::cerr << "No contour locator property in GenericContour" << std::endl;
      return EXIT_FAILURE;
    }

    auto proxyListDomain = contourLocatorProperty->FindDomain<vtkSMProxyListDomain>();
    vtkSMProxy* locator0 = proxyListDomain->GetProxy(0);
    vtkSMProxy* locator1 = proxyListDomain->GetProxy(1);
    contourLocatorProperty->SetProxy(0, locator1);
    settings->SetProxySettings(contour);
    contourLocatorProperty->SetProxy(0, locator0);

    settings->GetProxySettings(contour);
    contour->ResetPropertiesToDefault();

    if (strcmp(contourLocatorProperty->GetProxy(0)->GetXMLName(), locator1->GetXMLName()) != 0)
    {
      std::cerr << "Wrong selected locator. Has "
                << contourLocatorProperty->GetProxy(0)->GetXMLName() << " instead of "
                << locator1->GetXMLName() << std::endl;
      std::cerr << *settings << std::endl;
      return EXIT_FAILURE;
    }
  }

  vtkSMPropertyHelper(sphere, "Radius").Set(12);
  Json::Value state = vtkSMSettings::SerializeAsJSON(sphere);
  cout << state.toStyledString() << endl;

  vtkSMPropertyHelper(sphere, "Radius").Set(1);
  if (!vtkSMSettings::DeserializeFromJSON(sphere, state) ||
    vtkSMPropertyHelper(sphere, "Radius").GetAsInt() != 12)
  {
    cerr << "Failed to DeserializeFromJSON." << endl;
    return EXIT_FAILURE;
  }

  // avoid writing our test settings on disk.
  settings->ClearAllSettings();
  session->Delete();
  vtkInitializationHelper::Finalize();
  return EXIT_SUCCESS;
}
