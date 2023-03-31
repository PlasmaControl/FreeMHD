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

#include "triSurfaceCleanupDuplicates.H"
#include "meshOctree.H"
#include "demandDrivenData.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::Module::triSurfaceCleanupDuplicates::triSurfaceCleanupDuplicates
(
    const meshOctree& octree,
    const scalar tol
)
:
    tolerance_(tol),
    surf_(const_cast<triSurf&>(octree.surface())),
    octree_(octree),
    newTriangleLabel_(),
    done_(false)
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::triSurfaceCleanupDuplicates::mergeIdentities()
{
    if (Pstream::parRun())
        FatalError << "Material detection does not run in parallel"
            << exit(FatalError);

    if (done_)
    {
        WarningInFunction
            << "Operation is already performed" << endl;
        return;
    }

    newTriangleLabel_.setSize(surf_.size());
    forAll(newTriangleLabel_, triI)
        newTriangleLabel_[triI] = triI;

    bool finished;
    do
    {
        finished = true;

        if (checkDuplicateTriangles())
            finished = false;
        if (mergeDuplicatePoints())
            finished = false;
    } while (!finished);

    done_ = true;
}


// ************************************************************************* //
