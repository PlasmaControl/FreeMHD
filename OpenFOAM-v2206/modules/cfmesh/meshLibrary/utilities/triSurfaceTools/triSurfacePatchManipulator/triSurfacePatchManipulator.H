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
    Foam::Module::triSurfacePatchManipulator

Description
    Generates patches in the surface mesh based
    on the user-selected feature edges

SourceFiles
    triSurfacePatchManipulator.C
    triSurfacePatchManipulatorFunctions.C

\*---------------------------------------------------------------------------*/

#ifndef triSurfacePatchManipulator_H
#define triSurfacePatchManipulator_H

#include "IOdictionary.H"
#include "triSurf.H"
#include "VRWGraph.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
                 Class triSurfacePatchManipulator Declaration
\*---------------------------------------------------------------------------*/

class triSurfacePatchManipulator
{
    // Private data

        //- const reference to triSurf
        const triSurf& surf_;

        //- detected feature edges
        List<direction> featureEdges_;

        //- surface patches
        labelList facetInPatch_;

        //- number of patches
        label nPatches_;

        //- patch names
        wordList newPatchNames_;

        //- patch types
        wordList newPatchTypes_;


    // Private member functions

        //- allocate and fill the feature edges list
        void allocateFeatureEdges();

        //- create patches bounded by a set of feature edges
        void createPatches();

        //- Disallow default bitwise copy construct
        triSurfacePatchManipulator(const triSurfacePatchManipulator&);

        //- Disallow default bitwise assignment
        void operator=(const triSurfacePatchManipulator&);


public:

    //- Construct from triSurface
    triSurfacePatchManipulator(const triSurf& surface);

    //- Destructor
    ~triSurfacePatchManipulator() = default;


    // Member Functions

        void detectedSurfaceRegions(VRWGraph&) const;

        //- store regions into subsets with a given prefix
        const triSurf* surfaceWithPatches
        (
            IOdictionary* meshDictPtr = nullptr,
            const word prefix = "patch_",
            const bool forceOverwrite = false
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
