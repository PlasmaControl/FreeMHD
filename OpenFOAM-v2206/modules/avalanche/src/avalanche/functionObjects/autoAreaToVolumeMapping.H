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
    Foam::functionObjects::autoAreaToVolumeMapping

Group
    grpUtilitiesFunctionObjects

Description

Usage


SourceFiles
    shapefileWrite.C

\*---------------------------------------------------------------------------*/

#ifndef autoAreaToVolumeMapping_H
#define autoAreaToVolumeMapping_H

#include "regionFunctionObject.H"
#include "wordRes.H"
#include "Enum.H"
#include "areaFieldsFwd.H"
#include "volMesh.H"
#include "fvMesh.H"
#include "volSurfaceMapping.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class faMesh;

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
          Class functionObjects::autoAreaToVolumeMapping Declaration
\*---------------------------------------------------------------------------*/

class autoAreaToVolumeMapping
:
    public regionFunctionObject
{
public:

    // Public data types

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

        //- Reference to fvMesh
        const fvMesh& mesh_;

        //- To only write objects of defined writeOption
        writeOption writeOption_;

        //- Names of objects to control
        wordRes objectNames_;

        //- outfile name
        word prefix_;

        //The surfacemapper
        volSurfaceMapping vsm_;

    // Private Member Functions

        //- No copy construct
        autoAreaToVolumeMapping(const autoAreaToVolumeMapping&) = delete;

        //- No copy assignment
        void operator=(const autoAreaToVolumeMapping&) = delete;

public:

    //- Runtime type information
    TypeName("autoAreaToVolumeMapping");


    // Constructors

        //- Construct from Time and dictionary
        autoAreaToVolumeMapping
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~autoAreaToVolumeMapping() = default;


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
