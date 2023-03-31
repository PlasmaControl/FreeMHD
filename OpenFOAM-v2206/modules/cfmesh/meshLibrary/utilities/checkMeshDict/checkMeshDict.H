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
    Foam::Module::checkMeshDict

Description
    Check whether the meshDict file is set correctly

SourceFiles
    checkMeshDict.C

\*---------------------------------------------------------------------------*/

#ifndef checkMeshDict_H
#define checkMeshDict_H

#include "IOdictionary.H"

#include <map>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
                        Class checkMeshDict Declaration
\*---------------------------------------------------------------------------*/

class checkMeshDict
{
    // Private data

        //- Reference to the mesh
        IOdictionary& meshDict_;


    // Private member functions

        //- check settings for cell size in meshDict
        void checkBasicSettings() const;

        //- check patchCellSize entry
        void checkPatchCellSize() const;

        //- check subsetCellSize entry
        void checkSubsetCellSize() const;

        //- check local refinement level
        void checkLocalRefinementLevel() const;

        //- check keepCellsIntersectingPatches entry
        void checkKeepCellsIntersectingPatches() const;

        //- check removeCellsIntersectingPatches entry
        void checkRemoveCellsIntersectingPatches() const;

        //- check objectRefinements entry
        void checkObjectRefinements() const;

        //- check anisotropic sources
        void checkAnisotropicSources() const;

        //- check surfaceRefinements entry
        void checkSurfaceRefinements() const;

        //- check edgeMeshRefinements entry
        void checkEdgeMeshRefinements() const;

        //- check entry for boundary layers
        void checkBoundaryLayers() const;

        //- check renameBoundary entry
        void checkRenameBoundary() const;

        //- check entry for mesh quality
        void checkQualitySettings() const;

        //- perform all checks
        void checkEntries() const;

        //- update patchCellSize entry
        void updatePatchCellSize(const std::map<word, wordList>&);

        //- update subsetCellSize entry
        void updateSubsetCellSize(const std::map<word, wordList>&);

        //- update local refinement
        void updateLocalRefinement(const std::map<word, wordList>&);

        //- check keepCellsIntersectingPatches entry
        void updateKeepCellsIntersectingPatches
        (
            const std::map<word, wordList>&
        );

        //- check removeCellsIntersectingPatches entry
        void updateRemoveCellsIntersectingPatches
        (
            const std::map<word, wordList>&
        );

        //- check objectRefinements entry
        void updateObjectRefinements(const std::map<word, wordList>&);

        //- check entry for boundary layers
        void updateBoundaryLayers(const std::map<word, wordList>&);

        //- check renameBoundary entry
        void updateRenameBoundary
        (
            const std::map<word, wordList>&,
            const std::map<word, word>&
        );


public:

    //- Construct from IOdictionary
    checkMeshDict(IOdictionary& meshDict);

    //- Destructor
    ~checkMeshDict() = default;


    // Public member functions

        //- update meshDict based on modification of patches in the surface
        void updateDictionaries
        (
            const std::map<word, wordList>& patchesForPatch,
            const std::map<word, word>& patchTypes,
            const bool renamePatches = true
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
