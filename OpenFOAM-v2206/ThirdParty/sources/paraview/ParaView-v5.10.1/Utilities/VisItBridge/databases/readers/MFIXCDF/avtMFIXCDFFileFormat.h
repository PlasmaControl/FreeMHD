// Copyright (c) Lawrence Livermore National Security, LLC and other VisIt
// Project developers.  See the top-level LICENSE file for dates and other
// details.  No copyright assignment is required to contribute to VisIt.

// ************************************************************************* //
//                            avtMFIXCDFFileFormat.h                         //
// ************************************************************************* //

#ifndef AVT_MFIXCDF_FILE_FORMAT_H
#define AVT_MFIXCDF_FILE_FORMAT_H

#include <avtSTMDFileFormat.h>
#include <vtk_netcdf.h>

#include <string>
#include <set>
#include <map>
#include <memory>

class DBOptionsAttributes;
class vtkDoubleArray;
class vtkFloatArray;
class vtkIntArray;
class vtkLongLongArray;
class vtkMFIXReader;
class vtkStringArray;
class vtkUnstructuredGrid;
class vtkUnsignedCharArray;

// ****************************************************************************
//  Class: avtMFIXCDFFileFormat
//
//  Purpose:
//      Reads in MFIXCDF files as a plugin to VisIt.
//
//  Programmer: welling -- generated by xml2avt
//  Creation:   Wed Aug 3 16:35:13 PST 2011
//
// ****************************************************************************

class avtMFIXCDFFileFormat : public avtSTMDFileFormat
{
public:
    avtMFIXCDFFileFormat(const char *, DBOptionsAttributes *);
    virtual           ~avtMFIXCDFFileFormat();

    virtual double GetTime(void);

    //
    // This is used to return unconvention data -- ranging from material
    // information to information about block connectivity.
    //
    virtual void      *GetAuxiliaryData(const char *var, int domain,
                                        const char *type, void *args,
                                        DestructorFunction &);
    //

    //
    // If you know the cycle number, overload this function.
    // Otherwise, VisIt will make up a reasonable one for you.
    //
    // virtual int         GetCycle(void);
    //

    virtual const char    *GetType(void)
    {
        return "MFIXCDF";
    };
    virtual void           FreeUpResources(void);

    virtual vtkDataSet    *GetMesh(int, const char *);
    virtual vtkDataArray  *GetVar(int, const char *);
    virtual vtkDataArray  *GetVectorVar(int, const char *);

protected:
    // DATA MEMBERS

    virtual void           PopulateDatabaseMetaData(avtDatabaseMetaData *);

private:
    void               CalcDomainBreakdown2D(long targetDomains,
                                             int cellsX, int cellsY, int* nX, int* nY);
    void               CalcDomainBreakdown3D(long targetDomains,
                                             int cellsX, int cellsY, int cellsZ,
                                             int* nX, int* nY, int* nZ);
    void               decompose_domains(int, int *, int *, int *);
    void               get_limit(int, int, int, vtkDoubleArray *,
                                 int *, int *, double **);
    void               getBlockOfDoubles1D(int ncid,
                                           const char* varname,
                                           double* data,
                                           long offset, int n);
    void               getBlockOfFloats3D(int ncid,
                                          const char* varname,
                                          float* data,
                                          long iOffset, int iN,
                                          long jOffset, int jN,
                                          long kOffset, int kN);
    void               getBlockOfInts3D(int ncid,
                                        const char* varname,
                                        int* data,
                                        long iOffset, int iN,
                                        long jOffset, int jN,
                                        long kOffset, int kN);
    void               inferVectorVariableNames(avtDatabaseMetaData *md,
                                                std::set<std::string>& varNames);
    void               checkCoordArrays(void);

    // These members are initialized in the constructor
    std::string*       filePath;
    int                dataFile;
    double             timeNow;
    int                numXDomains;
    int                numYDomains;
    int                numZDomains;
    int                iSz;
    int                jSz;
    int                kSz;
    int                par_rank;
    int                par_size;
    int                coordCode;

    // These members only become valid after execution of
    // PopulateDatabaseMetaData()
    vtkDoubleArray    *Dx;                     // Cell widths in x axis
    vtkDoubleArray    *Lx;                     // Integral of Dx
    vtkDoubleArray    *Dy;                     // Cell widths in y axis
    vtkDoubleArray    *Ly;                     // Integral of Ly
    vtkDoubleArray    *Dz;                     // Cell widths in z axis
    vtkDoubleArray    *Lz;                     // Integral of Lz
};


#endif