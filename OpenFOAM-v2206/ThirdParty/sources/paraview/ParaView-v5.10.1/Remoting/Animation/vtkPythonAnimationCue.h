/*=========================================================================

  Program:   ParaView
  Module:    $RCSfile$

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkPythonAnimationCue
 *
 * vtkPythonAnimationCue is an animation cue that can execute arbitrary Python
 * scripts.
 */

#ifndef vtkPythonAnimationCue_h
#define vtkPythonAnimationCue_h

#include "vtkAnimationCue.h"
#include "vtkRemotingAnimationModule.h" //needed for exports

#include <memory> //for std::unique_ptr
#include <string> // for std::string

class VTKREMOTINGANIMATION_EXPORT vtkPythonAnimationCue : public vtkAnimationCue
{
public:
  static vtkPythonAnimationCue* New();
  vtkTypeMacro(vtkPythonAnimationCue, vtkAnimationCue);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Enable/Disable this cue.
   */
  vtkSetMacro(Enabled, bool);
  vtkGetMacro(Enabled, bool);
  vtkBooleanMacro(Enabled, bool);
  //@}

  //@{
  /**
   * Get/Set the python script to execute. The script must have the following
   * functions:
   * \li start_cue(cue): (optional) if present, called when the cue starts.
   * \li tick(cue) : (required) called on every tick.
   * \li end_cue(cue): (optional) if present, called when the cue ends.
   */
  void SetScript(const std::string&);
  std::string GetScript() const;
  //@}

protected:
  vtkPythonAnimationCue();
  ~vtkPythonAnimationCue() override;

  //@{
  /**
   * Callbacks that forward the call to corresponding Python function.
   */
  virtual void HandleStartCueEvent();
  virtual void HandleTickEvent();
  virtual void HandleEndCueEvent();
  //@}

  /**
   * Cleans the interpretor.
   */
  void DeleteInterpretor();

  bool Enabled;
  std::string Script;

private:
  vtkPythonAnimationCue(const vtkPythonAnimationCue&) = delete;
  void operator=(const vtkPythonAnimationCue&) = delete;

  struct pqInternals;
  std::unique_ptr<pqInternals> Internals;
};

#endif
