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

#include "refineBoundaryLayers.H"
#include "meshSurfaceEngine.H"
#include "demandDrivenData.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

const Foam::Module::meshSurfaceEngine&
Foam::Module::refineBoundaryLayers::surfaceEngine() const
{
    if (!msePtr_)
    {
        msePtr_ = new meshSurfaceEngine(mesh_);
    }

    return *msePtr_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::refineBoundaryLayers::refineBoundaryLayers(polyMeshGen& mesh)
:
    mesh_(mesh),
    msePtr_(nullptr),
    globalNumLayers_(1),
    globalThicknessRatio_(1.0),
    globalMaxThicknessFirstLayer_(VGREAT),
    numLayersForPatch_(),
    thicknessRatioForPatch_(),
    maxThicknessForPatch_(),
    discontinuousLayersForPatch_(),
    cellSubsetName_(),
    done_(false),
    is2DMesh_(false),
    specialMode_(false),
    nLayersAtBndFace_(),
    splitEdges_(),
    splitEdgesAtPoint_(),
    newVerticesForSplitEdge_(),
    facesFromFace_(),
    newFaces_()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::Module::refineBoundaryLayers::~refineBoundaryLayers()
{
    deleteDemandDrivenData(msePtr_);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::refineBoundaryLayers::avoidRefinement()
{
    if (done_)
    {
        FatalErrorInFunction
            << "refineLayers is already executed" << exit(FatalError);
    }

    globalNumLayers_ = 1;
    numLayersForPatch_.clear();
}


void Foam::Module::refineBoundaryLayers::activate2DMode()
{
    if (done_)
    {
        FatalErrorInFunction
            << "refineLayers is already executed" << exit(FatalError);
    }

    is2DMesh_ = true;
}


void Foam::Module::refineBoundaryLayers::setGlobalNumberOfLayers
(
    const label nLayers
)
{
    if (done_)
    {
        FatalErrorInFunction
            << "refineLayers is already executed" << exit(FatalError);
    }

    if (nLayers < 2)
    {
        WarningInFunction
            << "The specified global number of boundary layers is less than 2"
            << endl;

        return;
    }

    globalNumLayers_ = nLayers;
}


void Foam::Module::refineBoundaryLayers::setGlobalThicknessRatio
(
    const scalar thicknessRatio
)
{
    if (done_)
    {
        FatalErrorInFunction
            << "refineLayers is already executed" << exit(FatalError);
    }

    if (thicknessRatio < 1.0)
    {
        WarningInFunction
            << "The specified global thickness ratio is less than 1.0" << endl;

        return;
    }

    globalThicknessRatio_ = thicknessRatio;
}


void Foam::Module::refineBoundaryLayers::setGlobalMaxThicknessOfFirstLayer
(
    const scalar maxThickness
)
{
    if (done_)
    {
        FatalErrorInFunction
            << "refineLayers is already executed" << exit(FatalError);
    }

    if (maxThickness <= 0.0)
    {
        WarningInFunction
            << "The specified global maximum thickness of the first"
            << " boundary layer is negative!!" << endl;

        return;
    }

    globalMaxThicknessFirstLayer_ = maxThickness;
}


void Foam::Module::refineBoundaryLayers::setNumberOfLayersForPatch
(
    const word& patchName,
    const label nLayers
)
{
    if (done_)
    {
        FatalErrorInFunction
            << "refineLayers is already executed" << exit(FatalError);
    }

    if (nLayers < 2)
    {
        WarningInFunction
            << "The specified number of boundary layers for patch "
            << patchName
            << " is less than 2... boundary layers disabled for this patch!"
            << endl;
    }

    const labelList matchedIDs = mesh_.findPatches(patchName);

    forAll(matchedIDs, matchI)
    {
        numLayersForPatch_[mesh_.getPatchName(matchedIDs[matchI])] = nLayers;
    }
}


void Foam::Module::refineBoundaryLayers::setThicknessRatioForPatch
(
    const word& patchName,
    const scalar thicknessRatio
)
{
    if (done_)
    {
        FatalErrorInFunction
            << "refineLayers is already executed" << exit(FatalError);
    }

    if (thicknessRatio < 1.0)
    {
        WarningInFunction
            << "The specified thickness ratio for patch " << patchName
            << " is less than 1.0" << endl;

        return;
    }

    const labelList matchedIDs = mesh_.findPatches(patchName);

    forAll(matchedIDs, matchI)
    {
        const word pName = mesh_.getPatchName(matchedIDs[matchI]);
        thicknessRatioForPatch_[pName] = thicknessRatio;
    }
}


void Foam::Module::refineBoundaryLayers::setMaxThicknessOfFirstLayerForPatch
(
    const word& patchName,
    const scalar maxThickness
)
{
    if (done_)
    {
        FatalErrorInFunction
            << "refineLayers is already executed" << exit(FatalError);
    }

    if (maxThickness <= 0.0)
    {
        WarningInFunction
            << "The specified maximum thickness of the first boundary layer "
            << "for patch " << patchName << " is negative!!" << endl;

        return;
    }

    const labelList matchedIDs = mesh_.findPatches(patchName);

    forAll(matchedIDs, matchI)
    {
        const word pName = mesh_.getPatchName(matchedIDs[matchI]);
        maxThicknessForPatch_[pName] = maxThickness;
    }
}


void Foam::Module::refineBoundaryLayers::setInteruptForPatch
(
    const word& patchName
)
{
    if (done_)
    {
        FatalErrorInFunction
            << "refineLayers is already executed" << exit(FatalError);
    }

    const labelList matchedIDs = mesh_.findPatches(patchName);

    forAll(matchedIDs, matchI)
    {
        const word pName = mesh_.getPatchName(matchedIDs[matchI]);
        discontinuousLayersForPatch_.insert(pName);
    }
}


void Foam::Module::refineBoundaryLayers::setCellSubset(const word subsetName)
{
    if (done_)
    {
        FatalErrorInFunction
            << "refineLayers is already executed" << exit(FatalError);
    }

    cellSubsetName_ = subsetName;
}


void Foam::Module::refineBoundaryLayers::activateSpecialMode()
{
    specialMode_ = true;
}


void Foam::Module::refineBoundaryLayers::refineLayers()
{
    bool refinePatch(false);
    forAllConstIters(numLayersForPatch_, it)
    {
        if (it->second > 1)
        {
            refinePatch = true;
        }
    }

    if ((globalNumLayers_ < 2) && !refinePatch)
    {
        return;
    }

    Info<< "Starting refining boundary layers" << endl;

    if (done_)
    {
        WarningInFunction
            << "Boundary layers are already refined! "
            << "Stopping refinement" << endl;

        return;
    }

    if (!analyseLayers())
    {
        WarningInFunction
            << "Boundary layers do not exist in the mesh! Cannot refine"
            << endl;

        return;
    }

    generateNewVertices();

    generateNewFaces();

    generateNewCells();

    done_ = true;

    Info<< "Finished refining boundary layers" << endl;
}


void Foam::Module::refineBoundaryLayers::pointsInBndLayer
(
    labelLongList& layerPoints
)
{
    layerPoints.clear();

    boolList pointInLayer(mesh_.points().size(), false);

    forAll(newVerticesForSplitEdge_, seI)
    {
        forAllRow(newVerticesForSplitEdge_, seI, i)
        {
            pointInLayer[newVerticesForSplitEdge_(seI, i)] = true;
        }
    }

    forAll(pointInLayer, pointI)
    {
        if (pointInLayer[pointI])
        {
            layerPoints.append(pointI);
        }
    }

}


void Foam::Module::refineBoundaryLayers::pointsInBndLayer(const word subsetName)
{
    label sId = mesh_.pointSubsetIndex(subsetName);
    if (sId < 0)
    {
        sId = mesh_.addPointSubset(subsetName);
    }

    forAll(newVerticesForSplitEdge_, seI)
    {
        forAllRow(newVerticesForSplitEdge_, seI, i)
        {
            mesh_.addPointToSubset(sId, newVerticesForSplitEdge_(seI, i));
        }
    }
}


void Foam::Module::refineBoundaryLayers::readSettings
(
    const dictionary& meshDict,
    refineBoundaryLayers& refLayers
)
{
    if (meshDict.isDict("boundaryLayers"))
    {
        const dictionary& bndLayers = meshDict.subDict("boundaryLayers");

        // read global properties
        {
            label nLayers;
            if (bndLayers.readIfPresent("nLayers", nLayers))
            {
                refLayers.setGlobalNumberOfLayers(nLayers);
            }
        }

        {
            scalar ratio;
            if (bndLayers.readIfPresent("thicknessRatio", ratio))
            {
                refLayers.setGlobalThicknessRatio(ratio);
            }
        }

        {
            scalar thick;
            if (bndLayers.readIfPresent("maxFirstLayerThickness", thick))
            {
                refLayers.setGlobalMaxThicknessOfFirstLayer(thick);
            }
        }

        // consider specified patches for exclusion from boundary layer creation
        // implemented by setting the number of layers to 1
        if (bndLayers.found("excludedPatches"))
        {
            const wordList patchNames(bndLayers.lookup("excludedPatches"));

            forAll(patchNames, patchI)
            {
                const word pName = patchNames[patchI];

                refLayers.setNumberOfLayersForPatch(pName, 1);
            }
        }

        // patch-based properties
        if (bndLayers.isDict("patchBoundaryLayers"))
        {
            const dictionary& patchBndLayers =
                bndLayers.subDict("patchBoundaryLayers");
            const wordList patchNames = patchBndLayers.toc();

            forAll(patchNames, patchI)
            {
                const word pName = patchNames[patchI];

                if (patchBndLayers.isDict(pName))
                {
                    const dictionary& patchDict =
                        patchBndLayers.subDict(pName);

                    label nLayers;
                    if (patchDict.readIfPresent("nLayers", nLayers))
                    {
                        refLayers.setNumberOfLayersForPatch(pName, nLayers);
                    }

                    scalar ratio;
                    if (patchDict.readIfPresent("thicknessRatio", ratio))
                    {
                        refLayers.setThicknessRatioForPatch(pName, ratio);
                    }

                    scalar thick;
                    if
                    (
                        patchDict.readIfPresent
                        (
                            "maxFirstLayerThickness",
                            thick
                        )
                    )
                    {
                        refLayers.setMaxThicknessOfFirstLayerForPatch
                        (
                            pName,
                            thick
                        );
                    }

                    if
                    (
                        patchDict.lookupOrDefault<bool>
                        (
                            "allowDiscontinuity",
                            false
                        )
                    )
                    {
                        refLayers.setInteruptForPatch(pName);
                    }
                }
                else
                {
                    Warning << "Cannot refine layer for patch "
                        << patchNames[patchI] << endl;
                }
            }
        }
    }
    else
    {
        // the layer will not be refined
        refLayers.avoidRefinement();
    }
}


// ************************************************************************* //
