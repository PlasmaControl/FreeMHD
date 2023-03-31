/*=========================================================================

Program:   ParaView
Module:    pvpython.cxx

Copyright (c) Kitware, Inc.
All rights reserved.
See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkPVConfig.h" // Required to get build options for paraview

extern "C"
{
  void vtkPVInitializePythonModules();
}

#include "vtkCLIOptions.h"
#include "vtkInitializationHelper.h"
#include "vtkLogger.h"
#include "vtkMultiProcessController.h"
#include "vtkPVPluginTracker.h"
#include "vtkProcessModule.h"
#include "vtkProcessModuleConfiguration.h"
#include "vtkPythonInterpreter.h"
#include "vtkRemotingCoreConfiguration.h"
#include "vtkSMProxyManager.h"
#include "vtkSMSession.h"

#include <vector>
#include <vtksys/SystemTools.hxx>

#include "ParaView_paraview_plugins.h"

namespace ParaViewPython
{

//---------------------------------------------------------------------------

void ProcessArgsForPython(std::vector<char*>& pythonArgs, const std::vector<std::string>& args,
  int vtkNotUsed(argc), char** argv)
{
  pythonArgs.clear();

  // push the executable name first.
  pythonArgs.push_back(vtksys::SystemTools::DuplicateString(argv[0]));

  // now push the unparsed arguments.
  if (args.empty())
  {
    return;
  }

  // here we handle a special case when the filename specified is a zip
  // archive.
  if (vtksys::SystemTools::GetFilenameLastExtension(args[0]) == ".zip")
  {
    // add the archive to sys.path
    vtkPythonInterpreter::PrependPythonPath(args[0].c_str());
    pythonArgs.push_back(vtksys::SystemTools::DuplicateString("-m"));

    std::string modulename = vtksys::SystemTools::GetFilenameWithoutLastExtension(
      vtksys::SystemTools::GetFilenameName(args[0].c_str()));
    pythonArgs.push_back(vtksys::SystemTools::DuplicateString(modulename.c_str()));
  }
  else
  {
    pythonArgs.push_back(vtksys::SystemTools::DuplicateString(args[0].c_str()));
  }

  for (size_t cc = 1, max = args.size(); cc < max; ++cc)
  {
    pythonArgs.push_back(vtksys::SystemTools::DuplicateString(args[cc].c_str()));
  }
}

//---------------------------------------------------------------------------
int Run(int processType, int argc, char* argv[])
{
  // Setup options
  vtkInitializationHelper::SetApplicationName("ParaView");

  auto options = vtk::TakeSmartPointer(vtkCLIOptions::New());
  const auto status = vtkInitializationHelper::Initialize(argc, argv, processType, options);
  if (!status)
  {
    return vtkInitializationHelper::GetExitCode();
  }

  if (processType == vtkProcessModule::PROCESS_BATCH && options->GetExtraArguments().empty())
  {
    vtkLogF(ERROR, "No script specified. Please specify a batch script or use 'pvpython'.");
    return EXIT_FAILURE;
  }

  vtkProcessModule* pm = vtkProcessModule::GetProcessModule();

  // register callback to initialize modules statically. The callback is
  // empty when BUILD_SHARED_LIBS is ON.
  vtkPVInitializePythonModules();

  // register static plugins
  ParaView_paraview_plugins_initialize();

  vtkPVPluginTracker::GetInstance()->LoadPluginConfigurationXMLs("paraview");

  int ret_val = 0;
  if (pm->GetSymmetricMPIMode() == false && pm->GetPartitionId() > 0)
  {
    vtkIdType sid = vtkSMSession::ConnectToSelf();
    pm->GetGlobalController()->ProcessRMIs();
    pm->UnRegisterSession(sid);
  }
  else
  {
    // Process arguments
    std::vector<char*> pythonArgs;
    ProcessArgsForPython(pythonArgs, options->GetExtraArguments(), argc, argv);
    pythonArgs.push_back(nullptr);

    // if user specified verbosity option on command line, then we make vtkPythonInterpreter post
    // log information as INFO, otherwise we leave it at default which is TRACE.
    auto pmConfig = vtkProcessModuleConfiguration::GetInstance();
    vtkPythonInterpreter::SetLogVerbosity(
      pmConfig->GetLogStdErrVerbosity() != vtkLogger::VERBOSITY_INVALID
        ? vtkLogger::VERBOSITY_INFO
        : vtkLogger::VERBOSITY_TRACE);

    // Start interpretor
    vtkPythonInterpreter::Initialize();

    ret_val =
      vtkPythonInterpreter::PyMain(static_cast<int>(pythonArgs.size()) - 1, &pythonArgs.front());

    // Free python args
    for (auto& ptr : pythonArgs)
    {
      delete[] ptr;
    }
  }
  // Exit application
  vtkInitializationHelper::Finalize();
  return ret_val;
}
}
