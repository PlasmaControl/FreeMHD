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
    gridfile

Description
    A class for handling ESRI-gridfiles.

SourceFiles
    gridfile.C

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#ifndef gridfile_H
#define gridfile_H

#include <iostream>

class gridfile
{
public:

    gridfile();

    gridfile(const double &xllcenter, const double &yllcenter,
             const double &dx, const double &dy,
             const int &ncols, const int &nrows);

    ~gridfile();

    std::ostream &log() const;

    void clear();

    std::string info() const;

    int read(std::string filename_);

    int write(std::string filename_);

    //Bilinear interpolation
    double interpolate(const double &x, const double &y) const;

    //Nearest neighbor interpolation
    double interpolateNN(const double &x, const double &y) const;

    inline const std::string &filename() const {return this->filename_;}

    inline const double &xllcenter() const {return xllcenter_;}

    inline const double &yllcenter() const {return yllcenter_;}

    inline const double &dx() const {return dx_;}

    inline const double &dy() const {return dy_;}

    inline const unsigned int &ncols() const {return ncols_;}

    inline const unsigned int &nrows() const {return nrows_;}

    inline const unsigned int &xCellCount() const {return ncols();}

    inline const unsigned int &yCellCount() const {return nrows();}

    inline const double &v(int xindex, int yindex) const {return v_[xindex][yindex];}

    inline double &vRef(int xindex, int yindex) {return v_[xindex][yindex];}

    inline double** vRef() {return v_;}

    inline const double &NODATA_value() const {return NODATA_value_;}

private:
    int debug;

    //Last known filename
    std::string filename_;

    //lower left pixel centre coordinates
    double xllcenter_;
    double yllcenter_;

    //cell size of the raster x-direction
    double dx_;

    //cell size of the raster y-direction
    double dy_;

    //number of columns
    unsigned int ncols_;

    //number of rows
    unsigned int nrows_;

    //proxy value indicating that no data is available
    double NODATA_value_;

    //field values in a nrows x ncols array
    double **v_;
};

#endif

// ************************************************************************* //
