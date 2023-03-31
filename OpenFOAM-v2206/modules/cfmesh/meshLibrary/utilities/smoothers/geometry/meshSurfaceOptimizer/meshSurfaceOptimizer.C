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

#include "demandDrivenData.H"
#include "meshSurfaceOptimizer.H"
#include "meshSurfaceEngine.H"
#include "meshSurfacePartitioner.H"
#include "meshSurfaceMapper.H"
#include "polyMeshGenChecks.H"

#include <map>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::meshSurfaceOptimizer::classifySurfaceVertices()
{
    const labelHashSet& corners = partitionerPtr_->corners();
    const labelHashSet& edgePoints = partitionerPtr_->edgePoints();

    // set all vertices to partition
    vertexType_ = PARTITION;

    // set corners
    for (const label corneri : corners)
    {
        vertexType_[corneri] = CORNER;
    }

    // set edges
    for (const label edgei : edgePoints)
    {
        vertexType_[edgei] = EDGE;
    }

    if (Pstream::parRun())
    {
        // mark nodes at parallel boundaries
        const Map<label>& globalToLocal =
            surfaceEngine_.globalToLocalBndPointAddressing();

        forAllConstIters(globalToLocal, iter)
        {
            const label bpI = iter();

            vertexType_[bpI] |= PROCBND;
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::meshSurfaceOptimizer::meshSurfaceOptimizer
(
    const meshSurfaceEngine& surface
)
:
    surfaceEngine_(surface),
    vertexType_(surface.boundaryPoints().size()),
    partitionerPtr_(new meshSurfacePartitioner(surface)),
    deletePartitioner_(true),
    octreePtr_(nullptr),
    triMeshPtr_(nullptr),
    enforceConstraints_(false),
    badPointsSubsetName_("invertedBoundaryPoints")
{
    classifySurfaceVertices();
}


Foam::Module::meshSurfaceOptimizer::meshSurfaceOptimizer
(
    const meshSurfacePartitioner& mPart
)
:
    surfaceEngine_(mPart.surfaceEngine()),
    vertexType_(surfaceEngine_.boundaryPoints().size()),
    partitionerPtr_(&mPart),
    deletePartitioner_(true),
    octreePtr_(nullptr),
    triMeshPtr_(nullptr),
    enforceConstraints_(false),
    badPointsSubsetName_("invertedBoundaryPoints")
{
    classifySurfaceVertices();
}


Foam::Module::meshSurfaceOptimizer::meshSurfaceOptimizer
(
    const meshSurfaceEngine& surface,
    const meshOctree& octree
)
:
    surfaceEngine_(surface),
    vertexType_(surface.boundaryPoints().size()),
    partitionerPtr_(new meshSurfacePartitioner(surface)),
    deletePartitioner_(true),
    octreePtr_(&octree),
    triMeshPtr_(nullptr),
    enforceConstraints_(false),
    badPointsSubsetName_("invertedBoundaryPoints")
{
    classifySurfaceVertices();
}


Foam::Module::meshSurfaceOptimizer::meshSurfaceOptimizer
(
    const meshSurfacePartitioner& partitioner,
    const meshOctree& octree
)
:
    surfaceEngine_(partitioner.surfaceEngine()),
    vertexType_(surfaceEngine_.boundaryPoints().size()),
    partitionerPtr_(&partitioner),
    deletePartitioner_(false),
    octreePtr_(&octree),
    triMeshPtr_(nullptr),
    enforceConstraints_(false),
    badPointsSubsetName_("invertedBoundaryPoints")
{
    classifySurfaceVertices();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::Module::meshSurfaceOptimizer::~meshSurfaceOptimizer()
{
    deleteDemandDrivenData(triMeshPtr_);

    if (deletePartitioner_)
        deleteDemandDrivenData(partitionerPtr_);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::meshSurfaceOptimizer::removeUserConstraints()
{
    # ifdef USE_OMP
    # pragma omp parallel for schedule(dynamic, 100)
    # endif
    forAll(vertexType_, bpI)
        if (vertexType_[bpI] & LOCKED)
            vertexType_[bpI] ^= LOCKED;
}


void Foam::Module::meshSurfaceOptimizer::enforceConstraints
(
    const word subsetName
)
{
    enforceConstraints_ = true;

    badPointsSubsetName_ = subsetName;
}


// ************************************************************************* //
