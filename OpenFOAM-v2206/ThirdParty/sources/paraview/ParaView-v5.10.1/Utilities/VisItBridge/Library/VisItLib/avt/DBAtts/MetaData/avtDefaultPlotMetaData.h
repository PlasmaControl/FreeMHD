// Copyright (c) Lawrence Livermore National Security, LLC and other VisIt
// Project developers.  See the top-level LICENSE file for dates and other
// details.  No copyright assignment is required to contribute to VisIt.

#ifndef AVTDEFAULTPLOTMETADATA_H
#define AVTDEFAULTPLOTMETADATA_H
#include <dbatts_exports.h>
#include <string>
#include <AttributeSubject.h>


// ****************************************************************************
// Class: avtDefaultPlotMetaData
//
// Purpose:
//    Contains default plot metadata attributes
//
// Notes:      Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//
// ****************************************************************************

class DBATTS_API avtDefaultPlotMetaData : public AttributeSubject
{
public:
    // These constructors are for objects of this class
    avtDefaultPlotMetaData();
    avtDefaultPlotMetaData(const avtDefaultPlotMetaData &obj);
protected:
    // These constructors are for objects derived from this class
    avtDefaultPlotMetaData(private_tmfs_t tmfs);
    avtDefaultPlotMetaData(const avtDefaultPlotMetaData &obj, private_tmfs_t tmfs);
public:
    virtual ~avtDefaultPlotMetaData();

    virtual avtDefaultPlotMetaData& operator = (const avtDefaultPlotMetaData &obj);
    virtual bool operator == (const avtDefaultPlotMetaData &obj) const;
    virtual bool operator != (const avtDefaultPlotMetaData &obj) const;
private:
    void Init();
    void Copy(const avtDefaultPlotMetaData &obj);
public:

    virtual const std::string TypeName() const;
    virtual bool CopyAttributes(const AttributeGroup *);
    virtual AttributeSubject *CreateCompatible(const std::string &) const;
    virtual AttributeSubject *NewInstance(bool) const;

    // Property selection methods
    virtual void SelectAll();


    // Keyframing methods
    virtual std::string               GetFieldName(int index) const;
    virtual AttributeGroup::FieldType GetFieldType(int index) const;
    virtual std::string               GetFieldTypeName(int index) const;
    virtual bool                      FieldsEqual(int index, const AttributeGroup *rhs) const;

    // User-defined methods
    avtDefaultPlotMetaData(const std::string &, const std::string &);
    void AddAttribute(const std::string& attr);
    void Print(ostream &, int = 0) const;

    // IDs that can be used to identify fields in case statements
    enum {
        ID_pluginID = 0,
        ID_plotVar,
        ID_plotAttributes,
        ID__LAST
    };

public:
    std::string  pluginID;
    std::string  plotVar;
    stringVector plotAttributes;

private:
    // Static class format string for type map.
    static const char *TypeMapFormatString;
    static const private_tmfs_t TmfsStruct;
};
#define AVTDEFAULTPLOTMETADATA_TMFS "sss*"

#endif