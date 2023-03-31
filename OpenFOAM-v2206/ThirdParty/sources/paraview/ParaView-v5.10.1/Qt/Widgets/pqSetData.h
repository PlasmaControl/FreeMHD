/*=========================================================================

   Program: ParaView
   Module:    pqSetData.h

   Copyright (c) 2005-2008 Sandia Corporation, Kitware Inc.
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

=========================================================================*/

#ifndef _pqSetData_h
#define _pqSetData_h

#include "pqWidgetsModule.h"
#include <QVariant>

/**
  Using pqSetData, you can create and initialize Qt objects without having to create a bunch of
  temporaries:

  \code
  menu->addAction("Open") << pqSetData("My Private Data");
  \endcode

  \sa pqSetName, pqConnect
*/

struct PQWIDGETS_EXPORT pqSetData
{
  pqSetData(const QVariant& Data);
  const QVariant Data;

private:
  void operator=(const pqSetData&);
};

/**
 * Sets custom data for a Qt object
 */
template <typename T>
T* operator<<(T* LHS, const pqSetData& RHS)
{
  LHS->setData(RHS.Data);
  return LHS;
}

#endif // !_pqSetData_h
