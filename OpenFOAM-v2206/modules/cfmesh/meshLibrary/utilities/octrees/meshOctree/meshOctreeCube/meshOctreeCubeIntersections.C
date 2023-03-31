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

\*---------------------------------------------------------------------------*/

#include "triSurf.H"
#include "meshOctreeCube.H"
#include "VRWGraph.H"
#include "helperFunctions.H"

//#define DEBUGSearch

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

bool Foam::Module::meshOctreeCube::hasContainedTriangles
(
    const triSurf& surface,
    const boundBox& rootBox,
    const VRWGraph& containedElements
) const
{
    if (containedElementsLabel_ == -1)
    {
        return false;
    }

    forAllRow(containedElements, containedElementsLabel_, tI)
    {
        if
        (
            intersectsTriangleExact
            (
                surface,
                rootBox,
                containedElements(containedElementsLabel_, tI)
            )
        )
        {
            return true;
        }
    }

    return false;
}


// ************************************************************************* //
