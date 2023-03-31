/*=========================================================================

   Program: ParaView
   Module:  pqFileListPropertyWidget.cxx

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
#include "pqFileListPropertyWidget.h"
#include "ui_pqFileListPropertyWidget.h"

#include <QStringListModel>
#include <QtDebug>
#include <algorithm>

#include "pqApplicationCore.h"
#include "pqFileDialog.h"
#include "pqServerManagerModel.h"
#include "pqStringVectorPropertyWidget.h"
#include "vtkPVLogger.h"
#include "vtkSMProperty.h"
#include "vtkSMProxy.h"

class pqFileListPropertyWidget::pqInternals
{
public:
  Ui::FileListPropertyWidget Ui;
  QStringListModel Model;

  QString chooseFile(
    pqFileListPropertyWidget* self, const QString& defaultFileName = QString()) const
  {
    // there's not much to do with the `defaultFileName` currently.
    // pqFileDialog doesn't support specifying the existing file!
    Q_UNUSED(defaultFileName);

    auto proxy = self->proxy();
    auto property = self->property();
    auto session = proxy->GetSession();
    auto hints = property->GetHints();

    // process hints.
    bool directoryMode, anyFile, browseLocalFileSystem;
    QString filter;
    pqStringVectorPropertyWidget::processFileChooserHints(
      hints, directoryMode, anyFile, filter, browseLocalFileSystem);

    pqServerManagerModel* smModel = pqApplicationCore::instance()->getServerManagerModel();
    auto server = browseLocalFileSystem ? nullptr : smModel->findServer(session);

    pqFileDialog dialog(
      server, self, QString("Select %1").arg(property->GetXMLLabel()), QString(), filter);
    if (directoryMode)
    {
      dialog.setFileMode(pqFileDialog::Directory);
    }
    else if (anyFile)
    {
      dialog.setFileMode(pqFileDialog::AnyFile);
    }
    else
    {
      // note: we select 1 file at a time here.
      dialog.setFileMode(pqFileDialog::ExistingFile);
    }

    if (dialog.exec() == QDialog::Accepted)
    {
      auto filesNames = dialog.getSelectedFiles();
      return filesNames.isEmpty() ? QString() : filesNames[0];
    }

    return QString();
  }
};

//-----------------------------------------------------------------------------
pqFileListPropertyWidget::pqFileListPropertyWidget(
  vtkSMProxy* smproxy, vtkSMProperty* smproperty, QWidget* parentObject)
  : Superclass(smproxy, parentObject)
  , Internals(new pqFileListPropertyWidget::pqInternals())
{
  vtkVLogF(PARAVIEW_LOG_APPLICATION_VERBOSITY(), "using pqFileListPropertyWidget.");
  auto& internals = (*this->Internals);
  internals.Ui.setupUi(this);
  internals.Ui.label->setText(QString("<b>%1</b>").arg(smproperty->GetXMLLabel()));

  QObject::connect(&internals.Model, &QAbstractItemModel::dataChanged, this,
    &pqFileListPropertyWidget::fileNamesChanged);
  QObject::connect(&internals.Model, &QAbstractItemModel::modelReset, this,
    &pqFileListPropertyWidget::fileNamesChanged);
  QObject::connect(&internals.Model, &QAbstractItemModel::rowsInserted, this,
    &pqFileListPropertyWidget::fileNamesChanged);
  QObject::connect(&internals.Model, &QAbstractItemModel::rowsRemoved, this,
    &pqFileListPropertyWidget::fileNamesChanged);

  // disable "remove" and "browse" button if nothing is selected.
  internals.Ui.tableView->setModel(&internals.Model);
  auto selectionModel = internals.Ui.tableView->selectionModel();
  Q_ASSERT(selectionModel);
  QObject::connect(selectionModel, &QItemSelectionModel::selectionChanged,
    [selectionModel, &internals](const QItemSelection&, const QItemSelection&) {
      internals.Ui.remove->setEnabled(selectionModel->hasSelection());
      internals.Ui.browse->setEnabled(selectionModel->hasSelection());
    });

  QObject::connect(
    internals.Ui.add, &QAbstractButton::clicked, [&internals, this, selectionModel]() {
      const auto filename = internals.chooseFile(this);
      if (!filename.isEmpty())
      {
        auto list = internals.Model.stringList();
        list.push_back(filename);
        internals.Model.setStringList(list);
        internals.Ui.tableView->setCurrentIndex(internals.Model.index(list.size() - 1, 0));
      }
    });

  QObject::connect(
    internals.Ui.browse, &QAbstractButton::clicked, [selectionModel, &internals, this]() {
      auto index = selectionModel->currentIndex();
      if (!index.isValid())
      {
        return;
      }
      qDebug() << "index=" << index;
      auto filenames = internals.Model.stringList();
      const auto filename = internals.chooseFile(this, filenames.at(index.row()));
      if (!filename.isEmpty())
      {
        filenames[index.row()] = filename;
        internals.Model.setStringList(filenames);
        internals.Ui.tableView->setCurrentIndex(index);
      }
    });

  QObject::connect(internals.Ui.remove, &QAbstractButton::clicked, [selectionModel, &internals]() {
    std::vector<int> rows;
    for (const auto& index : selectionModel->selectedRows(0))
    {
      rows.push_back(index.row());
    }

    std::sort(rows.begin(), rows.end());
    std::reverse(rows.begin(), rows.end());

    auto list = internals.Model.stringList();
    for (auto& row : rows)
    {
      list.removeAt(row);
    }

    internals.Ui.tableView->clearSelection();
    internals.Model.setStringList(list);
    if (!list.isEmpty())
    {
      int currentRow = rows.front();
      while (list.size() <= currentRow)
      {
        --currentRow;
      }
      internals.Ui.tableView->setCurrentIndex(internals.Model.index(currentRow, 0));
    }
  });

  QObject::connect(internals.Ui.removeAll, &QAbstractButton::clicked, [&internals]() {
    internals.Ui.tableView->clearSelection();
    internals.Model.setStringList(QStringList());
  });

  this->setShowLabel(false);
  this->setChangeAvailableAsChangeFinished(true);
  this->addPropertyLink(this, "fileNames", SIGNAL(fileNamesChanged()), smproperty);
}

//-----------------------------------------------------------------------------
pqFileListPropertyWidget::~pqFileListPropertyWidget() = default;

//-----------------------------------------------------------------------------
const QStringList& pqFileListPropertyWidget::fileNames() const
{
  auto& internals = (*this->Internals);
  this->FileNamesCache = internals.Model.stringList();
  return this->FileNamesCache;
}

//-----------------------------------------------------------------------------
void pqFileListPropertyWidget::setFileNames(const QStringList& filenames)
{
  auto& internals = (*this->Internals);
  this->FileNamesCache = filenames;
  internals.Model.setStringList(this->FileNamesCache);
}
