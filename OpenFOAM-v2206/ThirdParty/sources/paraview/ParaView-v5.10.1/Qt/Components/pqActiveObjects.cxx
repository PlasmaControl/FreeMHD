/*=========================================================================

   Program: ParaView
   Module:    pqActiveObjects.cxx

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
#include "pqActiveObjects.h"

#include "pqApplicationCore.h"
#include "pqMultiViewWidget.h"
#include "pqServerManagerModel.h"
#include "pqTabbedMultiViewWidget.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkSMOutputPort.h"
#include "vtkSMProxyManager.h"
#include "vtkSMProxySelectionModel.h"
#include "vtkSMSession.h"
#include "vtkSMViewLayoutProxy.h"

#include <QSignalBlocker>
#include <algorithm>

namespace
{
pqOutputPort* getPortFromPipelineProxy(pqProxy* proxy)
{
  if (auto port = qobject_cast<pqOutputPort*>(proxy))
  {
    return port;
  }
  if (auto source = qobject_cast<pqPipelineSource*>(proxy))
  {
    return source->getNumberOfOutputPorts() > 0 ? source->getOutputPort(0) : nullptr;
  }
  return nullptr;
}
}

//-----------------------------------------------------------------------------
pqActiveObjects& pqActiveObjects::instance()
{
  static pqActiveObjects activeObject;
  return activeObject;
}

//-----------------------------------------------------------------------------
pqActiveObjects::pqActiveObjects()
  : CachedServer(nullptr)
  , CachedPipelineProxy(nullptr)
  , CachedSource(nullptr)
  , CachedPort(nullptr)
  , CachedView(nullptr)
  , CachedRepresentation(nullptr)
{
  pqServerManagerModel* smmodel = pqApplicationCore::instance()->getServerManagerModel();
  QObject::connect(smmodel, SIGNAL(serverAdded(pqServer*)), this, SLOT(serverAdded(pqServer*)));
  QObject::connect(smmodel, SIGNAL(serverRemoved(pqServer*)), this, SLOT(serverRemoved(pqServer*)));
  QObject::connect(smmodel, SIGNAL(preItemRemoved(pqServerManagerModelItem*)), this,
    SLOT(proxyRemoved(pqServerManagerModelItem*)));

  QObject::connect(this, SIGNAL(viewChanged(pqView*)), this, SLOT(updateRepresentation()));
  QObject::connect(this, SIGNAL(portChanged(pqOutputPort*)), this, SLOT(updateRepresentation()));

  this->VTKConnector->Connect(vtkSMProxyManager::GetProxyManager(),
    vtkSMProxyManager::ActiveSessionChanged, this, SLOT(onActiveServerChanged()));

  QList<pqServer*> servers = smmodel->findItems<pqServer*>();
  if (servers.size() == 1)
  {
    this->setActiveServer(servers[0]);
  }
}

//-----------------------------------------------------------------------------
pqActiveObjects::~pqActiveObjects() = default;

//-----------------------------------------------------------------------------
void pqActiveObjects::resetActives()
{
  this->ActivePipelineProxy = nullptr;
  this->ActiveView = nullptr;
  this->ActiveRepresentation = nullptr;
  this->Selection.clear();
}

//-----------------------------------------------------------------------------
void pqActiveObjects::triggerSignals()
{
  if (this->signalsBlocked())
  {
    // don't update cached variables when signals are blocked.
    return;
  }

  if (this->ActiveServer.data() != this->CachedServer)
  {
    this->CachedServer = this->ActiveServer.data();
    Q_EMIT this->serverChanged(this->ActiveServer);
  }

  if (this->ActivePipelineProxy.data() != this->CachedPipelineProxy)
  {
    this->CachedPipelineProxy = this->ActivePipelineProxy.data();

    auto port = getPortFromPipelineProxy(this->ActivePipelineProxy);
    auto source = port ? port->getSource() : nullptr;

    if (source != this->CachedSource)
    {
      this->CachedSource = source;
      Q_EMIT this->sourceChanged(source);
    }

    if (port != this->CachedPort)
    {
      this->CachedPort = port;
      Q_EMIT this->portChanged(port);
    }

    Q_EMIT this->pipelineProxyChanged(this->ActivePipelineProxy);
  }

  if (this->ActiveRepresentation.data() != this->CachedRepresentation)
  {
    this->CachedRepresentation = this->ActiveRepresentation.data();
    Q_EMIT this->representationChanged(this->ActiveRepresentation);
    Q_EMIT this->representationChanged(
      static_cast<pqRepresentation*>(this->ActiveRepresentation.data()));
  }

  if (this->ActiveView.data() != this->CachedView)
  {
    this->CachedView = this->ActiveView.data();
    Q_EMIT this->viewChanged(this->ActiveView);
  }

  if (this->CachedSelection != this->Selection)
  {
    this->CachedSelection = this->Selection;
    Q_EMIT this->selectionChanged(this->Selection);
  }
}

//-----------------------------------------------------------------------------
void pqActiveObjects::serverAdded(pqServer* server)
{
  if (this->activeServer() == nullptr && server)
  {
    this->setActiveServer(server);
  }
}

//-----------------------------------------------------------------------------
void pqActiveObjects::serverRemoved(pqServer* server)
{
  if (this->activeServer() == server)
  {
    this->setActiveServer(nullptr);
  }
}

//-----------------------------------------------------------------------------
void pqActiveObjects::proxyRemoved(pqServerManagerModelItem* proxy)
{
  bool prev = this->blockSignals(true);

  if (this->ActivePipelineProxy == proxy)
  {
    this->setActivePipelineProxy(nullptr);
  }
  else if (this->ActiveView == proxy)
  {
    this->setActiveView(nullptr);
  }

  this->blockSignals(prev);
  this->triggerSignals();
}

//-----------------------------------------------------------------------------
void pqActiveObjects::viewSelectionChanged()
{
  pqServer* server = this->activeServer();
  if (!this->ActiveServer)
  {
    this->resetActives();
    this->triggerSignals();
    return;
  }

  if (server->activeViewSelectionModel() == nullptr)
  {
    // This mean that the servermanager is currently updating itself and no
    // selection manager is set yet...
    return;
  }

  vtkSMProxy* selectedProxy = nullptr;
  vtkSMProxySelectionModel* viewSelection = server->activeViewSelectionModel();
  if (viewSelection->GetNumberOfSelectedProxies() == 1)
  {
    selectedProxy = viewSelection->GetSelectedProxy(0);
  }
  else if (viewSelection->GetNumberOfSelectedProxies() > 1)
  {
    selectedProxy = viewSelection->GetCurrentProxy();
    if (selectedProxy && !viewSelection->IsSelected(selectedProxy))
    {
      selectedProxy = nullptr;
    }
  }

  pqView* view =
    pqApplicationCore::instance()->getServerManagerModel()->findItem<pqView*>(selectedProxy);

  if (this->ActiveView)
  {
    QObject::disconnect(this->ActiveView, nullptr, this, nullptr);
  }
  if (view)
  {
    QObject::connect(view, SIGNAL(representationAdded(pqRepresentation*)), this,
      SLOT(updateRepresentation()), Qt::UniqueConnection);
    QObject::connect(view, SIGNAL(representationRemoved(pqRepresentation*)), this,
      SLOT(updateRepresentation()), Qt::UniqueConnection);
  }

  this->ActiveView = view;

  // if view changed, then the active representation may have changed as well.
  this->updateRepresentation();
  // updateRepresentation calls triggerSignals().
}

//-----------------------------------------------------------------------------
void pqActiveObjects::sourceSelectionChanged()
{
  pqServer* server = this->activeServer();
  if (!this->ActiveServer)
  {
    this->resetActives();
    this->triggerSignals();
    return;
  }

  if (server->activeSourcesSelectionModel() == nullptr)
  {
    // This mean that the servermanager is currently updating itself and no
    // selection manager is set yet...
    return;
  }

  if (auto port = ::getPortFromPipelineProxy(this->ActivePipelineProxy))
  {
    port->disconnect(this);
    auto source = port->getSource();
    source->disconnect(this);
  }

  // Get the pqProxy for the current selected item.

  pqServerManagerModel* smmodel = pqApplicationCore::instance()->getServerManagerModel();
  vtkSMProxySelectionModel* selModel = server->activeSourcesSelectionModel();

  vtkSMProxy* currentProxy = selModel->GetCurrentProxy();
  auto proxy = smmodel->findItem<pqProxy*>(currentProxy);
  this->ActivePipelineProxy = proxy;

  if (auto port = ::getPortFromPipelineProxy(proxy))
  {
    this->connect(port, SIGNAL(representationAdded(pqOutputPort*, pqDataRepresentation*)),
      SLOT(updateRepresentation()));
    auto source = port->getSource();
    this->connect(source, SIGNAL(dataUpdated(pqPipelineSource*)), SIGNAL(dataUpdated()));
  }

  // Update the Selection.
  pqProxySelectionUtilities::copy(selModel, this->Selection);

  this->updateRepresentation();
  // updateRepresentation calls triggerSignals().
}

//-----------------------------------------------------------------------------
void pqActiveObjects::onActiveServerChanged()
{
  vtkSMSession* activeSession = vtkSMProxyManager::GetProxyManager()->GetActiveSession();
  if (activeSession == nullptr)
  {
    return;
  }

  pqServerManagerModel* smmodel = pqApplicationCore::instance()->getServerManagerModel();
  pqServer* newActiveServer = smmodel->findServer(activeSession);

  if (newActiveServer)
  {
    this->setActiveServer(newActiveServer);
  }
}
//-----------------------------------------------------------------------------

void pqActiveObjects::setActiveServer(pqServer* server)
{
  // Make sure the active server has the proper listener setup
  // in case of collaboration the object initialisation can cause a server to
  // become active before any selection model get setup
  if (this->ActiveServer == server && this->VTKConnector->GetNumberOfConnections() > 1)
  {
    return;
  }

  QSignalBlocker blocker(this);

  this->VTKConnector->Disconnect();
  this->ActiveServer = server;
  vtkSMProxyManager::GetProxyManager()->SetActiveSession(server ? server->session() : nullptr);

  // Need to connect it back to know if python is changing the active one
  this->VTKConnector->Connect(vtkSMProxyManager::GetProxyManager(),
    vtkSMProxyManager::ActiveSessionChanged, this, SLOT(onActiveServerChanged()));

  if (server && server->activeSourcesSelectionModel() && server->activeViewSelectionModel())
  {
    this->VTKConnector->Connect(server->activeSourcesSelectionModel(),
      vtkCommand::CurrentChangedEvent, this, SLOT(sourceSelectionChanged()));
    this->VTKConnector->Connect(server->activeSourcesSelectionModel(),
      vtkCommand::SelectionChangedEvent, this, SLOT(sourceSelectionChanged()));

    this->VTKConnector->Connect(server->activeViewSelectionModel(), vtkCommand::CurrentChangedEvent,
      this, SLOT(viewSelectionChanged()));
    this->VTKConnector->Connect(server->activeViewSelectionModel(),
      vtkCommand::SelectionChangedEvent, this, SLOT(viewSelectionChanged()));
  }

  this->sourceSelectionChanged();
  this->viewSelectionChanged();

  blocker.unblock();
  this->triggerSignals();
}

//-----------------------------------------------------------------------------
void pqActiveObjects::setActiveView(pqView* view)
{
  bool prev = this->blockSignals(true);

  // ensure that the appropriate server is active.
  if (view)
  {
    this->setActiveServer(view->getServer());
  }

  pqServer* server = this->activeServer();
  if (server && server->activeViewSelectionModel())
  {
    server->activeViewSelectionModel()->SetCurrentProxy(
      view ? view->getProxy() : nullptr, vtkSMProxySelectionModel::CLEAR_AND_SELECT);

    // this triggers a call to viewSelectionChanged(); but in collaboration mode
    // the ProxySelectionModel has been changed underneath therefore this
    // current proxy call do not trigger anything as it is already set with
    // the given value so we force our current class to update itself.
    this->viewSelectionChanged();
  }
  else
  {
    // if there's no-active server, it implies that setActiveView() must be
    // called with nullptr. In that case, we cannot really clear the selection
    // since we have no clue what's the active server. So nothing to do here.
  }

  this->blockSignals(prev);
  this->triggerSignals();
}

//-----------------------------------------------------------------------------
void pqActiveObjects::setActivePipelineProxy(pqProxy* proxy)
{
  bool prev = this->blockSignals(true);

  // ensure that the appropriate server is active.
  if (proxy)
  {
    this->setActiveServer(proxy->getServer());
  }

  pqServer* server = this->activeServer();
  if (server && server->activeSourcesSelectionModel())
  {
    server->activeSourcesSelectionModel()->SetCurrentProxy(
      proxy ? proxy->getProxy() : nullptr, vtkSMProxySelectionModel::CLEAR_AND_SELECT);
    // this triggers a call to sourceSelectionChanged();
  }
  else
  {
    // if there's no-active server, it implies that setActivePipelineProxy() must be
    // called with nullptr when no server was already active.
    // In that case, we cannot really clear the selection
    // since we have no clue what's the active server. So nothing to do here.
  }

  this->blockSignals(prev);
  this->triggerSignals();
}

//-----------------------------------------------------------------------------
void pqActiveObjects::setSelection(
  const pqProxySelection& selection_, pqServerManagerModelItem* current)
{
  pqProxy* current_proxy = qobject_cast<pqProxy*>(current);
  pqOutputPort* current_port = qobject_cast<pqOutputPort*>(current);
  pqServer* server = current_proxy ? current_proxy->getServer()
                                   : (current_port ? current_port->getServer() : nullptr);

  // ascertain that all items in the selection have the same server.
  foreach (pqServerManagerModelItem* item, selection_)
  {
    pqProxy* proxy = qobject_cast<pqProxy*>(item);
    pqOutputPort* port = qobject_cast<pqOutputPort*>(item);
    pqServer* cur_server =
      proxy ? proxy->getServer() : (port ? port->getServer() : qobject_cast<pqServer*>(item));
    if (cur_server != nullptr && cur_server != server)
    {
      if (server == nullptr)
      {
        server = cur_server;
      }
      else
      {
        qCritical("Selections with heterogeneous servers are not supported.");
        return;
      }
    }
  }

  bool prev = this->blockSignals(true);

  // ensure that the appropriate server is active.
  if (server)
  {
    this->setActiveServer(server);

    pqProxySelectionUtilities::copy(selection_, server->activeSourcesSelectionModel());
    // this triggers a call to selectionChanged() if selection actually changed.

    vtkSMProxy* proxy = current_proxy
      ? current_proxy->getProxy()
      : (current_port ? current_port->getOutputPortProxy() : nullptr);
    server->activeSourcesSelectionModel()->SetCurrentProxy(proxy, vtkSMProxySelectionModel::SELECT);
  }
  else
  {
    // if there's no-active server, it implies that setSelection() must be
    // called with nullptr. In that case, we cannot really clear the selection
    // since we have no clue what's the active server. So nothing to do here.
  }
  this->blockSignals(prev);
  this->triggerSignals();
}

//-----------------------------------------------------------------------------
void pqActiveObjects::updateRepresentation()
{
  pqOutputPort* port = this->activePort();
  if (port)
  {
    this->ActiveRepresentation = port->getRepresentation(this->activeView());
  }
  else
  {
    this->ActiveRepresentation = nullptr;
  }
  this->triggerSignals();
}

//-----------------------------------------------------------------------------
vtkSMSessionProxyManager* pqActiveObjects::proxyManager() const
{
  return this->activeServer() ? this->activeServer()->proxyManager() : nullptr;
}

//-----------------------------------------------------------------------------
vtkSMViewLayoutProxy* pqActiveObjects::activeLayout() const
{
  if (pqView* view = this->activeView())
  {
    if (auto l = vtkSMViewLayoutProxy::FindLayout(view->getViewProxy()))
    {
      return l;
    }
  }

  auto server = this->activeServer();
  if (!server)
  {
    // if not active server, then don't even attempt to deduce the active layout
    // using the pqTabbedMultiViewWidget since that doesn't matter, there's no
    // active server.
    return nullptr;
  }

  // if no active view is present, let's attempt to look for
  // pqTabbedMultiViewWidget and use its current tab if it is on the active
  // server.
  auto core = pqApplicationCore::instance();
  auto tmvwidget = qobject_cast<pqTabbedMultiViewWidget*>(core->manager("MULTIVIEW_WIDGET"));
  auto layoutProxy = tmvwidget ? tmvwidget->layoutProxy() : nullptr;
  return (layoutProxy && server->session() == layoutProxy->GetSession()) ? layoutProxy : nullptr;
}

//-----------------------------------------------------------------------------
int pqActiveObjects::activeLayoutLocation() const
{
  if (auto layout = this->activeLayout())
  {
    auto core = pqApplicationCore::instance();
    if (auto tmvwidget = qobject_cast<pqTabbedMultiViewWidget*>(core->manager("MULTIVIEW_WIDGET")))
    {
      if (auto mvwidget = tmvwidget->findTab(layout))
      {
        return std::max(mvwidget->activeFrameLocation(), 0);
      }
    }
  }
  return 0;
}

//-----------------------------------------------------------------------------
pqPipelineSource* pqActiveObjects::activeSource() const
{
  auto port = ::getPortFromPipelineProxy(this->ActivePipelineProxy);
  return port ? port->getSource() : nullptr;
}

//-----------------------------------------------------------------------------
pqOutputPort* pqActiveObjects::activePort() const
{
  return ::getPortFromPipelineProxy(this->ActivePipelineProxy);
}
