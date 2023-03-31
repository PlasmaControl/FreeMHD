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
    Foam::functionObjects::shapefileWrite

Group
    grpUtilitiesFunctionObjects

Description

Usage

SourceFiles
    shapefileWrite.C

\*---------------------------------------------------------------------------*/

#ifndef shapefileWrite_H
#define shapefileWrite_H

#include "regionFunctionObject.H"
#include "wordRes.H"
#include "Enum.H"
#include "shapefile.H"
#include "areaFieldsFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class faMesh;

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
               Class functionObjects::shapefileWrite Declaration
\*---------------------------------------------------------------------------*/

class shapefileWrite
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

        //- The shapefile which is written
        shapefile shp_;

        //- To only write objects of defined writeOption
        writeOption writeOption_;

        //- Names of objects to control
        wordRes objectNames_;

        //- outfile name
        word prefix_;

        //- offset the outfile
        vector offset_;


    // Private Member Functions

        //- No copy construct
        shapefileWrite(const shapefileWrite&) = delete;

        //- No copy assignment
        void operator=(const shapefileWrite&) = delete;

        shapefile& generateShapeFileGeometry(const faMesh &mesh);

        shapefile& addToShapeFile(const areaScalarField &f, shapefile &shp);

        shapefile& addToShapeFile(const areaVectorField &f, shapefile &shp);

        shapefile& addToShapeFile(const areaSphericalTensorField &f, shapefile &shp);

        shapefile& addToShapeFile(const areaSymmTensorField &f, shapefile &shp);

        shapefile& addToShapeFile(const areaTensorField &f, shapefile &shp);

        bool writeShapeFile(const shapefile &shp, const fileName &filename);

public:

    //- Runtime type information
    TypeName("shapefileWrite");


    // Constructors

        //- Construct from Time and dictionary
        shapefileWrite
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~shapefileWrite() = default;


    // Member Functions

        //- Read the writeObjects data
        virtual bool read(const dictionary&);

        //- Do nothing
        virtual bool execute();

        //- Write the registered objects
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
