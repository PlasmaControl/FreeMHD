/*=========================================================================

   Program: ParaView
   Module:    pqImageTip.h

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

#ifndef _pqImageTip_h
#define _pqImageTip_h

#include "pqComponentsModule.h"
#include <QLabel>

class QBasicTimer;
class QPixmap;
class QPoint;

/**
 * Provides tooltip-like behavior, but displays an image instead of text
 */
class PQCOMPONENTS_EXPORT pqImageTip : public QLabel
{
  Q_OBJECT

public:
  /**
   * Displays a pixmap at the given screen coordinates
   */
  static void showTip(const QPixmap& image, const QPoint& pos);

private:
  pqImageTip(const QPixmap& image, QWidget* parent);
  ~pqImageTip() override;

  QBasicTimer* const hideTimer;

  bool eventFilter(QObject*, QEvent*) override;
  void enterEvent(QEvent*) override;
  void timerEvent(QTimerEvent* e) override;
  void paintEvent(QPaintEvent* e) override;
};

#endif
