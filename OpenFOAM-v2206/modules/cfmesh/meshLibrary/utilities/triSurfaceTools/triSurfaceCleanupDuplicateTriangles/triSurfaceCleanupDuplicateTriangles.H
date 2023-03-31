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
    Foam::Module::triSurfaceCleanupDuplicateTriangles

Description
    Removes duplicate triangles from the surface.

SourceFiles
    triSurfaceCleanupDuplicateTriangles.C
    triSurfaceCleanupDuplicateTrianglesFunctions.C

\*---------------------------------------------------------------------------*/

#ifndef triSurfaceCleanupDuplicateTriangles_H
#define triSurfaceCleanupDuplicateTriangles_H

#include "VRWGraph.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

// Forward declarations
class triSurf;

/*---------------------------------------------------------------------------*\
             Class triSurfaceCleanupDuplicateTriangles Declaration
\*---------------------------------------------------------------------------*/

class triSurfaceCleanupDuplicateTriangles
{
    // Private data

        //- reference to triSurf
        triSurf& surf_;

        //- new triangle labels in case some of them is removed
        labelLongList newTriangleLabel_;


    // Private member functions

        //- Check duplicate triangles
        void checkDuplicateTriangles();

        //- Disallow default bitwise copy construct
        triSurfaceCleanupDuplicateTriangles
        (
            const triSurfaceCleanupDuplicateTriangles&
        );

        //- Disallow default bitwise assignment
        void operator=(const triSurfaceCleanupDuplicateTriangles&);


public:

    //- Construct from triSurf
    triSurfaceCleanupDuplicateTriangles(triSurf& surf);

    //- Destructor
    ~triSurfaceCleanupDuplicateTriangles() = default;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
