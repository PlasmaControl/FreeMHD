/*=========================================================================

  Program:   ParaView
  Module:    vtkPVFileInformationHelper.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkPVFileInformationHelper.h"

#include "vtkObjectFactory.h"

#if defined(_WIN32)
#include <wchar.h>
#include <windows.h>
#endif

#include <vtksys/SystemTools.hxx>

vtkStandardNewMacro(vtkPVFileInformationHelper);
//-----------------------------------------------------------------------------
vtkPVFileInformationHelper::vtkPVFileInformationHelper()
  : Path(nullptr)
  , WorkingDirectory(nullptr)
  , DirectoryListing(0)
  , SpecialDirectories(0)
  , FastFileTypeDetection(1)
  , GroupFileSequences(true)
  , ReadDetailedFileInformation(false)
  , PathSeparator(nullptr)
{
  this->SetPath(".");
#if defined(_WIN32) && !defined(__CYGWIN__)
  this->SetPathSeparator("\\");
#else
  this->SetPathSeparator("/");
#endif
}

//-----------------------------------------------------------------------------
vtkPVFileInformationHelper::~vtkPVFileInformationHelper()
{
  this->SetPath(nullptr);
  this->SetPathSeparator(nullptr);
  this->SetWorkingDirectory(nullptr);
}

//-----------------------------------------------------------------------------
bool vtkPVFileInformationHelper::GetActiveFileIsReadable()
{
  return vtksys::SystemTools::FileExists(this->Path);
}

//-----------------------------------------------------------------------------
bool vtkPVFileInformationHelper::GetActiveFileIsDirectory()
{
  return vtksys::SystemTools::FileIsDirectory(this->Path);
}

//-----------------------------------------------------------------------------
void vtkPVFileInformationHelper::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Path: " << (this->Path ? this->Path : "(null)") << endl;
  os << indent
     << "WorkingDirectory: " << (this->WorkingDirectory ? this->WorkingDirectory : "(null)")
     << endl;
  os << indent << "DirectoryListing: " << this->DirectoryListing << endl;
  os << indent << "SpecialDirectories: " << this->SpecialDirectories << endl;
  os << indent << "PathSeparator: " << (this->PathSeparator ? this->PathSeparator : "(null)")
     << endl;
  os << indent << "FastFileTypeDetection: " << this->FastFileTypeDetection << endl;
}
