/*=========================================================================

  Program:   ParaView
  Module:    vtkSMProperty.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkSMProperty.h"

#include "vtkClientServerStream.h"
#include "vtkClientServerStreamInstantiator.h"
#include "vtkCommand.h"
#include "vtkObjectFactory.h"
#include "vtkPVXMLElement.h"
#include "vtkProcessModule.h"
#include "vtkSMDocumentation.h"
#include "vtkSMDomain.h"
#include "vtkSMDomainIterator.h"
#include "vtkSMMessage.h"
#include "vtkSMProperty.h"
#include "vtkSMPropertyLink.h"
#include "vtkSMProxy.h"
#include "vtkSMProxyProperty.h"
#include "vtkSmartPointer.h"

#include <sstream>
#include <vector>

#include "vtkSMPropertyInternals.h"

vtkStandardNewMacro(vtkSMProperty);

vtkCxxSetObjectMacro(vtkSMProperty, InformationProperty, vtkSMProperty);
vtkCxxSetObjectMacro(vtkSMProperty, Documentation, vtkSMDocumentation);
vtkCxxSetObjectMacro(vtkSMProperty, Hints, vtkPVXMLElement);

//---------------------------------------------------------------------------
vtkSMProperty::vtkSMProperty()
{
  this->StateIgnored = false;
  this->Command = nullptr;
  this->ImmediateUpdate = 0;
  this->Animateable = 2; // By default Animateable in advanced mode only.
  this->PInternals = new vtkSMPropertyInternals;
  this->XMLName = nullptr;
  this->XMLLabel = nullptr;
  this->PanelVisibility = nullptr;
  this->PanelVisibilityDefaultForRepresentation = nullptr;
  this->PanelWidget = nullptr;
  this->DisableSubTrace = nullptr;
  this->DomainIterator = vtkSMDomainIterator::New();
  this->DomainIterator->SetProperty(this);
  this->Proxy = nullptr;
  this->InformationOnly = 0;
  this->InformationProperty = nullptr;
  this->IsInternal = 0;
  this->NoCustomDefault = 0;
  this->Documentation = nullptr;
  this->Repeatable = 0;
  this->IgnoreSynchronization = 0;
  this->Links = nullptr;

  this->Hints = nullptr;
  this->BlockModifiedEvents = false;
  this->PendingModifiedEvents = false;

  this->Proxy = nullptr;

  // Whenever the property fires UncheckedPropertyModifiedEvent, we update any
  // dependent domains.
  this->AddObserver(
    vtkCommand::UncheckedPropertyModifiedEvent, this, &vtkSMProperty::UpdateDomains);
}

//---------------------------------------------------------------------------
vtkSMProperty::~vtkSMProperty()
{
  this->RemoveFromSourceLink();
  this->SetCommand(nullptr);
  delete this->PInternals;
  this->SetXMLName(nullptr);
  this->SetXMLLabel(nullptr);
  this->DomainIterator->Delete();
  this->SetInformationProperty(nullptr);
  this->SetDocumentation(nullptr);
  this->SetHints(nullptr);
  this->SetParent(nullptr);
  this->SetPanelVisibility(nullptr);
  this->SetPanelVisibilityDefaultForRepresentation(nullptr);
  this->SetPanelWidget(nullptr);
  this->SetDisableSubTrace(nullptr);
  if (this->Links)
  {
    this->Links->Delete();
    this->Links = nullptr;
  }
}

//-----------------------------------------------------------------------------
// UnRegister is overloaded because the object has a reference to itself
// through the domain iterator.
void vtkSMProperty::UnRegister(vtkObjectBase* obj)
{
  if (this->ReferenceCount == 2)
  {
    this->Superclass::UnRegister(obj);

    vtkSMDomainIterator* tmp = this->DomainIterator;
    tmp->Register(nullptr);
    tmp->SetProperty(nullptr);
    tmp->UnRegister(nullptr);
    return;
  }
  this->Superclass::UnRegister(obj);
}

//---------------------------------------------------------------------------
int vtkSMProperty::IsInDomains()
{
  return this->IsInDomains(nullptr);
}

//---------------------------------------------------------------------------
int vtkSMProperty::IsInDomains(vtkSMDomain** ptr)
{
  this->DomainIterator->Begin();
  while (!this->DomainIterator->IsAtEnd())
  {
    if (this->DomainIterator->GetDomain()->IsInDomain(this) == vtkSMDomain::NOT_IN_DOMAIN)
    {
      if (ptr)
      {
        *ptr = this->DomainIterator->GetDomain();
      }
      return 0;
    }
    this->DomainIterator->Next();
  }
  return 1;
}

//---------------------------------------------------------------------------
void vtkSMProperty::AddDomain(const char* name, vtkSMDomain* domain)
{
  // Check if the proxy already exists. If it does, we will
  // replace it
  vtkSMPropertyInternals::DomainMap::iterator it = this->PInternals->Domains.find(name);

  if (it != this->PInternals->Domains.end())
  {
    vtkWarningMacro("Domain " << name << " already exists. Replacing");
    it->second->SetProperty(nullptr);
  }

  this->PInternals->Domains[name] = domain;
  if (domain)
  {
    domain->SetProperty(this); // doesn't change reference count.
    domain->AddObserver(
      vtkCommand::DomainModifiedEvent, this, &vtkSMProperty::InvokeDomainModifiedEvent);
  }
}

//---------------------------------------------------------------------------
void vtkSMProperty::AddLinkedProperty(vtkSMProperty* targetProperty)
{
  if (!targetProperty)
  {
    return;
  }

  // This sets up the target property to take on the value of this property
  // whenever it is updated.
  if (!this->Links)
  {
    this->Links = vtkSMPropertyLink::New();
    this->Links->AddLinkedProperty(this->GetParent(), this->GetXMLName(), vtkSMLink::INPUT);
  }

  this->Links->AddLinkedProperty(
    targetProperty->GetParent(), targetProperty->GetXMLName(), vtkSMLink::OUTPUT);
  targetProperty->PInternals->LinkSourceProperty = this;
}

//---------------------------------------------------------------------------
void vtkSMProperty::RemoveLinkedProperty(vtkSMProperty* targetProperty)
{
  if (!targetProperty || !this->Links)
  {
    return;
  }

  this->Links->RemoveLinkedProperty(targetProperty->GetParent(), targetProperty->GetXMLName());
}

//---------------------------------------------------------------------------
void vtkSMProperty::RemoveFromSourceLink()
{
  if (this->PInternals->LinkSourceProperty)
  {
    // Remove this instance as a subscriber to the source proxy
    this->PInternals->LinkSourceProperty->RemoveLinkedProperty(this);
    this->PInternals->LinkSourceProperty = nullptr;
  }
}

//---------------------------------------------------------------------------
void vtkSMProperty::InvokeDomainModifiedEvent()
{
  this->InvokeEvent(vtkCommand::DomainModifiedEvent);
}

//---------------------------------------------------------------------------
vtkSMDomain* vtkSMProperty::GetDomain(const char* name)
{
  vtkSMPropertyInternals::DomainMap::iterator it = this->PInternals->Domains.find(name);

  if (it == this->PInternals->Domains.end())
  {
    return nullptr;
  }

  return it->second.GetPointer();
}

//---------------------------------------------------------------------------
vtkSMDomain* vtkSMProperty::FindDomain(const char* classname)
{
  vtkSmartPointer<vtkSMDomainIterator> iter;
  iter.TakeReference(this->NewDomainIterator());
  for (iter->Begin(); !iter->IsAtEnd(); iter->Next())
  {
    if (iter->GetDomain()->IsA(classname))
    {
      return iter->GetDomain();
    }
  }
  return nullptr;
}

//---------------------------------------------------------------------------
unsigned int vtkSMProperty::GetNumberOfDomains()
{
  return static_cast<unsigned int>(this->PInternals->Domains.size());
}

//---------------------------------------------------------------------------
vtkSMDomainIterator* vtkSMProperty::NewDomainIterator()
{
  vtkSMDomainIterator* iter = vtkSMDomainIterator::New();
  iter->SetProperty(this);
  return iter;
}

//---------------------------------------------------------------------------
void vtkSMProperty::AddDependent(vtkSMDomain* dom)
{
  this->PInternals->Dependents.push_back(dom);
}

//---------------------------------------------------------------------------
void vtkSMProperty::RemoveAllDependents()
{
  vtkSMPropertyInternals::DependentsVector::iterator iter = this->PInternals->Dependents.begin();
  for (; iter != this->PInternals->Dependents.end(); iter++)
  {
    iter->GetPointer()->RemoveRequiredProperty(this);
  }
  this->PInternals->Dependents.erase(
    this->PInternals->Dependents.begin(), this->PInternals->Dependents.end());
}

//---------------------------------------------------------------------------
void vtkSMProperty::UpdateDomains()
{
  // This method gets called when properties on "prototype" proxies are changed
  // too. So far in ParaView, we've only used prototype proxies to check whether
  // input is acceptable or to determine list of available properties. For such
  // use-cases, updating domains can not only cause unnecessary slowdowns, but
  // also raise issues like #20617. So starting with 5.10, we're skipping
  // updating dependent domains on prototype properties.
  auto parent = this->GetParent();
  if (parent && parent->IsPrototype())
  {
    return;
  }

  // I genuinely doubt when a property changes, its domain should change!!
  //// Update own domains
  // this->DomainIterator->Begin();
  // while(!this->DomainIterator->IsAtEnd())
  //  {
  //  this->DomainIterator->GetDomain()->Update(0);
  //  this->DomainIterator->Next();
  //  }

  // Update other dependent domains
  vtkSMPropertyInternals::DependentsVector::iterator iter = this->PInternals->Dependents.begin();
  for (; iter != this->PInternals->Dependents.end(); iter++)
  {
    iter->GetPointer()->Update(this);
  }
}

//---------------------------------------------------------------------------
vtkSMProperty* vtkSMProperty::NewProperty(const char* name)
{
  if (!this->Proxy)
  {
    return nullptr;
  }
  return this->Proxy->NewProperty(name);
}

//---------------------------------------------------------------------------
void vtkSMProperty::CreateAndSetPrettyLabel(const char* xmlname)
{
  const char* label = vtkSMProperty::CreateNewPrettyLabel(xmlname);
  this->SetXMLLabel(label);
  delete[] label;
}

//---------------------------------------------------------------------------
const char* vtkSMProperty::CreateNewPrettyLabel(const char* xmlname)
{

  // Add space before every capital letter not preceded by a capital letter
  // or space hence:
  // "MySpace" ==> "My Space"
  // "MySPACE" ==> "My SPACE"
  // "My Space" ==> "My Space"

  int max = static_cast<int>(strlen(xmlname));
  char* label = new char[2 * max + 10];
  char* ptr = label;

  bool previous_capital = false;
  *ptr = xmlname[0];
  ptr++;

  for (int cc = 1; cc < max; ++cc)
  {
    if (xmlname[cc] >= 'A' && xmlname[cc] <= 'Z')
    {
      if (!previous_capital && *(ptr - 1) != ' ')
      {
        *ptr = ' ';
        ptr++;
      }
      previous_capital = true;
    }
    else
    {
      previous_capital = false;
    }
    *ptr = xmlname[cc];
    ptr++;
  }
  *ptr = 0;
  return label;
}

//---------------------------------------------------------------------------
int vtkSMProperty::ReadXMLAttributes(vtkSMProxy* proxy, vtkPVXMLElement* element)
{
  // TODO: some of the attributes are no longer necessary on the proxy-side,
  // eg. "Command". We will remove those once we've verified that they are
  // present on the PMProperty side.
  const char* xmlname = element->GetAttribute("name");
  if (xmlname)
  {
    this->SetXMLName(xmlname);
  }

  const char* xmllabel = element->GetAttribute("label");
  if (xmllabel)
  {
    this->SetXMLLabel(xmllabel);
  }
  else
  {
    this->CreateAndSetPrettyLabel(xmlname);
  }

  const char* command = element->GetAttribute("command");
  if (command)
  {
    this->SetCommand(command);
  }

  int repeatable;
  int retVal = 1;
  if (element->GetScalarAttribute("repeatable", &repeatable) ||
    element->GetScalarAttribute("repeat_command", &repeatable))
  {
    this->Repeatable = repeatable;
  }

  const char* information_property = element->GetAttribute("information_property");
  if (information_property)
  {
    this->SetInformationProperty(this->NewProperty(information_property));
  }

  int immediate_update;
  retVal = element->GetScalarAttribute("immediate_update", &immediate_update);
  if (retVal)
  {
    this->SetImmediateUpdate(immediate_update);
  }

  int state_ignored;
  retVal = element->GetScalarAttribute("state_ignored", &state_ignored);
  if (retVal)
  {
    this->SetStateIgnored(state_ignored != 0);
  }
  else
  {
    this->StateIgnoredOff(); // Default value
  }

  int information_only;
  retVal = element->GetScalarAttribute("information_only", &information_only);
  if (retVal)
  {
    this->SetInformationOnly(information_only);
  }
  int ignore_synch;
  retVal = element->GetScalarAttribute("ignore_synchronization", &ignore_synch);
  if (retVal)
  {
    this->SetIgnoreSynchronization(ignore_synch);
  }

  int animateable;
  retVal = element->GetScalarAttribute("animateable", &animateable);
  if (retVal)
  {
    this->SetAnimateable(animateable);
  }

  int is_internal;
  if (element->GetScalarAttribute("is_internal", &is_internal))
  {
    this->SetIsInternal(is_internal);
  }

  int no_custom_default;
  if (element->GetScalarAttribute("no_custom_default", &no_custom_default))
  {
    this->SetNoCustomDefault(no_custom_default);
  }

  const char* panel_visibility = element->GetAttribute("panel_visibility");
  if (panel_visibility)
  {
    this->SetPanelVisibility(panel_visibility);
  }
  else
  {
    this->SetPanelVisibility(this->GetInformationOnly() ? "never" : "default");
  }

  const char* panel_visibility_default_for_representation =
    element->GetAttribute("panel_visibility_default_for_representation");
  if (panel_visibility_default_for_representation)
  {
    this->SetPanelVisibilityDefaultForRepresentation(panel_visibility_default_for_representation);
  }

  const char* panel_widget = element->GetAttribute("panel_widget");
  if (panel_widget)
  {
    this->SetPanelWidget(panel_widget);
  }

  const char* disable_sub_trace = element->GetAttribute("disable_sub_trace");
  if (disable_sub_trace)
  {
    this->SetDisableSubTrace(disable_sub_trace);
  }

  // Manage deprecated XML definition
  int deprecated_attr_value;
  if (element->GetScalarAttribute("update_self", &deprecated_attr_value))
  {
    std::ostringstream proxyXML;
    element->GetParent()->PrintXML(proxyXML, vtkIndent(1));
    vtkWarningMacro(<< "Attribute update_self is not managed anymore."
                    << "It is deprecated. " << endl
                    << "Please FIX the decalaration of the following Proxy." << endl
                    << proxyXML.str().c_str());
  }

  // Read and create domains.
  for (unsigned int i = 0; i < element->GetNumberOfNestedElements(); ++i)
  {
    vtkPVXMLElement* domainEl = element->GetNestedElement(i);

    // These are not domain elements.
    if (strcmp(domainEl->GetName(), "Documentation") == 0)
    {
      vtkSMDocumentation* doc = vtkSMDocumentation::New();
      doc->SetDocumentationElement(domainEl);
      this->SetDocumentation(doc);
      doc->Delete();
      continue;
    }
    else if (strcmp(domainEl->GetName(), "Hints") == 0)
    {
      this->SetHints(domainEl);
      continue;
    }
    else if (std::string(domainEl->GetName()).find("InformationHelper") != std::string::npos)
    {
      // InformationHelper are used to extract information from VTK object
      // therefore they are not used on the proxy side (SM).
      continue;
    }
    else if (std::string(domainEl->GetName()).find("StringArrayHelper") != std::string::npos)
    {
      // InformationHelper are used to extract information from VTK object
      // therefore they are not used on the proxy side (SM).
      continue;
    }

    // Everything else is assumed to be a domain element.
    vtkObjectBase* object = nullptr;
    std::ostringstream name;
    name << "vtkSM" << domainEl->GetName() << ends;
    object = vtkClientServerStreamInstantiator::CreateInstance(name.str().c_str());
    if (object)
    {
      vtkSMDomain* domain = vtkSMDomain::SafeDownCast(object);
      if (domain)
      {
        domain->SetSession(proxy->GetSession());
        if (domain->ReadXMLAttributes(this, domainEl))
        {
          const char* dname = domainEl->GetAttribute("name");
          if (dname)
          {
            domain->SetXMLName(dname);
            this->AddDomain(dname, domain);
          }
        }
      }
      else
      {
        vtkErrorMacro(
          "Object created (type: " << name.str().c_str() << ") is not of a recognized type.");
      }
      object->Delete();
    }
    else
    {
      vtkErrorMacro("Could not create object of type: " << name.str().c_str()
                                                        << ". Did you specify wrong xml element?");
    }
  }

  return 1;
}

//---------------------------------------------------------------------------
void vtkSMProperty::Copy(vtkSMProperty*) {}

//---------------------------------------------------------------------------
void vtkSMProperty::WriteTo(vtkSMMessage* msg)
{
  ProxyState_Property* prop = msg->AddExtension(ProxyState::property);
  prop->set_name(this->GetXMLName());
}

//---------------------------------------------------------------------------
bool vtkSMProperty::ResetToDomainDefaults(bool use_unchecked_values)
{
  if (vtkProcessModule::GetProcessModule() &&
    vtkProcessModule::GetProcessModule()->GetSymmetricMPIMode() &&
    vtkSMProxyProperty::SafeDownCast(this) == nullptr)
  {
    // when using symmetric mpi, we disable domains since they don't always have
    // the most updated information.
    return false;
  }

  this->DomainIterator->Begin();
  while (!this->DomainIterator->IsAtEnd())
  {
    if (this->DomainIterator->GetDomain()->SetDefaultValues(this, use_unchecked_values))
    {
      return true;
    }
    this->DomainIterator->Next();
  }

  return false;
}

//---------------------------------------------------------------------------
void vtkSMProperty::ResetToDefault()
{
  if (!this->ResetToDomainDefaults(false))
  {
    this->ResetToXMLDefaults();
  }
}

//---------------------------------------------------------------------------
void vtkSMProperty::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Proxy: " << this->Proxy << endl;
  os << indent << "Command: " << (this->Command ? this->Command : "(null)") << endl;
  os << indent << "ImmediateUpdate:" << this->ImmediateUpdate << endl;
  os << indent << "InformationOnly:" << this->InformationOnly << endl;
  os << indent << "XMLName:" << (this->XMLName ? this->XMLName : "(null)") << endl;
  os << indent << "XMLLabel: " << (this->XMLLabel ? this->XMLLabel : "(null)") << endl;
  os << indent << "InformationProperty: " << this->InformationProperty << endl;
  os << indent << "Animateable: " << this->Animateable << endl;
  os << indent << "Internal: " << this->IsInternal << endl;
  os << indent << "Repeatable: " << this->Repeatable << endl;
  os << indent << "Documentation: " << this->Documentation << endl;
  os << indent << "Hints: ";
  if (this->Hints)
  {
    this->Hints->PrintSelf(os, indent);
  }
  else
  {
    os << "(none)" << endl;
  }
  if (this->Links)
  {
    os << indent << "Links: " << endl;
    this->Links->PrintSelf(os, indent.GetNextIndent());
  }
  else
  {
    os << indent << "Links: none" << endl;
  }
}

//---------------------------------------------------------------------------
void vtkSMProperty::SaveState(
  vtkPVXMLElement* parent, const char* property_name, const char* uid, int saveDomains /*=1*/)
{
  vtkPVXMLElement* propertyElement = vtkPVXMLElement::New();
  propertyElement->SetName("Property");
  propertyElement->AddAttribute("name", property_name);
  propertyElement->AddAttribute("id", uid);

  this->SaveStateValues(propertyElement);

  if (saveDomains)
  {
    this->SaveDomainState(propertyElement, uid);
  }
  parent->AddNestedElement(propertyElement);
  propertyElement->Delete();
}
//---------------------------------------------------------------------------
void vtkSMProperty::SaveStateValues(vtkPVXMLElement* /*propertyElement*/)
{
  // Concreate class should override it !!!
}
//---------------------------------------------------------------------------
void vtkSMProperty::SaveDomainState(vtkPVXMLElement* propertyElement, const char* uid)
{
  this->DomainIterator->Begin();
  while (!this->DomainIterator->IsAtEnd())
  {
    std::ostringstream dname;
    dname << uid << "." << this->DomainIterator->GetKey() << ends;
    this->DomainIterator->GetDomain()->SaveState(propertyElement, dname.str().c_str());
    this->DomainIterator->Next();
  }
}
//---------------------------------------------------------------------------
int vtkSMProperty::LoadState(vtkPVXMLElement* propertyElement, vtkSMProxyLocator* loader)
{
  // Process the domains.
  unsigned int numElems = propertyElement->GetNumberOfNestedElements();
  for (unsigned int cc = 0; cc < numElems; cc++)
  {
    vtkPVXMLElement* child = propertyElement->GetNestedElement(cc);
    if (!child->GetName())
    {
      continue;
    }
    if (strcmp(child->GetName(), "Domain") == 0)
    {
      const char* name = child->GetAttribute("name");
      vtkSMDomain* domain = name ? this->GetDomain(name) : nullptr;
      if (domain)
      {
        domain->LoadState(child, loader);
      }
    }
  }
  return 1;
}
//---------------------------------------------------------------------------
void vtkSMProperty::SetParent(vtkSMProxy* proxy)
{
  this->Proxy = proxy;
}
//---------------------------------------------------------------------------
vtkSMProxy* vtkSMProperty::GetParent()
{
  return this->Proxy.GetPointer();
}

//---------------------------------------------------------------------------
bool vtkSMProperty::HasDomainsWithRequiredProperties()
{
  for (vtkSMPropertyInternals::DomainMap::iterator iter = this->PInternals->Domains.begin();
       iter != this->PInternals->Domains.end(); ++iter)
  {
    if (iter->second->GetNumberOfRequiredProperties() > 0)
    {
      return true;
    }
  }
  return false;
}
