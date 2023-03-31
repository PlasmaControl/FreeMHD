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
    Foam::Module::meshSurfaceCheckInvertedVertices

Description
    Checks if any of the surface is tangled at any of its vertices. This
    is performed by checking the dot product between the vertex normal and
    the normals of the faces attached to the vertex.

SourceFiles
    meshSurfaceCheckInvertedVertices.C

\*---------------------------------------------------------------------------*/

#ifndef meshSurfaceCheckInvertedVertices_H
#define meshSurfaceCheckInvertedVertices_H

#include "polyMeshGenModifier.H"
#include "HashSet.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

// Forward declarations
class meshSurfacePartitioner;

/*---------------------------------------------------------------------------*\
              Class meshSurfaceCheckInvertedVertices Declaration
\*---------------------------------------------------------------------------*/

class meshSurfaceCheckInvertedVertices
{
    // Private data

        //- mesh surface partitioner
        const meshSurfacePartitioner& surfacePartitioner_;

        //- active surface points
        const boolList* activePointsPtr_;

        //- set of inverted vertices
        labelHashSet invertedVertices_;


    // Private member functions

        //- check vertices by calculating dot products
        void checkVertices();

        //- Disallow default bitwise copy construct
        meshSurfaceCheckInvertedVertices
        (
            const meshSurfaceCheckInvertedVertices&
        );

        //- Disallow default bitwise assignment
        void operator=(const meshSurfaceCheckInvertedVertices&);


public:

    // Constructors

        //- Construct from meshSurfacePartitioner
        meshSurfaceCheckInvertedVertices
        (
            const meshSurfacePartitioner&
        );

        //- Construct from meshSurfacePartitioner and a list of active points
        meshSurfaceCheckInvertedVertices
        (
            const meshSurfacePartitioner&,
            const boolList& activePoints
        );

    //- Destructor
    ~meshSurfaceCheckInvertedVertices() = default;


    // Member Functions

        //- return the labels of inverted vertices
        inline const labelHashSet& invertedVertices() const
        {
            return invertedVertices_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
