/*=========================================================================

   Program: ParaView
   Module:    pqColorOverlay.h

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

#ifndef pq_ColorOverlay_h
#define pq_ColorOverlay_h

#include "pqComponentsModule.h"
#include <QWidget>

/**
 * pqColorOverlay defines a widget with a uniform color and an opacity.
 * The color and the opacity are exposed as properties so they can
 *  be animated with a QPropertyAnimation
 */
class PQCOMPONENTS_EXPORT pqColorOverlay : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(QColor rgb READ rgb WRITE setRgb)
  Q_PROPERTY(int opacity READ opacity WRITE setOpacity)

public:
  pqColorOverlay(QWidget* parent = nullptr);

  ///@{
  /**
   * Get/Set the red, green and blue values of the overlay
   * Defaults to white (255, 255, 255)
   */
  QColor rgb() const;
  void setRgb(int r, int g, int b);
  void setRgb(QColor Rgb);
  ///@}

  ///@{
  /**
   * Get/Set the opacity of the overlay
   * Default to 255
   */
  int opacity() const;
  void setOpacity(int opacity);
  ///@}

protected:
  void paintEvent(QPaintEvent*) override;

private:
  QColor Rgba = Qt::white;
};

#endif
