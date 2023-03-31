/*=========================================================================

   Program: ParaView
   Module:    pqPythonTextArea.h

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

#ifndef pqPythonTextArea_h
#define pqPythonTextArea_h

#include "pqPythonModule.h"

#include "pqPythonEditorActions.h"
#include "pqPythonUndoCommand.h"
#include "pqPythonUtils.h"
#include "pqTextLinkerPython.h"

#include <QKeyEvent>
#include <QUndoStack>
#include <QWidget>

class pqPythonSyntaxHighlighter;
class pqPythonFileIO;
class pqPythonLineNumberArea;
class QTextEdit;

/**
 * @class pqPythonTextArea
 * @brief A python text editor widget
 * @details Displays an editable text area with syntax
 * python highlighting and line numbering.
 */
class PQPYTHON_EXPORT pqPythonTextArea : public QWidget
{
  Q_OBJECT

public:
  /**
   * @brief Construct a pqPythonTextArea
   * @input[parent] the parent widget for the Qt ownership
   */
  explicit pqPythonTextArea(QWidget* parent);

  /**
   * @brief Returns the underlying \ref TextEdit
   */
  const QTextEdit* getTextEdit() const { return this->TextEdit; }

  /**
   * @brief Returns the underlying \ref TextEdit
   */
  QTextEdit* getTextEdit() { return this->TextEdit; }

  const QUndoStack& getUndoStack() const { return this->UndoStack; }

  /**
   * @brief Returns true if the file has been saved
   */
  bool saveOnClose();

  /**
   * @brief Opens a file in the editor.
   * Triggers a saving wizard if the current buffer
   * has not beed saved on the disk.
   */
  bool openFile(const QString& filename);

  /**
   * @brief Sets the default save directory.
   * Only used for the directory displayed
   * when the save popup window is shown.
   */
  void setDefaultSaveDirectory(const QString& dir);

  /**
   * @brief Connects this widget to the set of actions
   */
  void connectActions(pqPythonEditorActions& actions);

  /**
   * @brief Disconnect this widget from the set of actions
   */
  void disconnectActions(pqPythonEditorActions& actions);

  /**
   * @brief Get filename associated with this buffer.
   * Returns an empty string if no file is associated.
   */
  const QString& getFilename() const;

  /**
   * @brief Returns true if the buffer is empty
   */
  bool isEmpty() const;

  /**
   * @brief Returns true if the buffer has been modified
   * but not saved
   */
  bool isDirty() const;

  /**
   * @brief Override the current buffer content
   * with the referenced string
   */
  void setText(const QString& text);

  /**
   * @brief Link the current text area
   * to an arbitrary QTextEdit like
   * object
   */
  template <typename T>
  void linkTo(T* obj)
  {
    static_assert(sizeof(T) == 0, "Only specializations of linkTo(T* t) can be used");
  }

  /**
   * @brief Removes the linked QTextEdit like object
   */
  void unlink();

  /**
   * @brief Returns true if the parameter is linked
   * to this text area
   */
  bool isLinkedTo(const QObject* obj) const { return this->TextLinker.isLinkedTo(obj); }

  /**
   * @brief Returns true if this text area is linked
   */
  bool isLinked() const { return this->TextLinker.isLinked(); }

  /**
   * @brief Returns the linked text name. Returns an empty string
   * if nothing is linked
   */
  QString getLinkedName() const { return this->TextLinker.getSecondObjectName(); }

signals:
  /**
   * @brief Triggered when the current text
   * buffer is erased
   */
  void bufferErased();

  /**
   * @brief Triggers after a file has been
   * successfuly opened in the current buffer.
   */
  void fileOpened(const QString&);

  /**
   * @brief Triggers after a successful
   * saves of the current buffer.
   */
  void fileSaved(const QString&);

  /**
   * @brief Emitted when the buffer content
   * has been modified.
   */
  void contentChanged();

protected:
  /**
   * @brief Filter the KeyEvent CTRL+Z
   * @details We need to filter the QTextEdit KeyEvent
   * otherwise our undo/redo actions are not triggered.
   */
  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  void setupActions();

  void setupMenus();

  /**
   * @brief The editable text area
   */
  QPointer<QTextEdit> TextEdit;

  /**
   * @brief The line number area widget
   */
  QPointer<pqPythonLineNumberArea> LineNumberArea;

  /**
   * @brief The syntax highlighter used to color the \ref TextEdit
   */
  QPointer<pqPythonSyntaxHighlighter> SyntaxHighlighter;

  /**
   * @brief The IO module used to save the \ref TextEdit
   */
  QPointer<pqPythonFileIO> FileIO;

  pqTextLinker TextLinker;

  /**
   * @brief The text QUndoStack
   */
  QUndoStack UndoStack;

  /**
   * @brief The last history entry in the undo stack
   * @details Unfortunatly, QTextEdit doesn't keep
   * a text hystory, which we need for the \ref UndoStack.
   */
  pqPythonTextHistoryEntry lastEntry;
};

#include "pqPythonTextArea.txx"

#endif // pqPythonTextArea_h
