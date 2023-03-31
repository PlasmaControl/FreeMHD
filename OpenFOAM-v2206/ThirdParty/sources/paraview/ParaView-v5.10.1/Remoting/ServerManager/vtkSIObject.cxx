/*=========================================================================

  Program:   ParaView
  Module:    vtkSIObject.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkSIObject.h"

#include "vtkClientServerInterpreter.h"
#include "vtkObjectFactory.h"
#include "vtkPVSessionCore.h"
#include "vtkSMMessage.h"

#include <cassert>

vtkStandardNewMacro(vtkSIObject);
//----------------------------------------------------------------------------
vtkSIObject::vtkSIObject()
{
  this->Interpreter = nullptr;
  this->SessionCore = nullptr;
  this->GlobalID = 0;
  this->LastPushedMessage = new vtkSMMessage();
}

//----------------------------------------------------------------------------
vtkSIObject::~vtkSIObject()
{
  delete this->LastPushedMessage;
  this->LastPushedMessage = nullptr;
}

//----------------------------------------------------------------------------
void vtkSIObject::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//----------------------------------------------------------------------------
void vtkSIObject::Initialize(vtkPVSessionCore* session)
{
  assert(session != nullptr);
  this->SessionCore = session;
  this->Interpreter = session->GetInterpreter();
}

//----------------------------------------------------------------------------
vtkSIObject* vtkSIObject::GetSIObject(vtkTypeUInt32 globalid) const
{
  if (this->SessionCore)
  {
    return this->SessionCore->GetSIObject(globalid);
  }
  return nullptr;
}
//----------------------------------------------------------------------------
vtkObject* vtkSIObject::GetRemoteObject(vtkTypeUInt32 globalid)
{
  if (this->SessionCore)
  {
    return this->SessionCore->GetRemoteObject(globalid);
  }
  return nullptr;
}

//----------------------------------------------------------------------------
vtkClientServerInterpreter* vtkSIObject::GetInterpreter()
{
  return this->Interpreter;
}
//----------------------------------------------------------------------------
void vtkSIObject::Push(vtkSMMessage* msg)
{
  this->LastPushedMessage->CopyFrom(*msg);
}

//----------------------------------------------------------------------------
void vtkSIObject::Pull(vtkSMMessage* msg)
{
  msg->CopyFrom(*this->LastPushedMessage);
}
