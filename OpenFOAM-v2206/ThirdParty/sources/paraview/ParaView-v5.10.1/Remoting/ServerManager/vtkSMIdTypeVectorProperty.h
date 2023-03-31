/*=========================================================================

  Program:   ParaView
  Module:    vtkSMIdTypeVectorProperty.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkSMIdTypeVectorProperty
 * @brief   property representing a vector of integers
 *
 * vtkSMIdTypeVectorProperty is a concrete sub-class of vtkSMVectorProperty
 * representing a vector of integers (vtkIdType, possibly 64 bit).
 * @sa
 * vtkSMVectorProperty vtkSMDoubleVectorProperty vtkSMStringVectorProperty
 */

#ifndef vtkSMIdTypeVectorProperty_h
#define vtkSMIdTypeVectorProperty_h

#include "vtkRemotingServerManagerModule.h" //needed for exports
#include "vtkSMVectorProperty.h"

#include <vector> // for std::vector

class vtkSMStateLocator;

class VTKREMOTINGSERVERMANAGER_EXPORT vtkSMIdTypeVectorProperty : public vtkSMVectorProperty
{
public:
  static vtkSMIdTypeVectorProperty* New();
  vtkTypeMacro(vtkSMIdTypeVectorProperty, vtkSMVectorProperty);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Returns the size of the vector.
   */
  unsigned int GetNumberOfElements() override;

  /**
   * Sets the size of the vector. If num is larger than the current
   * number of elements, this may cause reallocation and copying.
   */
  void SetNumberOfElements(unsigned int num) override;

  /**
   * Set the value of 1 element. The vector is resized as necessary.
   * Returns 0 if Set fails either because the property is read only
   * or the value is not in all domains. Returns 1 otherwise.
   */
  int SetElement(unsigned int idx, vtkIdType value);

  //@{
  /**
   * Set the values of all elements. The size of the values array
   * has to be equal or larger to the size of the vector.
   * Returns 0 if Set fails either because the property is read only
   * or one or more of the values is not in all domains.
   * Returns 1 otherwise.
   */
  int SetElements(const vtkIdType* values);
  int SetElements(const vtkIdType* values, unsigned int numValues);
  //@}

  //@{
  /**
   * Sets the values of all the unchecked elements.
   */
  int SetUncheckedElements(const vtkIdType* values);
  int SetUncheckedElements(const vtkIdType* values, unsigned int numValues);
  //@}

  /**
   * Set the value of 1st element. The vector is resized as necessary.
   * Returns 0 if Set fails either because the property is read only
   * or one or more of the values is not in all domains.
   * Returns 1 otherwise.
   */
  int SetElements1(vtkIdType value0);

  /**
   * Set the values of the first 2 elements. The vector is resized as necessary.
   * Returns 0 if Set fails either because the property is read only
   * or one or more of the values is not in all domains.
   * Returns 1 otherwise.
   */
  int SetElements2(vtkIdType value0, vtkIdType value1);

  /**
   * Set the values of the first 3 elements. The vector is resized as necessary.
   * Returns 0 if Set fails either because the property is read only
   * or one or more of the values is not in all domains.
   * Returns 1 otherwise.
   */
  int SetElements3(vtkIdType value0, vtkIdType value1, vtkIdType value2);

  /**
   * Append the values. The vector is resized as necessary.
   */
  int AppendElements(const vtkIdType* values, unsigned int numValues);

  /**
   * Append the values to the unchecked elements. The vector is resized as necessary.
   */
  int AppendUncheckedElements(const vtkIdType* values, unsigned int numValues);

  /**
   * Returns the value of 1 element.
   */
  vtkIdType GetElement(unsigned int idx);

  /**
   * Returns the value of 1 unchecked element. These are used by
   * domains. SetElement() first sets the value of 1 unchecked
   * element and then calls IsInDomain and updates the value of
   * the corresponding element only if IsInDomain passes.
   */
  vtkIdType GetUncheckedElement(unsigned int idx);

  /**
   * Set the value of 1 unchecked element. This can be used to
   * check if a value is in all domains of the property. Call
   * this and call IsInDomains().
   */
  void SetUncheckedElement(unsigned int idx, vtkIdType value);

  /**
   * Returns the size of unchecked elements. Usually this is
   * the same as the number of elements but can be different
   * before a domain check is performed.
   */
  unsigned int GetNumberOfUncheckedElements() override;

  ///@{
  /**
   * Another API to access elements.
   */
  const std::vector<vtkIdType>& GetElements() const;
  const std::vector<vtkIdType>& GetUncheckedElements() const;
  ///@}

  //@{
  /**
   * If ArgumentIsArray is true, multiple elements are passed in as
   * array arguments. For example, For example, if
   * RepeatCommand is true, NumberOfElementsPerCommand is 2, the
   * command is SetFoo and the values are 1 2 3 4 5 6, the resulting
   * stream will have:
   * @verbatim
   * * Invoke obj SetFoo array(1, 2)
   * * Invoke obj SetFoo array(3, 4)
   * * Invoke obj SetFoo array(5, 6)
   * @endverbatim
   */
  vtkGetMacro(ArgumentIsArray, int);
  vtkSetMacro(ArgumentIsArray, int);
  vtkBooleanMacro(ArgumentIsArray, int);
  //@}

  /**
   * Copy all property values.
   */
  void Copy(vtkSMProperty* src) override;

  void ClearUncheckedElements() override;

  /**
   * For properties that support specifying defaults in XML configuration, this
   * method will reset the property value to the default values specified in the
   * XML.
   */
  void ResetToXMLDefaults() override;

protected:
  vtkSMIdTypeVectorProperty();
  ~vtkSMIdTypeVectorProperty() override;

  /**
   * Let the property write its content into the stream
   */
  void WriteTo(vtkSMMessage*) override;

  /**
   * Let the property read and set its content from the stream
   */
  void ReadFrom(const vtkSMMessage*, int msg_offset, vtkSMProxyLocator*) override;

  int ReadXMLAttributes(vtkSMProxy* parent, vtkPVXMLElement* element) override;

  int ArgumentIsArray;

  /**
   * Sets the size of unchecked elements. Usually this is
   * the same as the number of elements but can be different
   * before a domain check is performed.
   */
  void SetNumberOfUncheckedElements(unsigned int num) override;

  /**
   * Load the XML state.
   */
  int LoadState(vtkPVXMLElement* element, vtkSMProxyLocator* loader) override;

  // Save concrete property values into the XML state property declaration
  void SaveStateValues(vtkPVXMLElement* propElement) override;

private:
  vtkSMIdTypeVectorProperty(const vtkSMIdTypeVectorProperty&) = delete;
  void operator=(const vtkSMIdTypeVectorProperty&) = delete;

  class vtkInternals;
  vtkInternals* Internals;
};

#endif
