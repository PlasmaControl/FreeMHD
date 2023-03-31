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
    Foam::Module::triSurf

Description
    A class for triangulated surface used in the meshing process.
    It is derived from points and facets with some additional subsets.
    Subsets are very useful for local mesh refinement.

SourceFiles
    triSurf.C

\*---------------------------------------------------------------------------*/

#ifndef triSurf_H
#define triSurf_H

#include "triSurfPoints.H"
#include "triSurfFacets.H"
#include "triSurfFeatureEdges.H"
#include "triSurfAddressing.H"
#include <map>
#include "DynList.H"
#include "labelLongList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

// Froward declarations
class triSurfModifier;

/*---------------------------------------------------------------------------*\
                           Class triSurf Declaration
\*---------------------------------------------------------------------------*/

class triSurf
:
    public triSurfPoints,
    public triSurfFacets,
    public triSurfFeatureEdges,
    public triSurfAddressing
{
    // Private member functions

        void readFromFTR(const fileName&);
        void writeToFTR(const fileName&) const;

        void readFromFMS(const fileName&);
        void writeToFMS(const fileName&) const;

        inline LongList<labelledTri>& accessToFacets();
        inline geometricSurfacePatchList& accessToPatches();

        void topologyCheck();

        //- Disallow default bitwise assignment
        void operator=(const triSurf&);


public:

    // Friend classes

        friend class triSurfModifer;


    // Constructors

        //- Default construct
        triSurf();

        //- Construct from parts
        triSurf
        (
            const LongList<labelledTri>& triangles,
            const geometricSurfacePatchList& patches,
            const edgeLongList& featureEdges_,
            const pointField& points
        );

        //- Read from file
        triSurf(const fileName& fName);


    //- Destructor
    ~triSurf() = default;


    // Member Functions

        //- read and write the surface
        void readSurface(const fileName&);
        void writeSurface(const fileName&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "triSurfI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
