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
    Foam::Module::detectBoundaryLayers

Description
    Check the cells attached to the surface of the volume mesh and check
    the existence of boundary layers.

    It also provides information how boundary layers are connected over
    several patches.

SourceFiles
    detectBoundaryLayers.C
    detectBoundaryLayersFunctions.C

\*---------------------------------------------------------------------------*/

#ifndef detectBoundaryLayers_H
#define detectBoundaryLayers_H

#include "polyMeshGenModifier.H"
#include "meshSurfaceEngine.H"
#include "DynList.H"
#include "labelLongList.H"
#include "labelPair.H"
#include "edgeLongList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

// Forward declarations
class meshSurfacePartitioner;

/*---------------------------------------------------------------------------*\
                    Class detectBoundaryLayers Declaration
\*---------------------------------------------------------------------------*/

class detectBoundaryLayers
{
    // Private data

        //- Reference to the meshSurfacePartitioner
        const meshSurfacePartitioner& meshSurface_;

        //- find the number of distinct layers in the mesh
        //- at the boundary of the mesh
        label nFirstLayers_;

        //- information about the existing boundary layer at a boundary face
        labelList layerAtBndFace_;

        //- layer at a boundary patch
        List<DynList<label>> layerAtPatch_;

        //- hair edges found in the mesh
        edgeLongList hairEdges_;

        //- hair edges at a boundary point
        VRWGraph hairEdgesAtBoundaryPoint_;

        //- is it a 2D  mesh
        const bool is2DMesh_;


    // Private member functions

        //- analyse layers to check their topology
        void analyseLayers();

        //- provide hair edges in a cell above a boundary face
        bool findHairsForFace(const label, DynList<edge>& hairEdges) const;

        //- generate hair edges for all boundary points
        void generateHairEdges();

        //- Disallow bitwise copy construct
        detectBoundaryLayers(const detectBoundaryLayers&);

        //- Disallow bitwise assignment
        void operator=(const detectBoundaryLayers&);


public:

    //- Construct from meshSurfacePartitioner and a bool providing
    //- information whether the mesh is a 2D mesh. The default is false.
    detectBoundaryLayers
    (
        const meshSurfacePartitioner& meshSurface,
        const bool is2DMesh = false
    );

    //- Destructor
    ~detectBoundaryLayers() = default;


    // Public member functions

        //- return hair edges found in the detection process
        inline const edgeLongList& hairEdges() const
        {
            return hairEdges_;
        }

        //- hair edges attached to a boundary point
        inline const VRWGraph& hairEdgesAtBndPoint() const
        {
            return hairEdgesAtBoundaryPoint_;
        }

        //- number of distinct layers which are at the boundary of the mesh
        inline label nDistinctLayers() const
        {
            return nFirstLayers_;
        }

        //- index of a layer to which a boundary faces belong to
        inline const labelList& faceInLayer() const
        {
            return layerAtBndFace_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
