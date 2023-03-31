/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | avalanche module
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2019 Matthias Rauter
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    shapefile

Description
    A minimalistic class for handling ESRI shapefiles. The class can read and
    write a subset of ESRI shapefiles. The class is limited to points,
    polylines and ploygons. 

    en.wikipedia.org/wiki/Shapefile

    www.esri.com/library/whitepapers/pdfs/shapefile.pdf

SourceFiles
    shapefile.C

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#ifndef shapefile_H
#define shapefile_H

#include <vector>
#include <iostream>

class shapefile
{
public:

    enum type
    {
        NULLSHP=0, //read & write implemented
        POINT=1, //read & write implemented
        POLYLINE=3, //read & write implemented
        POLYGON=5, //read & write implemented
        MULTIPOINT=8, //read & write implemented
        POINTZ=11, //read & write implemented
        POLYLINEZ=13, //read & write implemented
        POLYGONZ=15, //read & write implemented
        MULTIPOINTZ=18, //read & write implemented
        POINTM=21, //read & write implemented
        POLYLINEM=23, //read & write implemented
        POLYGONM=25, //read & write implemented
        MULTIPOINTM=28, //read & write implemented
        MULTIPATCH=31 //read & write implemented
    };

    enum parttype
    {
        TRIANGLE_STRIP = 0,
        TRIANGLE_FAN = 1,
        OUTER_RING = 2,
        INNER_RING = 3,
        FIRST_RING = 4,
        RING = 5
    };

    shapefile(type shptype_ = NULLSHP);

    inline const uint32_t &shptype() const {return this->shptype_;}

    inline const unsigned int &fieldcount() const {return this->fieldcount_;}

    inline const unsigned int &recordcount() const {return this->recordcount_;}

    inline const int &partcount(int recordIndex) const {return this->shapeparts_[recordIndex];}

    inline const int &pointcount(int recordIndex) const {return this->shapepoints_[recordIndex];}

    int pointcount(int recordIndex, int partIndex) const;

    inline const int &parttype(int recordIndex, int partIndex) const {return this->pt_[recordIndex][partIndex];}

    inline const int &partStart(int recordIndex, int partIndex) const {return this->pn_[recordIndex][partIndex];}

    inline const double &x(int recordIndex, int pointIndex) const {return this->px_[recordIndex][pointIndex];}

    inline const double &y(int recordIndex, int pointIndex) const {return this->py_[recordIndex][pointIndex];}

    inline const double &z(int recordIndex, int pointIndex) const {return this->pz_[recordIndex][pointIndex];}

    inline const double &m(int recordIndex, int pointIndex) const {return this->pm_[recordIndex][pointIndex];}

    inline double &xRef(int recordIndex, int pointIndex) {return this->px_[recordIndex][pointIndex];}

    inline double &yRef(int recordIndex, int pointIndex) {return this->py_[recordIndex][pointIndex];}

    inline double &zRef(int recordIndex, int pointIndex) {return this->pz_[recordIndex][pointIndex];}

    inline double &mRef(int recordIndex, int pointIndex) {return this->pm_[recordIndex][pointIndex];}


    inline const std::string &fieldname(int fieldIndex) const {return this->fn_[fieldIndex];}

    inline bool isnumeric(int fieldIndex) const {return this->isnumeric_[fieldIndex];}


    const double &numericfield(int recordIndex, int fieldIndex_) const;

    const std::string &stringfield(int recordIndex, int fieldIndex_) const;


    int addRecord();

    int addPart(int record);

    int addPoint(int record, double x, double y, double z=0, double m=0);

    int addField(std::string fieldname, char fieldtype, int length, int decimals);

    void setField(int recordIndex,int fieldIndex_,  double value);

    void setField(int recordIndex, int fieldIndex_, const std::string &value);


    std::ostream &log() const;

    void clear();

    void calcBoundingBox();

    std::string info() const;

    int read(std::string filename_);

    int write(std::string filename_);

    void setDebugLvl(int d) {this->debug_=d;}

private:

    //debug level
    int debug_;

    //Last known filename
    std::string filename_;

    //Shape type
    uint32_t shptype_;

    //Number of shapes in the file
    unsigned int recordcount_;

    //Number of parts in a record
    std::vector<int> shapeparts_;

    //Index of parts
    std::vector<std::vector<int> > pn_;

    //Type of parts, only used in multipatch
    std::vector<std::vector<int> > pt_;

    //Number of points in a record
    std::vector<int> shapepoints_;

    //x-values
    std::vector<std::vector<double> > px_;

    //y-values
    std::vector<std::vector<double> > py_;

    //z-values
    std::vector<std::vector<double> > pz_;

    //m-values
    std::vector<std::vector<double> > pm_;

    //Number of different fields
    unsigned int fieldcount_;

    //Field Names
    std::vector<std::string> fn_;

    //Field types
    std::vector<char> ft_;

    //Field lengths
    std::vector<int> fl_;

    //Field decimal places
    std::vector<int> fd_;

    //Field is numeric and values are found in v
    std::vector<bool> isnumeric_;

    //Index in v or c where the field values can be found
    std::vector<int> fieldIndex_;

    //Field values
    std::vector<std::vector<double> > v_;
    std::vector<std::vector<std::string> > c_;

    //part boundary boxes;
     std::vector<double> bxmin_, bxmax_;
     std::vector<double> bymin_, bymax_;
     std::vector<double> bzmin_, bzmax_;
     std::vector<double> bmmin_, bmmax_;

    //Boundary box
    double xmin_, xmax_;
    double ymin_, ymax_;
    double zmin_, zmax_;
    double mmin_, mmax_;


    char dbase_versionbyte_;
    char dbase_datestring_[3];
};

#endif

// ************************************************************************* //
