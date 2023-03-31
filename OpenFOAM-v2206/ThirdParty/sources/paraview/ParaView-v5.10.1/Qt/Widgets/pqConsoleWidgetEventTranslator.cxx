/*=========================================================================

   Program: ParaView
   Module:  pqConsoleWidgetEventTranslator.cxx

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

========================================================================*/
#include "pqConsoleWidgetEventTranslator.h"

#include "pqConsoleWidget.h"
#include <QEvent>

//-----------------------------------------------------------------------------
pqConsoleWidgetEventTranslator::pqConsoleWidgetEventTranslator(QObject* parentObject)
  : Superclass(parentObject)
{
}

//-----------------------------------------------------------------------------
pqConsoleWidgetEventTranslator::~pqConsoleWidgetEventTranslator() = default;

//-----------------------------------------------------------------------------
bool pqConsoleWidgetEventTranslator::translateEvent(
  QObject* target, QEvent* qtevent, bool& errorFlag)
{
  Q_UNUSED(errorFlag);
  // Capture inputs for pqConsoleWidget and all its children
  pqConsoleWidget* object = nullptr;
  for (QObject* current = target; current != nullptr; current = current->parent())
  {
    object = qobject_cast<pqConsoleWidget*>(current);
    if (object)
    {
      break;
    }
  }
  if (!object)
  {
    return false;
  }

  if (qtevent->type() == QEvent::FocusIn)
  {
    if (this->CurrentObject)
    {
      QObject::disconnect(this->CurrentObject, nullptr, this, nullptr);
    }
    this->CurrentObject = object;
    QObject::connect(this->CurrentObject, SIGNAL(executeCommand(const QString&)), this,
      SLOT(recordCommand(const QString&)));
  }

  return true;
}

//-----------------------------------------------------------------------------
void pqConsoleWidgetEventTranslator::recordCommand(const QString& text)
{
  Q_EMIT this->recordEvent(this->CurrentObject, "executeCommand", text);
}
