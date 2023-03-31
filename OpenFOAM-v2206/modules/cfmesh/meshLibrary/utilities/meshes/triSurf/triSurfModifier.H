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
    Foam::Module::triSurfModifier

Description
    A class allowing non-const access to the member of the surface mesh

SourceFiles
    triSurfModifier.C

\*---------------------------------------------------------------------------*/

#ifndef triSurfModifier_H
#define triSurfModifier_H

#include "triSurf.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
                       Class triSurfModifier Declaration
\*---------------------------------------------------------------------------*/

class triSurfModifier
{
    // Private data

        //- reference to the surface mesh
        triSurf& surface_;


    // Disallow bitwise assignment

        //- Default construct
        triSurfModifier();

        //- Disallow default bitwise assignment
        void operator=(const triSurfModifier&);


public:

    //- Construct from surface mesh
    triSurfModifier(triSurf& surface);

    //- Destructor
    ~triSurfModifier() = default;


    // Member Functions

        //- non-const access to points
        inline pointField& pointsAccess();

        //- access to facets
        inline LongList<labelledTri>& facetsAccess();

        //- non-const access to feature edges
        inline edgeLongList& featureEdgesAccess();

        //- access to patches
        inline geometricSurfacePatchList& patchesAccess();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "triSurfModifierI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
