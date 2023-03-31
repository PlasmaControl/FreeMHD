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

#include "meshSurfaceEdgeExtractorFUN.H"
#include "meshSurfaceEngine.H"
#include "demandDrivenData.H"

# ifdef USE_OMP
#include <omp.h>
# endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::Module::meshSurfaceEngine&
Foam::Module::meshSurfaceEdgeExtractorFUN::surfaceEngine()
{
    # ifdef USE_OMP
    if (omp_in_parallel())
        FatalErrorInFunction
            << "Cannot create surface engine with a parallel region"
            << exit(FatalError);
    # endif

    if (!surfaceEnginePtr_)
        surfaceEnginePtr_ = new meshSurfaceEngine(mesh_);

    return *surfaceEnginePtr_;
}


void Foam::Module::meshSurfaceEdgeExtractorFUN::clearOut()
{
    deleteDemandDrivenData(surfaceEnginePtr_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::meshSurfaceEdgeExtractorFUN::meshSurfaceEdgeExtractorFUN
(
    polyMeshGen& mesh,
    const meshOctree& octree,
    const bool createWrapperSheet
)
:
    mesh_(mesh),
    meshOctree_(octree),
    surfaceEnginePtr_(nullptr),
    createWrapperSheet_(createWrapperSheet)
{
    if (Pstream::parRun())
        FatalErrorInFunction
            << "Cannot run in parallel!" << exit(FatalError);

    createBasicFundamentalSheets();

    smoothMeshSurface();

    remapBoundaryPoints();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::Module::meshSurfaceEdgeExtractorFUN::~meshSurfaceEdgeExtractorFUN()
{
    deleteDemandDrivenData(surfaceEnginePtr_);
}


// ************************************************************************* //
