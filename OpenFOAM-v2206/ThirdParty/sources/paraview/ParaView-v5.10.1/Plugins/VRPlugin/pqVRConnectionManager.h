/*=========================================================================

  Program: ParaView
  Module:  vtkVRConnectionManager.h

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

=========================================================================*/
#ifndef pqVRConnectionManager_h
#define pqVRConnectionManager_h
#include "vtkPVVRConfig.h"

#include <QObject>
#include <QPointer>
class vtkVRQueue;
class vtkPVXMLElement;
class vtkSMProxyLocator;
#if PARAVIEW_PLUGIN_VRPlugin_USE_VRUI
class pqVRUIConnection;
#endif
#if PARAVIEW_PLUGIN_VRPlugin_USE_VRPN
class pqVRPNConnection;
#endif

class pqVRConnectionManager : public QObject
{
  Q_OBJECT
  typedef QObject Superclass;

public:
  pqVRConnectionManager(vtkVRQueue* queue, QObject* parent = 0);
  virtual ~pqVRConnectionManager();
#if PARAVIEW_PLUGIN_VRPlugin_USE_VRPN
  void add(pqVRPNConnection* conn);
  void remove(pqVRPNConnection* conn);
  pqVRPNConnection* GetVRPNConnection(const QString& name);
#endif
#if PARAVIEW_PLUGIN_VRPlugin_USE_VRUI
  void add(pqVRUIConnection* conn);
  void remove(pqVRUIConnection* conn);
  pqVRUIConnection* GetVRUIConnection(const QString& name);
#endif
  void clear();

  QList<QString> connectionNames() const;

  int numConnections() const;

  static pqVRConnectionManager* instance();

public Q_SLOTS:
  /// start/stop connections
  void start();
  void stop();

  /// Clears current connections and loads a new set of connections from the XML
  /// Configuration
  void configureConnections(vtkPVXMLElement* xml, vtkSMProxyLocator* locator);

  // save the connection configuration
  void saveConnectionsConfiguration(vtkPVXMLElement* root);

Q_SIGNALS:
  void connectionsChanged();

private:
  Q_DISABLE_COPY(pqVRConnectionManager)
  class pqInternals;
  pqInternals* Internals;

  friend class pqVRStarter;
  static void setInstance(pqVRConnectionManager*);
  static QPointer<pqVRConnectionManager> Instance;
};

#endif // pqVRConnectionManager_h
