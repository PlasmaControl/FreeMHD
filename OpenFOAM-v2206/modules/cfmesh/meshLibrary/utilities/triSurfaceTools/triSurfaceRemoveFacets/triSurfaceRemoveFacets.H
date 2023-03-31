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
    Foam::Module::triSurfaceRemoveFacets

Description
    Divides the surface mesh into regions bounded by feature edges

SourceFiles
    triSurfaceRemoveFacets.C
    triSurfaceRemoveFacetsFunctions.C

\*---------------------------------------------------------------------------*/

#ifndef triSurfaceRemoveFacets_H
#define triSurfaceRemoveFacets_H

#include "triSurf.H"
#include "boolList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{
/*---------------------------------------------------------------------------*\
                   Class triSurfaceRemoveFacets Declaration
\*---------------------------------------------------------------------------*/

class triSurfaceRemoveFacets
{
    // Private data

        //- reference to triSurf
        triSurf& surf_;

        //- patches/subsets for removal
        DynList<word> selectedEntities_;


    // Private member functions

        //- remove facets in selected patches/subsets
        void markFacetsForRemoval(boolList&) const;

        //- Disallow default bitwise copy construct
        triSurfaceRemoveFacets(const triSurfaceRemoveFacets&);

        //- Disallow default bitwise assignment
        void operator=(const triSurfaceRemoveFacets&);

public:

    //- Construct from octree
    triSurfaceRemoveFacets(triSurf& surface);

    //- Destructor
    ~triSurfaceRemoveFacets() = default;


    // Member Functions

        //- add patch for removal
        void selectFacetsInPatch(const word&);

        //- add subsets for removal
        void selectFacetsInSubset(const word&);

        //- perform removal of selected facets
        void removeFacets();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
