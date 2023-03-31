/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | cfMesh: A library for mesh generation
   \\    /   O peration     |
    \\  /    A nd           | www.cfmesh.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2014-2017 Creative Fields, Ltd.
-------------------------------------------------------------------------------
Author
     Franjo Juretic (franjo.juretic@c-fields.com)

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
    Foam::Module::createFundamentalSheets

Description
    A base class for various method to generate fundamental sheets
    necessary to capture feature edges

SourceFiles
    createFundamentalSheets.C

\*---------------------------------------------------------------------------*/

#ifndef createFundamentalSheets_H
#define createFundamentalSheets_H

#include "polyMeshGenModifier.H"
#include "labelLongList.H"
#include "boolList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{
// Forward declarations
class meshOctree;
class meshSurfaceEngine;

/*---------------------------------------------------------------------------*\
                   Class createFundamentalSheets Declaration
\*---------------------------------------------------------------------------*/

class createFundamentalSheets
{
protected:

    // Protected data

        //- type of sheet creation
        word name_;

        //- reference to mesh
        polyMeshGen& mesh_;

        //- shall the procedure create the intial wrapper sheet
        const bool createWrapperSheet_;


    // Protected member functions

        //- Disallow default bitwise copy construct
        createFundamentalSheets(const createFundamentalSheets&);

        //- Disallow default bitwise assignment
        void operator=(const createFundamentalSheets&);


public:

    //- Runtime type information
    TypeName("createFundamentalSheets");


    //- Construct from mesh data
    createFundamentalSheets
    (
        polyMeshGen& mesh,
        const bool createWrapperSheet = true
    );


    //- Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        createFundamentalSheets,
        polyMeshGen,
        (
            polyMeshGen& mesh,
            const bool createWrapperSheet
        ),
        (mesh, createWrapperSheet)
    );


    //- Select constructed from dictionary
    static autoPtr<createFundamentalSheets> New
    (
        polyMeshGen& mesh, const bool createWrapperSheet = true
    );


    //- Destructor
    virtual ~createFundamentalSheets() = default;


    // Member Functions

        //- return the method name
        const word& name() const
        {
            return name_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
