/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | avalanche module
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2020 Matthias Rauter
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
    Foam::functionObjects::gridfileWrite

Group
    grpUtilitiesFunctionObjects

Description

Usage

SourceFiles
    gridfileWrite.C

\*---------------------------------------------------------------------------*/

#ifndef gridfileWrite_H
#define gridfileWrite_H

#include "regionFunctionObject.H"
#include "wordRes.H"
#include "Enum.H"
#include "gridfile.H"
#include "areaFieldsFwd.H"
#include "polyMesh.H"
#include "pointList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class faMesh;

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                Class functionObjects::gridfileWrite Declaration
\*---------------------------------------------------------------------------*/

class gridfileWrite
:
    public regionFunctionObject
{
public:

    // Public Data Types

        //- Re-enumeration defining the write options, based on the original
        //  ones at IOobject::writeOption
        enum writeOption
        {
            AUTO_WRITE,
            NO_WRITE,
            ANY_WRITE
        };

        static const Enum<writeOption> writeOptionNames_;

private:

    // Private Data

        //- Reference to faMesh
        const faMesh& aMesh_;

        //- To only write objects of defined writeOption
        writeOption writeOption_;

        //- Names of objects to control
        wordRes objectNames_;

        //- First or second order interpolationss
        Switch secondOrderInterpolations_;

        //- region to export - xmin
        scalar xmin_;

        //- region to export - ymin
        scalar ymin_;

        //- region to export - xmax
        scalar xmax_;

        //- region to export - ymax
        scalar ymax_;

        //- x-resolution
        scalar dx_;

        //- y-resolution
        scalar dy_;

        //- number of cells in x-direction
        label ncols_;

        //- number of cells in y-direction
        label nrows_;

        //- x coordinate of first cell
        scalar xllcenter_;

        //- y coordinate of first cell
        scalar yllcenter_;

        //- nodata value for gridfile
        scalar NODATA_value_;

        //- outfile name
        word postfix_;

        //- offset the outfile
        vector offset_;

        //- precalculated list of nearest neighbors
        labelList nearestNeighbor_;

        //- precalculated list of nearest neighbors
        pointList pointsOnSurface_;


    // Private Member Functions

        //- No copy construct
        gridfileWrite(const gridfileWrite&) = delete;

        //- No copy assignment
        void operator=(const gridfileWrite&) = delete;

        void writeField(const areaScalarField& f, const fileName &fn) const;

        //- Find nearest neighbors of grid cell centres.
        bool findNNByCellNeighbors();

        label checkPointInCellandNeighborsIterativly
        (
            label sI,
            const labelListList& faceFaceAddr,
            scalar x, scalar y, label depth
        ) const;

public:

    //- Runtime type information
    TypeName("gridfileWrite");


    // Constructors

        //- Construct from Time and dictionary
        gridfileWrite
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~gridfileWrite() = default;


    // Member Functions

        //- Read the writeObjects data
        virtual bool read(const dictionary&);

        //- Do nothing
        virtual bool execute();

        //- Write the registered objects
        virtual bool write();

        //- Update for changes of mesh - expires the surfaces
        virtual void updateMesh(const mapPolyMesh& mpm);

        //- Update for mesh point-motion - expires the surfaces
        virtual void movePoints(const polyMesh& mesh);

        //- Update for changes of mesh due to readUpdate - expires the surfaces
        virtual void readUpdate(const polyMesh::readUpdateState state);

        //- Mark intermediate surfaces and writers as needing an update.
        void expire();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
