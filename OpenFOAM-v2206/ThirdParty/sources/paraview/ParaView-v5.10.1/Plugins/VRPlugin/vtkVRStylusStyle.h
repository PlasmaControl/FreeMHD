/*=========================================================================

   Program: ParaView
   Module:  vtkVRStylusStyle.h

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
/**
 * @class   vtkVRStylusStyle
 * @brief   an interaction style to control translation and rotation with a stylus
 *
 * vtkVRStylusStyle is an interaction style that uses the position of the
 * stylus in screen space (for example the stylus of the zSpace) to modify a 4x4 matrix.
 * Only the location of the stylus is used, the rotation has no effect.
 * Only works with RenderView proxy.
 */
#ifndef vtkVRStylusStyle_h
#define vtkVRStylusStyle_h

#include "vtkVRTrackStyle.h"
struct vtkVREvent;

class vtkVRStylusStyle : public vtkVRTrackStyle
{
public:
  static vtkVRStylusStyle* New();
  vtkTypeMacro(vtkVRStylusStyle, vtkVRTrackStyle);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  vtkVRStylusStyle();
  ~vtkVRStylusStyle() override = default;

  void HandleButton(const vtkVREvent& event) override;
  void HandleTracker(const vtkVREvent& event) override;

  bool EnableTranslate;
  bool EnableRotate;

private:
  vtkVRStylusStyle(const vtkVRStylusStyle&) = delete;
  void operator=(const vtkVRStylusStyle&) = delete;

  double LastRecordedPosition[3];
  bool PositionRecorded;
};

#endif // vtkVRStylusStyle_h
