// Copyright (c) Lawrence Livermore National Security, LLC and other VisIt
// Project developers.  See the top-level LICENSE file for dates and other
// details.  No copyright assignment is required to contribute to VisIt.

// ************************************************************************* //
//                       avtFVCOM_MTSDFileFormat.h                           //
// ************************************************************************* //

#ifndef AVT_FVCOM_MTSD_FILE_FORMAT_H
#define AVT_FVCOM_MTSD_FILE_FORMAT_H

#include <avtMTSDFileFormat.h>
#include <avtMTSDFileFormatInterface.h>

#include <vector>

class NETCDFFileObject;
class avtFVCOMReader;

// ****************************************************************************
//  Class: avtFVCOM_MTSDFileFormat
//
//  Purpose:
//      Reads in FVCOM_MTSD files as a plugin to VisIt.
//
//  Programmer: whitlocb -- generated by xml2avt
//  Creation:   Wed May 31 15:50:52 PST 2006
//
// ****************************************************************************

class avtFVCOM_MTSDFileFormat : public avtMTSDFileFormat
{
public:
    static bool        Identify(NETCDFFileObject *);
    static avtFileFormatInterface *CreateInterface(NETCDFFileObject *f,
                         const char *const *list, int nList, int nBlock);

                       avtFVCOM_MTSDFileFormat(const char *, NETCDFFileObject *);
                       avtFVCOM_MTSDFileFormat(const char *);
    virtual           ~avtFVCOM_MTSDFileFormat();

    //
    // If you know the times and cycle numbers, overload this function.
    // Otherwise, VisIt will make up some reasonable ones for you.
    //
    virtual void        GetCycles(std::vector<int> &);
    virtual void        GetTimes(std::vector<double> &);

    virtual int            GetNTimesteps(void);

    virtual const char    *GetType(void)   { return "FVCOM_MTSD"; };
    virtual void           FreeUpResources(void); 

    virtual vtkDataSet    *GetMesh(int, const char *);
    virtual vtkDataArray  *GetVar(int, const char *);
    virtual vtkDataArray  *GetVectorVar(int, const char *);

    virtual void          *GetAuxiliaryData(const char *var,
                                            int timeState,
                                            const char *type,
                                            void *args,
                                            DestructorFunction &);



protected:
    // DATA MEMBERS

    virtual void           PopulateDatabaseMetaData(avtDatabaseMetaData *, int);

    avtFVCOMReader        *reader;
};


#endif