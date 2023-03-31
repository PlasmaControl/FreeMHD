/*=========================================================================

   Program: ParaView
   Module:    pqFileDialogFavoriteModel.h

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

#ifndef _pqFileDialogFavoriteModel_h
#define _pqFileDialogFavoriteModel_h

#include "pqCoreModule.h"
#include <QAbstractListModel>
#include <QList>
#include <QObject>
#include <QPointer>

class vtkProcessModule;
class pqFileDialogModel;
class pqServer;
class QModelIndex;

/**
pqFileDialogFavoriteModel allows remote browsing of a connected ParaView server's
filesystem, as well as browsing of the local file system.

\sa pqFileDialog, pqFileDialogModel
*/
class PQCORE_EXPORT pqFileDialogFavoriteModel : public QAbstractListModel
{
  typedef QAbstractListModel Superclass;

  Q_OBJECT

public:
  /**
   * server is the server for which we need the listing.
   * if the server is nullptr, we get file listings from the builtin server
   */
  pqFileDialogFavoriteModel(pqFileDialogModel* model, pqServer* server, QObject* Parent);
  ~pqFileDialogFavoriteModel() override;

  /**
   * return the path to the favorites item
   */
  QString filePath(const QModelIndex&) const;
  /**
   * return whether this item is a directory
   */
  bool isDir(const QModelIndex&) const;

  /**
   * returns the data for an item
   */
  QVariant data(const QModelIndex& idx, int role) const override;

  /**
   * return the number of rows in the model
   */
  int rowCount(const QModelIndex& idx) const override;

  /**
   * used by the view to edit the label of the favorite
   */
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

  /**
   * return the flags for a specific item
   */
  Qt::ItemFlags flags(const QModelIndex& index) const override;

  /**
   * return header data
   */
  QVariant headerData(int section, Qt::Orientation, int role) const override;

  /**
   * Adds a directory to the favorites
   */
  virtual void addToFavorites(QString const& dirPath);

  /**
   * Removes a directory from the favorites
   */
  virtual void removeFromFavorites(QString const& dirPath);

  /**
   * Resets the favorites to the system default
   */
  virtual void resetFavoritesToDefault();

  /**
   * Flag to indicate if the ParaView Examples directory must be added when creating the settings
   * for the first time, or when reseting it to the default value.
   */
  static bool AddExamplesInFavorites;

protected:
  struct pqFileDialogFavoriteModelFileInfo
  {
    QString Label;
    QString FilePath;
    int Type;
  };

  void LoadFavoritesFromSystem();

  QPointer<pqFileDialogModel> FileDialogModel;
  pqServer* Server = nullptr;
  QList<pqFileDialogFavoriteModelFileInfo> FavoriteList;
  QString SettingsKey;
};

#endif // !_pqFileDialogFavoriteModel_h
