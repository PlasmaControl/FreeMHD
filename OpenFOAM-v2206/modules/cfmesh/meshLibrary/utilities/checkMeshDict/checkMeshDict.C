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

#include "checkMeshDict.H"
#include "patchRefinementList.H"
#include "PtrList.H"
#include "LongList.H"
#include "objectRefinement.H"
#include "coordinateModification.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::checkMeshDict::checkBasicSettings() const
{
    bool bval;
    scalar sval;

    // Check if maxCellSize is valid
    sval = readScalar(meshDict_.lookup("maxCellSize"));
    if (sval < 0)
    {
        FatalErrorInFunction
            << "maxCellSize is negative! Cannot generate the mesh!!"
            << exit(FatalError);
    }

    // Check if boundaryCellSize makes sense
    if (meshDict_.readIfPresent("boundaryCellSize", sval))
    {
        if (sval < 0)
        {
            WarningInFunction
                << "Boundary cell size is negative!!" << endl;
        }

        if
        (
            meshDict_.readIfPresent
            (
                "boundaryCellSizeRefinementThickness",
                sval
            )
         && (sval < 0)
        )
        {
            WarningInFunction
                << "Boundary cell size refinement thickness is negative!!"
                << endl;
        }
    }

    // Check if minCellSize is valid
    if
    (
        meshDict_.readIfPresent("minCellSize", sval)
     && (sval < 0)
    )
    {
        FatalErrorInFunction
            << "Minimum cell size for automatic refinement is negative!!"
            << exit(FatalError);
    }

    // Check if keepCellsIntersectingBoundary can be read correctly
    meshDict_.readIfPresent("keepCellsIntersectingBoundary", bval);
    meshDict_.readIfPresent("checkForGluedMesh", bval);

    // Check if enforceConstraints is available
    meshDict_.readIfPresent("enforceConstraints", bval);
}


void Foam::Module::checkMeshDict::checkPatchCellSize() const
{
    if (meshDict_.found("patchCellSize"))
    {
        if (meshDict_.isDict("patchCellSize"))
        {
            const dictionary& dict = meshDict_.subDict("patchCellSize");

            const wordList patchNames = dict.toc();
            patchNames.size();
        }
        else
        {
            patchRefinementList prl(meshDict_.lookup("patchCellSize"));
            prl.size();
        }
    }
}


void Foam::Module::checkMeshDict::checkSubsetCellSize() const
{
    if (meshDict_.found("subsetCellSize"))
    {
        if (meshDict_.isDict("subsetCellSize"))
        {
            const dictionary& dict = meshDict_.subDict("subsetCellSize");

            const wordList subsetNames = dict.toc();
            subsetNames.size();
        }
        else
        {
            patchRefinementList prl(meshDict_.lookup("patchCellSize"));
        }
    }
}


void Foam::Module::checkMeshDict::checkLocalRefinementLevel() const
{
    if (meshDict_.found("localRefinement"))
    {
        if (meshDict_.isDict("localRefinement"))
        {
            const dictionary& refDict = meshDict_.subDict("localRefinement");
            const wordList entries = refDict.toc();

            forAll(entries, dictI)
            {
                const dictionary& dict = refDict.subDict(entries[dictI]);

                label ival;
                scalar sval;

                if (dict.readIfPresent("cellSize", sval))
                {
                    if (sval < 0.0)
                    {
                        WarningInFunction
                            << "Cell size for " << entries[dictI]
                            << " is negative" << endl;
                    }
                }
                else if (dict.readIfPresent("additionalRefinementLevels", ival))
                {
                    if (ival < 0)
                    {
                        WarningInFunction
                            << "Refinement level for " << entries[dictI]
                            << " is negative" << endl;
                    }
                }
                else
                {
                    FatalErrorInFunction
                        << "Cannot read keyword"
                        << " additionalRefinementLevels or cellSize"
                        << "for " << entries[dictI] << exit(FatalError);
                }

                if (dict.readIfPresent("refinementThickness", sval))
                {
                    if (sval < 0)
                    {
                        WarningInFunction
                            << "Refinement thickness for " << entries[dictI]
                            << " is negative" << endl;
                    }
                }
            }
        }
        else
        {
            FatalErrorInFunction
                << "Cannot read localRefinement" << exit(FatalError);
        }
    }
}


void Foam::Module::checkMeshDict::checkKeepCellsIntersectingPatches() const
{
    if (meshDict_.found("keepCellsIntersectingPatches"))
    {
        if (meshDict_.isDict("keepCellsIntersectingPatches"))
        {
            const dictionary& dict =
                meshDict_.subDict("keepCellsIntersectingPatches");

            const wordList patchNames = dict.toc();
            patchNames.size();
        }
        else
        {
            wordList kcip(meshDict_.lookup("keepCellsIntersectingPatches"));
        }
    }
}


void Foam::Module::checkMeshDict::checkRemoveCellsIntersectingPatches() const
{
    if (meshDict_.found("removeCellsIntersectingPatches"))
    {
        if (meshDict_.isDict("removeCellsIntersectingPatches"))
        {
            const dictionary& dict =
                meshDict_.subDict("removeCellsIntersectingPatches");

            const wordList patchNames = dict.toc();
            patchNames.size();
        }
        else
        {
            wordList kcip(meshDict_.lookup("removeCellsIntersectingPatches"));
        }
    }
}


void Foam::Module::checkMeshDict::checkObjectRefinements() const
{
    if (meshDict_.found("objectRefinements"))
    {
        PtrList<objectRefinement> refObjects;

        if (meshDict_.isDict("objectRefinements"))
        {
            const dictionary& dict = meshDict_.subDict("objectRefinements");
            const wordList objectNames = dict.toc();

            refObjects.setSize(objectNames.size());

            forAll(refObjects, objectI)
            {
                const entry& objectEntry =
                    dict.lookupEntry(objectNames[objectI], keyType::LITERAL);

                refObjects.set
                (
                    objectI,
                    objectRefinement::New
                    (
                        objectEntry.keyword(),
                        objectEntry.dict()
                    )
                );
            }
        }
        else
        {
            Istream& is = meshDict_.lookup("objectRefinements");

            PtrList<entry> objectEntries(is);
            refObjects.setSize(objectEntries.size());

            forAll(refObjects, objectI)
            {
                refObjects.set
                (
                    objectI,
                    objectRefinement::New
                    (
                        objectEntries[objectI].keyword(),
                        objectEntries[objectI].dict()
                    )
                );
            }
        }

        forAll(refObjects, oI)
        {
            if (refObjects[oI].refinementThickness() < 0.0)
            {
                WarningInFunction
                    << "Refinement thickness specified for object "
                    << refObjects[oI].name() << " is negative!!" << endl;
            }
        }
    }
}


void Foam::Module::checkMeshDict::checkAnisotropicSources() const
{
    if (meshDict_.found("anisotropicSources"))
    {
        PtrList<coordinateModification> anisotropicObjects;

        if (meshDict_.isDict("anisotropicSources"))
        {
            const dictionary& dict = meshDict_.subDict("anisotropicSources");
            const wordList objectNames = dict.toc();

            anisotropicObjects.setSize(objectNames.size());

            forAll(anisotropicObjects, objectI)
            {
                const entry& objectEntry =
                    dict.lookupEntry(objectNames[objectI], keyType::LITERAL);

                anisotropicObjects.set
                (
                    objectI,
                    coordinateModification::New
                    (
                        objectEntry.keyword(),
                        objectEntry.dict()
                    )
                );
            }
        }
    }
}


void Foam::Module::checkMeshDict::checkSurfaceRefinements() const
{
    if (meshDict_.found("surfaceMeshRefinement"))
    {
        const dictionary& surfaces = meshDict_.subDict("surfaceMeshRefinement");

        const wordList surfaceSources = surfaces.toc();

        forAll(surfaceSources, surfI)
        {
            if (surfaces.isDict(surfaceSources[surfI]))
            {
                const dictionary& dict =
                    surfaces.subDict(surfaceSources[surfI]);

                if (dict.found("surfaceFile"))
                {
                    const fileName fName(dict.lookup("surfaceFile"));

                    if (!isFile(fName))
                    {
                        FatalErrorInFunction
                            << "Surface file " << fName
                            << " does not exist or is not readable!!"
                            << exit(FatalError);
                    }
                }
                else
                {
                    FatalErrorInFunction
                        << "Missing surfaceFile for entry "
                        << surfaceSources[surfI] << exit(FatalError);
                }

                label ival;
                scalar sval;

                if (dict.readIfPresent("cellSize", sval))
                {
                    if (sval < VSMALL)
                    {
                        FatalErrorInFunction
                            << "Cell size for entry " << surfaceSources[surfI]
                            << " is extremely small or negative!!"
                            << exit(FatalError);
                    }
                }
                else if (dict.readIfPresent("additionalRefinementLevels", ival))
                {
                    if (ival < 0)
                    {
                        FatalErrorInFunction
                            << "Number refinement levels for entry "
                            << surfaceSources[surfI] << " is negative!!"
                            << exit(FatalError);
                    }
                }
                else
                {
                    FatalErrorInFunction
                        << "Missing cellSize or additionalRefinementLevels"
                        << " for entry " << surfaceSources[surfI]
                        << exit(FatalError);
                }

                if (dict.readIfPresent("refinementThickness", sval))
                {
                    if (sval < VSMALL)
                    {
                        WarningInFunction
                            << "Refinement thickness for entry "
                            << surfaceSources[surfI]
                            << " is extremely small or negative!!" << endl;
                    }
                }
            }
            else
            {
                FatalErrorInFunction
                    << "Dictionary " << surfaceSources[surfI]
                    << " does not exist!!"
                    << exit(FatalError);
            }
        }
    }
}


void Foam::Module::checkMeshDict::checkEdgeMeshRefinements() const
{
    if (meshDict_.found("edgeMeshRefinement"))
    {
        const dictionary& edgeMeshes = meshDict_.subDict("edgeMeshRefinement");

        const wordList edgeMeshSources = edgeMeshes.toc();

        forAll(edgeMeshSources, emI)
        {
            if (edgeMeshes.isDict(edgeMeshSources[emI]))
            {
                const dictionary& dict =
                    edgeMeshes.subDict(edgeMeshSources[emI]);

                if (dict.found("edgeFile"))
                {
                    const fileName fName(dict.lookup("edgeFile"));

                    if (!isFile(fName))
                    {
                        FatalErrorInFunction
                            << "Edge mesh file " << fName
                            << " does not exist or is not readable!!"
                            << exit(FatalError);
                    }
                }
                else
                {
                    FatalErrorInFunction
                        << "Missing edgeFilw for entry "
                        << edgeMeshSources[emI] << exit(FatalError);
                }

                scalar sval;
                label ival;

                if (dict.readIfPresent("cellSize", sval))
                {
                    if (sval < VSMALL)
                    {
                        FatalErrorInFunction
                            << "Cell size for entry " << edgeMeshSources[emI]
                            << " is extremely small or negative!!"
                            << exit(FatalError);
                    }
                }
                else if (dict.readIfPresent("additionalRefinementLevels", ival))
                {
                    if (ival < 0)
                    {
                        FatalErrorInFunction
                            << "Number refinement levels for entry "
                            << edgeMeshSources[emI] << " is negative!!"
                            << exit(FatalError);
                    }
                }
                else
                {
                    FatalErrorInFunction
                        << "Missing cellSize or additionalRefinementLevels"
                        << " for entry " << edgeMeshSources[emI]
                        << exit(FatalError);
                }

                if (dict.readIfPresent("refinementThickness", sval))
                {
                    if (sval < VSMALL)
                    {
                        WarningInFunction
                            << "Refinement thickness for entry "
                            << edgeMeshSources[emI]
                            << " is extremely small or negative!!" << endl;
                    }
                }
            }
            else
            {
                FatalErrorInFunction
                    << "Dictionary " << edgeMeshSources[emI]
                    << " does not exist!!"
                    << exit(FatalError);
            }
        }
    }
}


void Foam::Module::checkMeshDict::checkBoundaryLayers() const
{
    if (meshDict_.found("boundaryLayers"))
    {
        const dictionary& bndLayers = meshDict_.subDict("boundaryLayers");

        bool bval;
        label ival;
        scalar sval;

        // Check global properties
        bndLayers.readIfPresent<label>("nLayers", ival);
        bndLayers.readIfPresent<scalar>("thicknessRatio", sval);
        bndLayers.readIfPresent<scalar>("maxFirstLayerThickness", sval);

        // Patch-based properties
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

                    patchDict.readIfPresent<label>("nLayers", ival);
                    patchDict.readIfPresent("thicknessRatio", sval);
                    patchDict.readIfPresent("maxFirstLayerThickness", sval);
                    patchDict.readIfPresent<bool>("allowDiscontinuity", bval);
                }
                else
                {
                    Warning << "Cannot refine layer for patch "
                        << patchNames[patchI] << endl;
                }
            }
        }

        // Check for existence of boundary layer smoothing
        bndLayers.readIfPresent<bool>("optimiseLayer", bval);

        if (bndLayers.found("optimisationParameters"))
        {
            const dictionary& optParams =
                bndLayers.subDict("optimisationParameters");

            optParams.readIfPresent<bool>("reCalculateNormals", bval);

            if (optParams.readIfPresent("nSmoothNormals", ival))
            {
                if (ival < 0)
                {
                    FatalErrorInFunction
                        << "nSmoothNormals must not be negative!"
                        << exit(FatalError);
                }
            }

            if (optParams.readIfPresent("featureSizeFactor", sval))
            {
                if (sval >= 1.0 || sval < 0.0 )
                {
                    FatalErrorInFunction
                        << "Feature size factor is out"
                        << " of a valid range 0 to 1" << exit(FatalError);
                }
            }

            if (optParams.readIfPresent("relThicknessTol", sval))
            {
                if (sval >= 1.0 || sval < 0.0)
                {
                    FatalErrorInFunction
                        << "Relative thickness tolerance is out"
                        << " of a valid range 0 to 1" << exit(FatalError);
                }
            }

            if (optParams.readIfPresent("maxNumIterations", ival))
            {
                if (ival < 0)
                {
                    FatalErrorInFunction
                        << "maxNumIterations must not be negative!"
                        << exit(FatalError);
                }
            }
        }
    }
}


void Foam::Module::checkMeshDict::checkRenameBoundary() const
{
    if (meshDict_.found("renameBoundary"))
    {
        const dictionary& dict = meshDict_.subDict("renameBoundary");
        if (dict.found("newPatchNames"))
        {
            if (dict.isDict("newPatchNames"))
            {
                const dictionary& patchDicts = dict.subDict("newPatchNames");

                const wordList patchNames = patchDicts.toc();

                forAll(patchNames, patchI)
                {
                    const word& pName = patchNames[patchI];

                    if (!patchDicts.isDict(pName))
                    {
                        FatalErrorInFunction
                            << "Entry " << pName
                            << " is not a dictionary" << exit(FatalError);
                    }

                    const dictionary dict = patchDicts.subDict(pName);

                    if (!dict.found("newName"))
                    {
                        FatalErrorInFunction
                            << "Dictionary " << pName
                            << " does not contain a newName keyword"
                            << exit(FatalError);
                    }
                }
            }
            else
            {
                const PtrList<entry> patchesToRename
                (
                    dict.lookup("newPatchNames")
                );

                forAll(patchesToRename, patchI)
                {
                    const word& pName = patchesToRename[patchI].keyword();

                    const dictionary dict = patchesToRename[patchI].dict();

                    if (!dict.found("newName"))
                    {
                        FatalErrorInFunction
                            << "Dictionary " << pName
                            << " does not contain a newName keyword"
                            << exit(FatalError);
                    }
                }
            }
        }
    }
}


void Foam::Module::checkMeshDict::checkQualitySettings() const
{
    scalar sval;

    if (meshDict_.found("meshQualitySettings") )
    {
        const dictionary& qualityDict =
            meshDict_.subDict("meshQualitySettings");

        // maximum non-orthogonality defined by the user
        qualityDict.readIfPresent("maxNonOrthogonality", sval);

        // maximum skewness defined by the user
        qualityDict.readIfPresent("maxSkewness", sval);

        // minimum volume of the face pyramid defined by the user
        qualityDict.readIfPresent("minPyramidVolume", sval);

        // face flatness defined by the user
        qualityDict.readIfPresent("faceFlatness", sval);

        // minimum tetrahedral part of a cell defined by the user
        qualityDict.readIfPresent("minCellPartTetrahedra", sval);

        // minimum area of a face defined by the user
        qualityDict.readIfPresent("minimumFaceArea", sval);
    }
}


void Foam::Module::checkMeshDict::checkEntries() const
{
    checkBasicSettings();

    checkPatchCellSize();

    checkSubsetCellSize();

    checkSurfaceRefinements();

    checkKeepCellsIntersectingPatches();

    checkRemoveCellsIntersectingPatches();

    checkObjectRefinements();

    checkAnisotropicSources();

    checkBoundaryLayers();

    checkRenameBoundary();

    checkQualitySettings();
}


void Foam::Module::checkMeshDict::updatePatchCellSize
(
    const std::map<word, wordList>& patchesFromPatch
)
{
    if (meshDict_.found("patchCellSize"))
    {
        LongList<patchRefinement> updatedPatchRefinement;

        if (meshDict_.isDict("patchCellSize"))
        {
            const dictionary dict = meshDict_.subDict("patchCellSize");

            const wordList patchNames = dict.toc();

            forAll(patchNames, patchI)
            {
                const word& pName = patchNames[patchI];

                std::map<word, wordList>::const_iterator it =
                    patchesFromPatch.find(pName);
                if (it == patchesFromPatch.end())
                {
                    continue;
                }

                const wordList& updatedPatchNames = it->second;

                const dictionary& pDict = dict.subDict(pName);
                const scalar cellSize = readScalar(pDict.lookup("cellSize"));

                forAll(updatedPatchNames, nameI)
                {
                    updatedPatchRefinement.append
                    (
                        patchRefinement
                        (
                            updatedPatchNames[nameI],
                            cellSize
                        )
                    );
                }
            }
        }
        else
        {
            patchRefinementList prl(meshDict_.lookup("patchCellSize"));
            forAll(prl, prlI)
            {
                const word& pName = prl[prlI].patchName();
                const scalar cellSize = prl[prlI].cellSize();

                std::map<word, wordList>::const_iterator it =
                    patchesFromPatch.find(pName);

                if (it == patchesFromPatch.end())
                {
                    continue;
                }

                const wordList& updatedPatchNames = it->second;
                forAll(updatedPatchNames, nameI)
                {
                    updatedPatchRefinement.append
                    (
                        patchRefinement
                        (
                            updatedPatchNames[nameI],
                            cellSize
                        )
                    );
                }
            }
        }

        meshDict_.add("patchCellSize", updatedPatchRefinement, true);
    }
}


void Foam::Module::checkMeshDict::updateSubsetCellSize
(
    const std::map<word, wordList>& /*patchesFromPatch*/
)
{}


void Foam::Module::checkMeshDict::updateLocalRefinement
(
    const std::map<word, wordList>& patchesFromPatch
)
{
    if (meshDict_.found("localRefinement"))
    {
        if (meshDict_.isDict("localRefinement"))
        {
            dictionary& dict = meshDict_.subDict("localRefinement");

            const wordList entries = dict.toc();

            forAll(entries, dictI)
            {
                const word& pName = entries[dictI];

                std::map<word, wordList>::const_iterator it =
                    patchesFromPatch.find(pName);
                if (it == patchesFromPatch.end())
                {
                    continue;
                }

                const wordList& updatedPatchNames = it->second;

                const dictionary& pDict = dict.subDict(pName);
                dictionary copy = pDict;

                // add new patches
                forAll(updatedPatchNames, nameI)
                {
                    dict.add(updatedPatchNames[nameI], copy);
                }

                // remove the current patch
                dict.remove(pName);
            }
        }
    }
}


void Foam::Module::checkMeshDict::updateKeepCellsIntersectingPatches
(
    const std::map<word, wordList>& patchesFromPatch
)
{
    if (meshDict_.found("keepCellsIntersectingPatches"))
    {
        LongList<word> updatedPatchNames;
        if (meshDict_.isDict("keepCellsIntersectingPatches"))
        {
            const dictionary& dict =
                meshDict_.subDict("keepCellsIntersectingPatches");

            const wordList patchNames = dict.toc();
            forAll(patchNames, patchI)
            {
                const word& pName = patchNames[patchI];

                std::map<word, wordList>::const_iterator it =
                    patchesFromPatch.find(pName);

                if (it == patchesFromPatch.end())
                {
                    updatedPatchNames.append(pName);
                    continue;
                }

                const wordList& newPatchNames = it->second;

                forAll(newPatchNames, nameI)
                {
                    updatedPatchNames.append(newPatchNames[nameI]);
                }
            }
        }
        else
        {
            wordList kcip(meshDict_.lookup("keepCellsIntersectingPatches"));

            forAll(kcip, i)
            {
                const word& pName = kcip[i];

                std::map<word, wordList>::const_iterator it =
                    patchesFromPatch.find(pName);

                if (it == patchesFromPatch.end())
                {
                    updatedPatchNames.append(pName);
                }

                const wordList& newPatchNames = it->second;

                forAll(newPatchNames, nameI)
                {
                    updatedPatchNames.append(newPatchNames[nameI]);
                }
            }
        }

        meshDict_.add("keepCellsIntersectingPatches", updatedPatchNames, true);
    }
}


void Foam::Module::checkMeshDict::updateRemoveCellsIntersectingPatches
(
    const std::map<word, wordList>& patchesFromPatch
)
{
    if (meshDict_.found("removeCellsIntersectingPatches"))
    {
        LongList<word> updatedPatchNames;
        if (meshDict_.isDict("removeCellsIntersectingPatches"))
        {
            const dictionary& dict =
                meshDict_.subDict("removeCellsIntersectingPatches");

            const wordList patchNames = dict.toc();
            forAll(patchNames, patchI)
            {
                const word& pName = patchNames[patchI];

                std::map<word, wordList>::const_iterator it =
                    patchesFromPatch.find(pName);

                if (it == patchesFromPatch.end())
                {
                    updatedPatchNames.append(pName);
                    continue;
                }

                const wordList& newPatchNames = it->second;

                forAll(newPatchNames, nameI)
                {
                    updatedPatchNames.append(newPatchNames[nameI]);
                }
            }
        }
        else
        {
            wordList kcip(meshDict_.lookup("removeCellsIntersectingPatches"));

            forAll(kcip, i)
            {
                const word& pName = kcip[i];

                std::map<word, wordList>::const_iterator it =
                    patchesFromPatch.find(pName);

                if (it == patchesFromPatch.end())
                {
                    updatedPatchNames.append(pName);
                }

                const wordList& newPatchNames = it->second;

                forAll(newPatchNames, nameI)
                {
                    updatedPatchNames.append(newPatchNames[nameI]);
                }
            }
        }

        meshDict_.add
        (
            "removeCellsIntersectingPatches",
            updatedPatchNames,
            true
        );
    }
}


void Foam::Module::checkMeshDict::updateObjectRefinements
(
    const std::map<word, wordList>& /*patchesFromPatch*/
)
{}


void Foam::Module::checkMeshDict::updateBoundaryLayers
(
    const std::map<word, wordList>& patchesFromPatch
)
{
    if (meshDict_.isDict("boundaryLayers"))
    {
        dictionary& bndLayersDict = meshDict_.subDict("boundaryLayers");
        if (bndLayersDict.isDict("patchBoundaryLayers"))
        {
            dictionary& patchBndLayers =
                bndLayersDict.subDict("patchBoundaryLayers");

            const wordList patchLayers = patchBndLayers.toc();

            forAll(patchLayers, patchI)
            {
                const word& pName = patchLayers[patchI];

                dictionary dict = patchBndLayers.subDict(pName);

                const std::map<word, wordList>::const_iterator it =
                    patchesFromPatch.find(pName);

                // patch name may be a regex
                if (it != patchesFromPatch.end())
                {
                    const wordList& newNames = it->second;

                    forAll(newNames, i)
                    {
                        patchBndLayers.add(newNames[i], dict);
                    }

                    patchBndLayers.remove(pName);
                }
            }
        }
    }
}


void Foam::Module::checkMeshDict::updateRenameBoundary
(
    const std::map<word, wordList>& patchesFromPatch,
    const std::map<word, word>& patchTypes
)
{
    dictionary newDict;

    newDict.add("newPatchNames", dictionary());

    if (meshDict_.found("renameBoundary"))
    {
        const dictionary& dict = meshDict_.subDict("renameBoundary");

        // transfer or generate default name/type entries
        for (word keyName : { "defaultName", "defaultType" })
        {
            word wval;
            if (dict.readIfPresent(keyName, wval))
            {
                newDict.add(keyName, wval);
            }
        }

        if (dict.found("newPatchNames"))
        {
            // stores the updated dictionary
            dictionary& newPatchesDict = newDict.subDict("newPatchNames");

            if (dict.isDict("newPatchNames"))
            {
                // current state of the dictionary
                const dictionary& patchDicts = dict.subDict("newPatchNames");

                forAllConstIters(patchesFromPatch, it)
                {
                    const word& pName = it->first;
                    const wordList& newNames = it->second;

                    if (patchDicts.found(pName))
                    {
                        // patch renaming is already requested by the user
                        // use the new name for all newly created patches
                        const dictionary& patchDict = patchDicts.subDict(pName);
                        if (!patchDict.found("newName"))
                        {
                            continue;
                        }
                        if (!patchDict.found("type"))
                        {
                            continue;
                        }

                        const word newName(patchDict.lookup("newName"));
                        const word newType(patchDict.lookup("type"));

                        forAll(newNames, i)
                        {
                            dictionary newPatchDict;
                            newPatchDict.add("newName", newName);
                            newPatchDict.add("type", newType);

                            newPatchesDict.add(newNames[i], newPatchDict);
                        }
                    }
                    else
                    {
                        // rename all newly create patches
                        // with the original name
                        forAll(newNames, i)
                        {
                            dictionary newPatchDict;

                            newPatchDict.add("newName", it->first);
                            std::map<word, word>::const_iterator tIter =
                                patchTypes.find(it->first);
                            newPatchDict.add("type", tIter->second);

                            newPatchesDict.add(newNames[i], newPatchDict);
                        }
                    }
                }
            }
            else
            {
                const PtrList<entry> patchEntries(dict.lookup("newPatchNames"));

                forAll(patchEntries, entryI)
                {
                    const word& pName = patchEntries[entryI].keyword();
                    dictionary patchDict(patchEntries[entryI].dict());

                    std::map<word, wordList>::const_iterator it =
                        patchesFromPatch.find(pName);

                    if (it == patchesFromPatch.end())
                    {
                        continue;
                    }

                    const wordList& newNames = it->second;

                    forAll(newNames, i)
                    {
                        newPatchesDict.add(newNames[i], patchDict, true);
                    }
                }

                forAllConstIters(patchesFromPatch, it)
                {
                    const word& pName = it->first;
                    const wordList& newNames = it->second;

                    if (newPatchesDict.found(pName))
                    {
                        continue;
                    }

                    // rename all newly created patches
                    // with the original name
                    forAll(newNames, i)
                    {
                        dictionary newPatchDict;

                        newPatchDict.add("newName", it->first);
                        std::map<word, word>::const_iterator tIter =
                            patchTypes.find(it->first);
                        newPatchDict.add("type", tIter->second);

                        newPatchesDict.add(newNames[i], newPatchDict);
                    }
                }
            }
        }
        else
        {
            // newPatchNames is not used
            dictionary& newPatchesDict = newDict.subDict("newPatchNames");

            forAllConstIters(patchesFromPatch, it)
            {
                const wordList& newPatchNames = it->second;

                forAll(newPatchNames, i)
                {
                    const word& pName = newPatchNames[i];
                    dictionary newPatchDict;
                    newPatchDict.add("newName", it->first);
                    std::map<word, word>::const_iterator tIter =
                        patchTypes.find(it->first);
                    newPatchDict.add("type", tIter->second);

                    newPatchesDict.add(pName, newPatchDict);
                }
            }
        }

        // delete all previus entries from the dictionary
        meshDict_.subDict("renameBoundary").clear();
    }
    else
    {
        // create the dictionary if it has not existed before
        dictionary& newPatchesDict = newDict.subDict("newPatchNames");

        forAllConstIters(patchesFromPatch, it)
        {
            const wordList& newPatchNames = it->second;

            forAll(newPatchNames, i)
            {
                const word& pName = newPatchNames[i];
                dictionary newPatchDict;
                newPatchDict.add("newName", it->first);
                std::map<word, word>::const_iterator tIter =
                    patchTypes.find(it->first);
                newPatchDict.add("type", tIter->second);

                newPatchesDict.add(pName, newPatchDict);
            }
        }
    }

    meshDict_.add("renameBoundary", newDict, true);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::checkMeshDict::checkMeshDict
(
    IOdictionary& meshDict
)
:
    meshDict_(meshDict)
{
    checkEntries();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::checkMeshDict::updateDictionaries
(
    const std::map<word, wordList>& patchesFromPatch,
    const std::map<word, word>& patchTypes,
    const bool renamePatches
)
{
    updatePatchCellSize(patchesFromPatch);

    updateSubsetCellSize(patchesFromPatch);

    updateLocalRefinement(patchesFromPatch);

    updateKeepCellsIntersectingPatches(patchesFromPatch);

    updateRemoveCellsIntersectingPatches(patchesFromPatch);

    updateObjectRefinements(patchesFromPatch);

    updateBoundaryLayers(patchesFromPatch);

    if (renamePatches)
    {
        updateRenameBoundary(patchesFromPatch, patchTypes);
    }
}


// ************************************************************************* //
