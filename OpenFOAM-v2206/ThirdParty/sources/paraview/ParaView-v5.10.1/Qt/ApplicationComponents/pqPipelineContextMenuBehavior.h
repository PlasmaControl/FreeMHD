/*=========================================================================

   Program: ParaView
   Module:    $RCSfile$

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
#ifndef pqPipelineContextMenuBehavior_h
#define pqPipelineContextMenuBehavior_h

#include "pqApplicationComponentsModule.h"
#include "vtkType.h"
#include <QList> // needed for QList.
#include <QObject>
#include <QPoint> // needed for QPoint.
#include <QPointer>

class pqContextMenuInterface;
class pqDataRepresentation;
class pqPipelineRepresentation;
class pqView;
class QAction;
class QMenu;

/**
 * @ingroup Behaviors
 *
 * This behavior manages showing up of a context menu with sensible pipeline
 * related actions for changing color/visibility etc. when the user
 * right-clicks on top of an object in the 3D view. Currently, it only supports
 * views with proxies that vtkSMRenderViewProxy subclasses.
 */
class PQAPPLICATIONCOMPONENTS_EXPORT pqPipelineContextMenuBehavior : public QObject
{
  Q_OBJECT
  typedef QObject Superclass;

public:
  pqPipelineContextMenuBehavior(QObject* parent = 0);
  ~pqPipelineContextMenuBehavior() override;

protected Q_SLOTS:

  /**
   * Called when a new view is added. We add actions to the widget for context
   * menu if the view is a render-view.
   */
  void onViewAdded(pqView*);

protected:
  /**
   * called to build the context menu for the given representation. If the
   * picked representation was a composite data set the block index of the
   * selected block will be passed in blockIndex.
   *
   * With introduction of vtkPartitionedDataSetCollection and
   * vtkPartitionedDataSet, flatIndex is no longer consistent across ranks and
   * hence rank is also returned. Unless dealing with these data types, rank can
   * be ignored.
   */
  virtual void buildMenu(pqDataRepresentation* repr, unsigned int blockIndex, int rank);

  /**
   * event filter to capture the right-click. We don't directly use mechanisms
   * from QWidget to popup the context menu since all of those mechanism seem
   * to eat away the right button release, leaving the render window in a
   * dragging state.
   */
  bool eventFilter(QObject* caller, QEvent* e) override;

  QMenu* Menu;
  QPoint Position;

private:
  Q_DISABLE_COPY(pqPipelineContextMenuBehavior)
};

#endif
