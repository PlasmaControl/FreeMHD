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
    Foam::Module::edgeMeshGeometryModification

Description
    Modifies geometry of edge meshes according to the anisotropic sources
    given by the user

SourceFiles
    edgeMeshGeometryModification.C

\*---------------------------------------------------------------------------*/

#ifndef edgeMeshGeometryModification_H
#define edgeMeshGeometryModification_H

#include "coordinateModification.H"
#include "point.H"
#include "typeInfo.H"
#include "coordinateModifier.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
class edgeMesh;
class dictionary;

namespace Module
{

/*---------------------------------------------------------------------------*\
                Class edgeMeshGeometryModification Declaration
\*---------------------------------------------------------------------------*/

class edgeMeshGeometryModification
{
    // Private Data

        //- reference to edgeMesh
        const edgeMesh& edgeMesh_;

        //- length of box sides
        const dictionary& meshDict_;

        //- contruct coordinate modification
        coordinateModifier* coordinateModifierPtr_;

        //- is mofdification active
        bool modificationActive_;


    // Private Member Functions

        //- check existence of geometry modifiers
        void checkModification();

        //- No copy construct
        edgeMeshGeometryModification
            (const edgeMeshGeometryModification&) = delete;

        //- No copy assignment
        void operator=(const edgeMeshGeometryModification&) = delete;


public:

    //- Construct from edgeMesh and dictionary
    edgeMeshGeometryModification(const edgeMesh&, const dictionary&);

    //- Destructor
    ~edgeMeshGeometryModification();


    // Member Functions

        //- is geometry modification active
        bool activeModification() const;

        //- modify coordinates
        const edgeMesh* modifyGeometry() const;

        //- revert geometry modification
        const edgeMesh* revertGeometryModification() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
