// Copyright (c) Lawrence Livermore National Security, LLC and other VisIt
// Project developers.  See the top-level LICENSE file for dates and other
// details.  No copyright assignment is required to contribute to VisIt.

#ifndef COLORTABLEATTRIBUTES_H
#define COLORTABLEATTRIBUTES_H
#include <state_exports.h>
#include <string>
#include <AttributeSubject.h>

class ColorControlPointList;

// ****************************************************************************
// Class: ColorTableAttributes
//
// Purpose:
//    This class contains the list of colortables.
//
// Notes:      Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//
// ****************************************************************************

class STATE_API ColorTableAttributes : public AttributeSubject
{
public:
    // These constructors are for objects of this class
    ColorTableAttributes();
    ColorTableAttributes(const ColorTableAttributes &obj);
protected:
    // These constructors are for objects derived from this class
    ColorTableAttributes(private_tmfs_t tmfs);
    ColorTableAttributes(const ColorTableAttributes &obj, private_tmfs_t tmfs);
public:
    virtual ~ColorTableAttributes();

    virtual ColorTableAttributes& operator = (const ColorTableAttributes &obj);
    virtual bool operator == (const ColorTableAttributes &obj) const;
    virtual bool operator != (const ColorTableAttributes &obj) const;
private:
    void Init();
    void Copy(const ColorTableAttributes &obj);
public:

    virtual const std::string TypeName() const;
    virtual bool CopyAttributes(const AttributeGroup *);
    virtual AttributeSubject *CreateCompatible(const std::string &) const;
    virtual AttributeSubject *NewInstance(bool) const;

    // Property selection methods
    virtual void SelectAll();
    void SelectNames();
    void SelectColorTables();
    void SelectActiveContinuous();
    void SelectActiveDiscrete();

    // Property setting methods
    void SetNames(const stringVector &names_);
    void SetActiveContinuous(const std::string &activeContinuous_);
    void SetActiveDiscrete(const std::string &activeDiscrete_);
    void SetGroupingFlag(bool groupingFlag_);

    // Property getting methods
    const stringVector &GetNames() const;
          stringVector &GetNames();
    const AttributeGroupVector &GetColorTables() const;
          AttributeGroupVector &GetColorTables();
    const std::string  &GetActiveContinuous() const;
          std::string  &GetActiveContinuous();
    const std::string  &GetActiveDiscrete() const;
          std::string  &GetActiveDiscrete();
    bool               GetGroupingFlag() const;

    // Persistence methods
    virtual bool CreateNode(DataNode *node, bool completeSave, bool forceAdd);
    virtual void SetFromNode(DataNode *node);


    // Attributegroup convenience methods
    void AddColorTables(const ColorControlPointList &);
    void ClearColorTables();
    void RemoveColorTables(int i);
    int  GetNumColorTables() const;
    ColorControlPointList &GetColorTables(int i);
    const ColorControlPointList &GetColorTables(int i) const;

    ColorControlPointList &operator [] (int i);
    const ColorControlPointList &operator [] (int i) const;


    // Keyframing methods
    virtual std::string               GetFieldName(int index) const;
    virtual AttributeGroup::FieldType GetFieldType(int index) const;
    virtual std::string               GetFieldTypeName(int index) const;
    virtual bool                      FieldsEqual(int index, const AttributeGroup *rhs) const;

    // User-defined methods
    int GetColorTableIndex(const std::string &name) const;
    const ColorControlPointList *GetColorControlPoints(int index) const;
    const ColorControlPointList *GetColorControlPoints(const std::string &name) const;
    void AddColorTable(const std::string &name, const ColorControlPointList &cpts);
    void RemoveColorTable(const std::string &name);
    void RemoveColorTable(int index);

    // IDs that can be used to identify fields in case statements
    enum {
        ID_names = 0,
        ID_colorTables,
        ID_activeContinuous,
        ID_activeDiscrete,
        ID_groupingFlag,
        ID__LAST
    };

protected:
    AttributeGroup *CreateSubAttributeGroup(int index);
private:
    stringVector         names;
    AttributeGroupVector colorTables;
    std::string          activeContinuous;
    std::string          activeDiscrete;
    bool                 groupingFlag;

    // Static class format string for type map.
    static const char *TypeMapFormatString;
    static const private_tmfs_t TmfsStruct;
};
#define COLORTABLEATTRIBUTES_TMFS "s*a*ssb"

#endif