/*=========================================================================

   Program: ParaView
   Module:    pqObjectBuilder.cxx

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

========================================================================*/
#include "pqObjectBuilder.h"

#include "vtkDebugLeaksManager.h"
#include "vtkNew.h"
#include "vtkPVXMLElement.h"
#include "vtkProcessModule.h"
#include "vtkSMCoreUtilities.h"
#include "vtkSMDomain.h"
#include "vtkSMDomainIterator.h"
#include "vtkSMInputProperty.h"
#include "vtkSMParaViewPipelineControllerWithRendering.h"
#include "vtkSMPropertyHelper.h"
#include "vtkSMPropertyIterator.h"
#include "vtkSMRenderViewProxy.h"
#include "vtkSMRepresentationProxy.h"
#include "vtkSMSession.h"
#include "vtkSMSessionProxyManager.h"
#include "vtkSMSettings.h"
#include "vtkSMSourceProxy.h"
#include "vtkSMStringVectorProperty.h"
#include "vtkSMTransferFunctionManager.h"
#include "vtkSMViewLayoutProxy.h"
#include "vtkSmartPointer.h"

#include <QApplication>
#include <QFileInfo>
#include <QtDebug>

#include "pqAnimationCue.h"
#include "pqApplicationCore.h"
#include "pqDataRepresentation.h"
#include "pqInterfaceTracker.h"
#include "pqOutputPort.h"
#include "pqPipelineFilter.h"
#include "pqPipelineSource.h"
#include "pqProxyModifiedStateUndoElement.h"
#include "pqRenderView.h"
#include "pqSMAdaptor.h"
#include "pqScalarBarRepresentation.h"
#include "pqScalarsToColors.h"
#include "pqServer.h"
#include "pqServerManagerModel.h"
#include "pqUndoStack.h"
#include "pqView.h"
#include "vtkSMAnimationSceneProxy.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <cassert>
#include <chrono>

namespace pqObjectBuilderNS
{
static bool ContinueWaiting = true;
static bool processEvents()
{
  QApplication::processEvents();
  return ContinueWaiting;
}

//-----------------------------------------------------------------------------
bool preCreatePipelineProxy(vtkSMParaViewPipelineController* controller, vtkSMProxy* proxy)
{
  return controller->PreInitializeProxy(proxy);
}

//-----------------------------------------------------------------------------
pqPipelineSource* postCreatePipelineProxy(vtkSMParaViewPipelineController* controller,
  vtkSMProxy* proxy, pqServer* server, const QString& regName = QString())
{
  // since there are no properties to set, nothing to do here.
  controller->PostInitializeProxy(proxy);
  if (regName.isEmpty())
  {
    controller->RegisterPipelineProxy(proxy);
  }
  else
  {
    controller->RegisterPipelineProxy(proxy, regName.toUtf8().data());
  }

  pqPipelineSource* source =
    pqApplicationCore::instance()->getServerManagerModel()->findItem<pqPipelineSource*>(proxy);
  source->setModifiedState(pqProxy::UNINITIALIZED);

  // Manage Modified state in Undo/Redo only if not a collaborative server
  if (!server->session()->IsMultiClients())
  {
    pqProxyModifiedStateUndoElement* elem = pqProxyModifiedStateUndoElement::New();
    elem->SetSession(server->session());
    elem->MadeUninitialized(source);
    ADD_UNDO_ELEM(elem);
    elem->Delete();
  }

  // Update animation scene.
  vtkSMAnimationSceneProxy::UpdateAnimationUsingDataTimeSteps(
    controller->GetAnimationScene(server->session()));

  return source;
}
}

//-----------------------------------------------------------------------------
pqObjectBuilder::pqObjectBuilder(QObject* _parent /*=0*/)
  : QObject(_parent)
  , ForceWaitingForConnection(false)
  , WaitingForConnection(false)
{
}

//-----------------------------------------------------------------------------
pqObjectBuilder::~pqObjectBuilder() = default;

//-----------------------------------------------------------------------------
pqPipelineSource* pqObjectBuilder::createSource(
  const QString& sm_group, const QString& sm_name, pqServer* server)
{
  vtkNew<vtkSMParaViewPipelineController> controller;
  vtkSMSessionProxyManager* pxm = server->proxyManager();
  vtkSmartPointer<vtkSMProxy> proxy;
  proxy.TakeReference(pxm->NewProxy(sm_group.toUtf8().data(), sm_name.toUtf8().data()));
  if (!pqObjectBuilderNS::preCreatePipelineProxy(controller, proxy))
  {
    return nullptr;
  }

  pqPipelineSource* source = pqObjectBuilderNS::postCreatePipelineProxy(controller, proxy, server);
  Q_EMIT this->sourceCreated(source);
  Q_EMIT this->proxyCreated(source);
  return source;
}

//-----------------------------------------------------------------------------
pqPipelineSource* pqObjectBuilder::createFilter(const QString& sm_group, const QString& sm_name,
  QMap<QString, QList<pqOutputPort*>> namedInputs, pqServer* server)
{
  vtkNew<vtkSMParaViewPipelineController> controller;
  vtkSMSessionProxyManager* pxm = server->proxyManager();
  vtkSmartPointer<vtkSMProxy> proxy;
  proxy.TakeReference(pxm->NewProxy(sm_group.toUtf8().data(), sm_name.toUtf8().data()));
  if (!pqObjectBuilderNS::preCreatePipelineProxy(controller, proxy))
  {
    return nullptr;
  }

  // Now for every input port, connect the inputs.
  QMap<QString, QList<pqOutputPort*>>::iterator mapIter;
  for (mapIter = namedInputs.begin(); mapIter != namedInputs.end(); ++mapIter)
  {
    const QString& input_port_name = mapIter.key();
    QList<pqOutputPort*>& inputs = mapIter.value();

    vtkSMProperty* prop = proxy->GetProperty(input_port_name.toUtf8().data());
    if (!prop)
    {
      qCritical() << "Failed to locate input property " << input_port_name;
      continue;
    }

    vtkSMPropertyHelper helper(prop);
    foreach (pqOutputPort* opPort, inputs)
    {
      helper.Add(opPort->getSource()->getProxy(), opPort->getPortNumber());
    }
  }

  pqPipelineSource* filter = pqObjectBuilderNS::postCreatePipelineProxy(controller, proxy, server);
  Q_EMIT this->filterCreated(filter);
  Q_EMIT this->proxyCreated(filter);
  return filter;
}

//-----------------------------------------------------------------------------
pqPipelineSource* pqObjectBuilder::createFilter(
  const QString& group, const QString& name, pqPipelineSource* input, int output_port)
{
  QMap<QString, QList<pqOutputPort*>> namedInputs;
  QList<pqOutputPort*> inputs;
  inputs.push_back(input->getOutputPort(output_port));
  namedInputs["Input"] = inputs;

  return this->createFilter(group, name, namedInputs, input->getServer());
}

//-----------------------------------------------------------------------------
inline QString pqObjectBuilderGetPath(const QString& filename, bool use_dir)
{
  if (use_dir)
  {
    return QFileInfo(filename).path();
  }
  return filename;
}

//-----------------------------------------------------------------------------
pqPipelineSource* pqObjectBuilder::createReader(
  const QString& sm_group, const QString& sm_name, const QStringList& files, pqServer* server)
{
  if (files.empty())
  {
    return nullptr;
  }

  unsigned int numFiles = files.size();
  QString reg_name = QFileInfo(files[0]).fileName();

  if (numFiles > 1)
  {
    // Find the largest prefix that matches all filenames, and then append a '*'
    // to signify that it is a collection of files.  If they all start with
    // something different, just give up and add the '*' anyway.
    for (unsigned int i = 1; i < numFiles; i++)
    {
      QString nextFile = QFileInfo(files[i]).fileName();
      if (nextFile.startsWith(reg_name))
        continue;
      QString commonPrefix = reg_name;
      do
      {
        commonPrefix.chop(1);
      } while (!nextFile.startsWith(commonPrefix) && !commonPrefix.isEmpty());
      if (commonPrefix.isEmpty())
        break;
      reg_name = commonPrefix;
    }
    reg_name += '*';
  }

  vtkNew<vtkSMParaViewPipelineController> controller;
  vtkSMSessionProxyManager* pxm = server->proxyManager();
  vtkSmartPointer<vtkSMProxy> proxy;
  proxy.TakeReference(pxm->NewProxy(sm_group.toUtf8().data(), sm_name.toUtf8().data()));
  if (!pqObjectBuilderNS::preCreatePipelineProxy(controller, proxy))
  {
    return nullptr;
  }

  QString pname = this->getFileNamePropertyName(proxy);
  if (!pname.isEmpty())
  {
    vtkSMStringVectorProperty* prop =
      vtkSMStringVectorProperty::SafeDownCast(proxy->GetProperty(pname.toUtf8().data()));
    if (!prop)
    {
      return nullptr;
    }

    // If there's a hint on the property indicating that this property expects a
    // directory name, then, we will set the directory name on it.
    bool use_dir = false;
    if (prop->GetHints() && prop->GetHints()->FindNestedElementByName("UseDirectoryName"))
    {
      use_dir = true;
    }

    if (numFiles == 1 || !prop->GetRepeatCommand())
    {
      pqSMAdaptor::setElementProperty(prop, pqObjectBuilderGetPath(files[0], use_dir));
    }
    else
    {
      QList<QVariant> values;
      foreach (QString file, files)
      {
        values.push_back(pqObjectBuilderGetPath(file, use_dir));
      }
      pqSMAdaptor::setMultipleElementProperty(prop, values);
    }
    proxy->UpdateVTKObjects();
  }

  pqPipelineSource* reader =
    pqObjectBuilderNS::postCreatePipelineProxy(controller, proxy, server, reg_name);
  Q_EMIT this->readerCreated(reader, files[0]);
  Q_EMIT this->readerCreated(reader, files);
  Q_EMIT this->sourceCreated(reader);
  Q_EMIT this->proxyCreated(reader);
  return reader;
}
//-----------------------------------------------------------------------------
void pqObjectBuilder::destroy(pqPipelineSource* source)
{
  if (!source)
  {
    qDebug() << "Cannot remove null source.";
    return;
  }

  if (!source->getAllConsumers().empty())
  {
    qDebug() << "Cannot remove source with consumers.";
    return;
  }

  Q_EMIT this->destroying(source);

  vtkNew<vtkSMParaViewPipelineController> controller;
  controller->UnRegisterProxy(source->getProxy());
}

//-----------------------------------------------------------------------------
pqView* pqObjectBuilder::createView(const QString& type, pqServer* server)
{
  if (!server)
  {
    qDebug() << "Cannot create view without server.";
    return nullptr;
  }

  vtkSMSessionProxyManager* pxm = server->proxyManager();
  vtkSmartPointer<vtkSMProxy> proxy;
  proxy.TakeReference(pxm->NewProxy("views", type.toUtf8().data()));
  if (!proxy)
  {
    qDebug() << "Failed to create a proxy for the requested view type:" << type;
    return nullptr;
  }

  // notify the world that we may create a new view. applications may handle
  // this by setting up layouts, etc.
  Q_EMIT this->aboutToCreateView(server);

  vtkNew<vtkSMParaViewPipelineController> controller;
  controller->PreInitializeProxy(proxy);
  controller->PostInitializeProxy(proxy);
  controller->RegisterViewProxy(proxy);

  pqServerManagerModel* model = pqApplicationCore::instance()->getServerManagerModel();
  pqView* view = model->findItem<pqView*>(proxy);
  if (view)
  {
    Q_EMIT this->viewCreated(view);
    Q_EMIT this->proxyCreated(view);
  }
  else
  {
    qDebug() << "Cannot locate the pqView for the view proxy of type" << type;
  }
  return view;
}

//-----------------------------------------------------------------------------
#if !defined(VTK_LEGACY_REMOVE)
pqView* pqObjectBuilder::createView(const QString& type, pqServer* server, bool)
{
  return this->createView(type, server);
}
#endif

//-----------------------------------------------------------------------------
void pqObjectBuilder::destroy(pqView* view)
{
  if (!view)
  {
    return;
  }

  Q_EMIT this->destroying(view);
  vtkNew<vtkSMParaViewPipelineController> controller;
  controller->UnRegisterProxy(view->getProxy());
}

//-----------------------------------------------------------------------------
void pqObjectBuilder::addToLayout(pqView* view, pqProxy* layout)
{
  if (view)
  {
    vtkNew<vtkSMParaViewPipelineControllerWithRendering> controller;
    controller->AssignViewToLayout(view->getViewProxy(),
      vtkSMViewLayoutProxy::SafeDownCast(layout ? layout->getProxy() : nullptr));
  }
}

//-----------------------------------------------------------------------------
pqDataRepresentation* pqObjectBuilder::createDataRepresentation(
  pqOutputPort* opPort, pqView* view, const QString& representationType)
{
  if (!opPort || !view)
  {
    qCritical() << "Missing required attribute.";
    return nullptr;
  }

  if (!view->canDisplay(opPort))
  {
    // View cannot display this source, nothing to do here.
    return nullptr;
  }

  vtkSmartPointer<vtkSMProxy> reprProxy;

  pqPipelineSource* source = opPort->getSource();
  vtkSMSessionProxyManager* pxm = source->proxyManager();

  // HACK to create correct representation for text sources/filters.
  QString srcProxyName = source->getProxy()->GetXMLName();
  if (representationType != "")
  {
    reprProxy.TakeReference(pxm->NewProxy("representations", representationType.toUtf8().data()));
  }
  else
  {
    reprProxy.TakeReference(view->getViewProxy()->CreateDefaultRepresentation(
      source->getProxy(), opPort->getPortNumber()));
  }

  // Could not determine representation proxy to create.
  if (!reprProxy)
  {
    return nullptr;
  }

  vtkNew<vtkSMParaViewPipelineController> controller;
  controller->PreInitializeProxy(reprProxy);

  // Set the reprProxy's input.
  pqSMAdaptor::setInputProperty(
    reprProxy->GetProperty("Input"), source->getProxy(), opPort->getPortNumber());
  controller->PostInitializeProxy(reprProxy);
  controller->RegisterRepresentationProxy(reprProxy);

  // Add the reprProxy to render module.
  vtkSMProxy* viewModuleProxy = view->getProxy();
  vtkSMPropertyHelper(viewModuleProxy, "Representations").Add(reprProxy);
  viewModuleProxy->UpdateVTKObjects();

  pqApplicationCore* core = pqApplicationCore::instance();
  pqDataRepresentation* repr =
    core->getServerManagerModel()->findItem<pqDataRepresentation*>(reprProxy);
  if (repr)
  {
    Q_EMIT this->dataRepresentationCreated(repr);
    Q_EMIT this->proxyCreated(repr);
  }
  return repr;
}

//-----------------------------------------------------------------------------
vtkSMProxy* pqObjectBuilder::createProxy(
  const QString& sm_group, const QString& sm_name, pqServer* server, const QString& reg_group)
{
  if (!server)
  {
    qDebug() << "server cannot be null";
    return nullptr;
  }
  if (sm_group.isEmpty() || sm_name.isEmpty())
  {
    qCritical() << "Group name and proxy name must be non empty.";
    return nullptr;
  }

  vtkSMSessionProxyManager* pxm = server->proxyManager();
  vtkSmartPointer<vtkSMProxy> proxy;
  proxy.TakeReference(pxm->NewProxy(sm_group.toUtf8().data(), sm_name.toUtf8().data()));
  if (!proxy.GetPointer())
  {
    qCritical() << "Failed to create proxy: " << sm_group << ", " << sm_name;
    return nullptr;
  }
  else if (reg_group.contains("prototypes"))
  {
    // Mark as prototype to prevent them from being saved in undo stack and
    // managed through the state
    proxy->SetPrototype(true);
  }

  pxm->RegisterProxy(reg_group.toUtf8().data(), proxy);
  return proxy;
}

//-----------------------------------------------------------------------------
void pqObjectBuilder::destroy(pqRepresentation* repr)
{
  if (!repr)
  {
    return;
  }

  Q_EMIT this->destroying(repr);

  // Remove repr from the view module.
  pqView* view = repr->getView();
  if (view)
  {
    vtkSMProxyProperty* pp =
      vtkSMProxyProperty::SafeDownCast(view->getProxy()->GetProperty("Representations"));
    pp->RemoveProxy(repr->getProxy());
    view->getProxy()->UpdateVTKObjects();
  }

  // If this repr has a lookuptable, we hide all scalar bars for that
  // lookup table unless there is some other repr that's using it.
  pqScalarsToColors* stc = nullptr;
  if (pqDataRepresentation* dataRepr = qobject_cast<pqDataRepresentation*>(repr))
  {
    stc = dataRepr->getLookupTable();
  }

  this->destroyProxyInternal(repr);

  if (stc)
  {
    // this hides scalar bars only if the LUT is not used by
    // any other repr. This must happen after the repr has
    // been deleted.
    stc->hideUnusedScalarBars();
  }
}

//-----------------------------------------------------------------------------
void pqObjectBuilder::destroy(pqProxy* proxy)
{
  Q_EMIT this->destroying(proxy);

  this->destroyProxyInternal(proxy);
}

//-----------------------------------------------------------------------------
void pqObjectBuilder::destroySources(pqServer* server)
{
  pqServerManagerModel* model = pqApplicationCore::instance()->getServerManagerModel();
  pqObjectBuilder* builder = pqApplicationCore::instance()->getObjectBuilder();

  QList<pqPipelineSource*> sources = model->findItems<pqPipelineSource*>(server);
  while (!sources.isEmpty())
  {
    for (int i = 0; i < sources.size(); i++)
    {
      if (sources[i]->getAllConsumers().empty())
      {
        builder->destroy(sources[i]);
        sources[i] = nullptr;
      }
    }
    sources.removeAll(nullptr);
  }
}

//-----------------------------------------------------------------------------
void pqObjectBuilder::destroyLookupTables(pqServer* server)
{
  pqServerManagerModel* model = pqApplicationCore::instance()->getServerManagerModel();
  pqObjectBuilder* builder = pqApplicationCore::instance()->getObjectBuilder();

  QList<pqScalarsToColors*> luts = model->findItems<pqScalarsToColors*>(server);
  foreach (pqScalarsToColors* lut, luts)
  {
    builder->destroy(lut);
  }

  QList<pqScalarBarRepresentation*> scalarbars =
    model->findItems<pqScalarBarRepresentation*>(server);
  foreach (pqScalarBarRepresentation* sb, scalarbars)
  {
    builder->destroy(sb);
  }
}

//-----------------------------------------------------------------------------
void pqObjectBuilder::destroyPipelineProxies(pqServer* server)
{
  this->destroySources(server);
  this->destroyLookupTables(server);
}

//-----------------------------------------------------------------------------
void pqObjectBuilder::destroyAllProxies(pqServer* server)
{
  if (!server)
  {
    qDebug() << "Server cannot be NULL.";
    return;
  }

  server->proxyManager()->UnRegisterProxies();
}

//-----------------------------------------------------------------------------
void pqObjectBuilder::destroyProxyInternal(pqProxy* proxy)
{
  if (proxy)
  {
    vtkSMSessionProxyManager* pxm = proxy->proxyManager();
    pxm->UnRegisterProxy(
      proxy->getSMGroup().toUtf8().data(), proxy->getSMName().toUtf8().data(), proxy->getProxy());
  }
}

//-----------------------------------------------------------------------------
QString pqObjectBuilder::getFileNamePropertyName(vtkSMProxy* proxy)
{
  const char* fname = vtkSMCoreUtilities::GetFileNameProperty(proxy);
  return fname ? QString(fname) : QString();
}

//-----------------------------------------------------------------------------
bool pqObjectBuilder::forceWaitingForConnection(bool force)
{
  std::swap(force, this->ForceWaitingForConnection);
  return force;
}

//-----------------------------------------------------------------------------
void pqObjectBuilder::abortPendingConnections()
{
  pqObjectBuilderNS::ContinueWaiting = false;
}

//-----------------------------------------------------------------------------
pqServer* pqObjectBuilder::createServer(const pqServerResource& resource, int connectionTimeout)
{
  if (this->WaitingForConnection)
  {
    qCritical() << "createServer called while waiting for previous connection "
                   "to be established.";
    return nullptr;
  }

  // Create a modified version of the resource that only contains server information
  const pqServerResource server_resource = resource.schemeHostsPorts();

  pqServerManagerModel* smModel = pqApplicationCore::instance()->getServerManagerModel();

  if (!vtkProcessModule::GetProcessModule()->GetMultipleSessionsSupport())
  {
    // If multiple connections are not supported, then we only connect to the
    // new server if no already connected and ensure that any previously
    // connected servers are disconnected.
    // determine if we're already connected to this server.
    pqServer* server = smModel->findServer(server_resource);
    if (server)
    {
      return server;
    }

    if (smModel->getNumberOfItems<pqServer*>() > 0)
    {
      this->removeServer(smModel->getItemAtIndex<pqServer*>(0));
    }
  }

  this->WaitingForConnection = true;

  // Let the pqServerManagerModel know the resource to use for the connection
  // to be created.
  smModel->setActiveResource(resource);

  // Based on the server resource, create the correct type of server ...
  vtkIdType id = 0;
  if (server_resource.scheme() == "builtin")
  {
    id = vtkSMSession::ConnectToSelf(connectionTimeout);
  }
  else if (server_resource.scheme() == "cs")
  {
    id = vtkSMSession::ConnectToRemote(
      resource.host().toUtf8().data(), resource.port(11111), connectionTimeout);
  }
  else if (server_resource.scheme() == "csrc")
  {
    pqObjectBuilderNS::ContinueWaiting = true;
    id = vtkSMSession::ReverseConnectToRemote(
      server_resource.port(11111), &pqObjectBuilderNS::processEvents);
  }
  else if (server_resource.scheme() == "cdsrs")
  {
    id = vtkSMSession::ConnectToRemote(server_resource.dataServerHost().toUtf8().data(),
      server_resource.dataServerPort(11111), server_resource.renderServerHost().toUtf8().data(),
      server_resource.renderServerPort(22221), connectionTimeout);
  }
  else if (server_resource.scheme() == "cdsrsrc")
  {
    pqObjectBuilderNS::ContinueWaiting = true;
    id = vtkSMSession::ReverseConnectToRemote(server_resource.dataServerPort(11111),
      server_resource.renderServerPort(22221), &pqObjectBuilderNS::processEvents);
  }
  else if (server_resource.scheme() == "catalyst")
  {
    id = vtkSMSession::ConnectToCatalyst();
  }
  else
  {
    qCritical() << "Unknown server type: " << server_resource.scheme() << "\n";
  }

  pqServer* server = nullptr;
  if (id != 0)
  {
    server = smModel->findServer(id);
    Q_EMIT this->finishedAddingServer(server);
  }
  this->WaitingForConnection = false;
  return server;
}

//-----------------------------------------------------------------------------
void pqObjectBuilder::removeServer(pqServer* server)
{
  if (!server)
  {
    qDebug() << "No server to remove.";
    return;
  }

  pqApplicationCore* core = pqApplicationCore::instance();
  pqServerManagerModel* sModel = core->getServerManagerModel();
  sModel->beginRemoveServer(server);
  vtkSMSession::Disconnect(server->sessionId());
  sModel->endRemoveServer();
}

//-----------------------------------------------------------------------------
pqServer* pqObjectBuilder::resetServer(pqServer* server)
{
  assert(server);

  // save the current remaining lifetime to restore it
  // when we recreate the server following reset.
  const int remainingLifetime = server->getRemainingLifeTime();
  auto startTime = std::chrono::system_clock::now();

  pqServerResource resource = server->getResource();
  vtkSmartPointer<vtkSMSession> session = server->session();
  vtkProcessModule* pm = vtkProcessModule::GetProcessModule();

  // simulate disconnect.
  pqApplicationCore* core = pqApplicationCore::instance();
  pqServerManagerModel* smModel = core->getServerManagerModel();
  smModel->beginRemoveServer(server);

  // Unregister all proxies.
  this->destroyAllProxies(server);

  // Unregister session.
  pm->UnRegisterSession(session);
  smModel->endRemoveServer();

  server = nullptr;

  // Let the pqServerManagerModel know the resource to use for the connection
  // to be created.
  smModel->setActiveResource(resource);

  // re-register session.
  vtkIdType id = pm->RegisterSession(session);

  pqServer* newServer = nullptr;
  if (id != 0)
  {
    newServer = smModel->findServer(id);

    auto endTime = std::chrono::system_clock::now();
    auto deltaMinutes = std::chrono::duration_cast<std::chrono::minutes>(endTime - startTime);
    newServer->setRemainingLifeTime(
      remainingLifetime > 0 ? (remainingLifetime - deltaMinutes.count()) : remainingLifetime);
    Q_EMIT this->finishedAddingServer(newServer);
  }
  return newServer;
}

//-----------------------------------------------------------------------------
void pqObjectBuilder::destroy(pqAnimationCue* cue)
{
  if (!cue)
  {
    return;
  }

  vtkSMSessionProxyManager* pxm = cue->proxyManager();

  QList<vtkSMProxy*> keyframes = cue->getKeyFrames();
  // unregister all the keyframes.
  foreach (vtkSMProxy* kf, keyframes)
  {
    pxm->UnRegisterProxy("animation", pxm->GetProxyName("animation", kf), kf);
  }
  this->destroy(static_cast<pqProxy*>(cue));
}
