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
    Foam::Module::meshSurfaceEdgeExtractorNonTopo

Description
    Stores boundary faces into patches and captures edges and corners

SourceFiles
    meshSurfaceEdgeExtractorNonTopo.C

\*---------------------------------------------------------------------------*/

#ifndef meshSurfaceEdgeExtractorNonTopo_H
#define meshSurfaceEdgeExtractorNonTopo_H

#include "polyMeshGenModifier.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{
// Forward declarations
class meshOctree;

/*---------------------------------------------------------------------------*\
               Class meshSurfaceEdgeExtractorNonTopo Declaration
\*---------------------------------------------------------------------------*/

class meshSurfaceEdgeExtractorNonTopo
{
    // Private data

        //- reference to polyMeshGen
        polyMeshGen& mesh_;

        //- reference to meshOctree
        const meshOctree& meshOctree_;


    // Private member functions

        //- decompose faces with more than one edge at the same feature edge
        void decomposeBoundaryFaces();

        //- re-map points after edges have been extracted
        void remapBoundaryPoints();

        //- Disallow default bitwise copy construct
        meshSurfaceEdgeExtractorNonTopo(const meshSurfaceEdgeExtractorNonTopo&);

        //- Disallow default bitwise assignment
        void operator=(const meshSurfaceEdgeExtractorNonTopo&);


public:

    //- Construct from mesh data
    meshSurfaceEdgeExtractorNonTopo
    (
        polyMeshGen& mesh,
        const meshOctree& octree
    );

    //- Destructor
    ~meshSurfaceEdgeExtractorNonTopo() = default;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
