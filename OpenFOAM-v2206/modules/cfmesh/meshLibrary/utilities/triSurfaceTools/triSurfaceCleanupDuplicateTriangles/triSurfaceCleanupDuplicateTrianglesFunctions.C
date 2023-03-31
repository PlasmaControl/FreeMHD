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

#include "triSurfaceCleanupDuplicateTriangles.H"
#include "triSurfModifier.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::triSurfaceCleanupDuplicateTriangles::
checkDuplicateTriangles()
{
    labelLongList newTriangleLabel(surf_.size(), -1);

    const VRWGraph& pointTriangles = surf_.pointFacets();

    // check if there exist duplicate triangles
    label counter(0);

    forAll(surf_, triI)
    {
        if (newTriangleLabel[triI] != -1)
            continue;

        newTriangleLabel[triI] = counter;
        ++counter;

        const labelledTri& tri = surf_[triI];

        forAll(pointTriangles[tri[0]], ptI)
        {
            const label triJ = pointTriangles(tri[0], ptI);

            if (triJ <= triI)
                continue;

            const labelledTri& otherTri = surf_[triJ];

            if (tri == otherTri)
                newTriangleLabel[triJ] = newTriangleLabel[triI];
        }
    }

    Info<< "Found " << (newTriangleLabel.size() - counter)
        << " duplicate triangles" << endl;

    // return if there exist no duplicate triangles
    if (counter == newTriangleLabel.size())
        return;

    Info<< "Current number of triangles" << surf_.size() << endl;
    Info<< "New number of triangles " << counter << endl;

    // create new list of triangles and store it in the surface mesh
    LongList<labelledTri> newTriangles(counter);

    forAll(newTriangleLabel, triI)
    {
        newTriangles[newTriangleLabel[triI]] = surf_[triI];
    }

    triSurfModifier(surf_).facetsAccess().transfer(newTriangles);
    surf_.updateFacetsSubsets(newTriangleLabel);

    surf_.clearAddressing();
    surf_.clearGeometry();
}


// ************************************************************************* //
