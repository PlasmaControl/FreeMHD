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

#include "boundaryLayerOptimisation.H"
#include "meshSurfacePartitioner.H"
#include "demandDrivenData.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::boundaryLayerOptimisation::boundaryLayerOptimisation
(
    polyMeshGen& mesh
)
:
    mesh_(mesh),
    meshSurfacePtr_(new meshSurfaceEngine(mesh)),
    deleteMeshSurface_(true),
    partitionerPtr_(nullptr),
    hairEdges_(),
    hairEdgesAtBndPoint_(),
    hairEdgesNearHairEdge_(),
    isBndLayerBase_(),
    isExitFace_(),
    hairEdgeType_(),
    thinnedHairEdge_(),
    maxNumIterations_(5),
    nSmoothNormals_(5),
    relThicknessTol_(0.1),
    featureSizeFactor_(0.3),
    reCalculateNormals_(true)
{
    calculateHairEdges();
}


Foam::Module::boundaryLayerOptimisation::boundaryLayerOptimisation
(
    polyMeshGen& mesh,
    const meshSurfaceEngine& mse
)
:
    mesh_(mesh),
    meshSurfacePtr_(&mse),
    deleteMeshSurface_(false),
    partitionerPtr_(nullptr),
    hairEdges_(),
    hairEdgesAtBndPoint_(),
    hairEdgesNearHairEdge_(),
    isBndLayerBase_(),
    isExitFace_(),
    hairEdgeType_(),
    thinnedHairEdge_(),
    maxNumIterations_(5),
    nSmoothNormals_(5),
    relThicknessTol_(0.15),
    featureSizeFactor_(0.3),
    reCalculateNormals_(true)
{
    calculateHairEdges();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::Module::boundaryLayerOptimisation::~boundaryLayerOptimisation()
{
    deleteDemandDrivenData(partitionerPtr_);

    if (deleteMeshSurface_)
        deleteDemandDrivenData(meshSurfacePtr_);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::boundaryLayerOptimisation::setMaxNumIterations
(
    const label maxNumIterations
)
{
    maxNumIterations_ = maxNumIterations;
}


void Foam::Module::boundaryLayerOptimisation::setNumNormalsSmoothingIterations
(
    const label nSmoothNormals
)
{
    nSmoothNormals_ = nSmoothNormals;
}


void Foam::Module::boundaryLayerOptimisation::recalculateNormals
(
    const bool shallRecalculate
)
{
    reCalculateNormals_ = shallRecalculate;
}


void Foam::Module::boundaryLayerOptimisation::setRelativeThicknessTolerance
(
    const scalar relThicknessTol
)
{
    relThicknessTol_ = relThicknessTol;
}


void Foam::Module::boundaryLayerOptimisation::setFeatureSizeFactor
(
    const scalar featureSizeFactor
)
{
    featureSizeFactor_ = featureSizeFactor;
}


const Foam::Module::edgeLongList&
Foam::Module::boundaryLayerOptimisation::hairEdges() const
{
    return hairEdges_;
}


const Foam::Module::VRWGraph&
Foam::Module::boundaryLayerOptimisation::hairEdgesAtBndPoint() const
{
    return hairEdgesAtBndPoint_;
}


const Foam::boolList&
Foam::Module::boundaryLayerOptimisation::isBaseFace() const
{
    return isBndLayerBase_;
}


const Foam::boolList&
Foam::Module::boundaryLayerOptimisation::isExitFace() const
{
    return isExitFace_;
}


void Foam::Module::boundaryLayerOptimisation::readSettings
(
    const dictionary& meshDict,
    boundaryLayerOptimisation& blOptimisation
)
{
    if (meshDict.found("boundaryLayers"))
    {
        const dictionary& layersDict = meshDict.subDict("boundaryLayers");

        bool smoothLayers;
        if (layersDict.readIfPresent("optimiseLayer", smoothLayers))
        {
            if (!smoothLayers) return;
        }

        if (layersDict.found("optimisationParameters"))
        {
            const dictionary& optParams =
                layersDict.subDict("optimisationParameters");

            bool recalcNormals;
            if (optParams.readIfPresent("recalculateNormals", recalcNormals))
            {
                blOptimisation.recalculateNormals(recalcNormals);
            }

            label nSmoothNormals;
            if (optParams.readIfPresent("nSmoothNormals", nSmoothNormals))
            {
                blOptimisation.setNumNormalsSmoothingIterations(nSmoothNormals);
            }

            scalar featureSizeFactor;
            if (optParams.readIfPresent("featureSizeFactor", featureSizeFactor))
            {
                if (featureSizeFactor >= 1.0 || featureSizeFactor < 0.0)
                    FatalErrorInFunction
                        << "Feature size factor is out"
                        << " of a valid range 0 to 1" << exit(FatalError);

                blOptimisation.setFeatureSizeFactor(featureSizeFactor);
            }

            scalar relThicknessTol;
            if (optParams.readIfPresent("relThicknessTol", relThicknessTol))
            {
                if (relThicknessTol >= 1.0 || relThicknessTol < 0.0)
                    FatalErrorInFunction
                        << "Relative thickness tolerance is out"
                        << " of a valid range 0 to 1" << exit(FatalError);

                blOptimisation.setRelativeThicknessTolerance(relThicknessTol);
            }

            label maxNumIterations;
            if (optParams.readIfPresent("maxNumIterations", maxNumIterations))
            {
                blOptimisation.setMaxNumIterations(maxNumIterations);
            }
        }
    }
}


// ************************************************************************* //
