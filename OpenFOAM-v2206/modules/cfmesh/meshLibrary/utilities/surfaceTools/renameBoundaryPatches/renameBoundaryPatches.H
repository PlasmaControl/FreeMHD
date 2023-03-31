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
    Foam::Module::renameBoundaryPatches

Description
    Renames boundary patches based on user intput and sets correct patch types

SourceFiles
    renameBoundaryPatches.C

\*---------------------------------------------------------------------------*/

#ifndef renameBoundaryPatches_H
#define renameBoundaryPatches_H

#include "polyMeshGenModifier.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
// Forward declarations
class IOdictionary;

namespace Module
{

/*---------------------------------------------------------------------------*\
                    Class renameBoundaryPatches Declaration
\*---------------------------------------------------------------------------*/

class renameBoundaryPatches
{
    // Private data

        //- mesh
        polyMeshGen& mesh_;

        //- dictionary containing relevant information
        const IOdictionary& meshDict_;


    // Private member functions

        //- calculate new boundary and replace the existing one
        void calculateNewBoundary();

        //- find empty patches in 3D meshes and change them to wall
        void checkEmptyPatches();

        //- check if there exist any symmetry planes
        //- and make sure they get perfectly flat
        void checkSymmetryPlanes();

        //- Disallow default bitwise copy construct
        renameBoundaryPatches(const renameBoundaryPatches&);

        //- Disallow default bitwise assignment
        void operator=(const renameBoundaryPatches&);


public:

    //- Construct from mesh and IOdictionary
    renameBoundaryPatches
    (
        polyMeshGen& mesh,
        const IOdictionary& meshDict,
        const bool allowEmptyPatches = false
    );

    //- Destructor
    ~renameBoundaryPatches() = default;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
