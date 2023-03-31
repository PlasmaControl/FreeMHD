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

#include "voronoiMeshExtractor.H"
#include "demandDrivenData.H"

// #define DEBUGTets

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::label Foam::Module::voronoiMeshExtractor::sameOrientation_[6]
    = {3, 1, 2, 2, 3, 0};


Foam::label Foam::Module::voronoiMeshExtractor::oppositeOrientation_[6]
    = {2, 3, 1, 0, 0, 1};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::voronoiMeshExtractor::clearOut()
{
    deleteDemandDrivenData(pointEdgesPtr_);
    deleteDemandDrivenData(edgesPtr_);
    deleteDemandDrivenData(edgeTetsPtr_);
    deleteDemandDrivenData(boundaryEdgePtr_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::voronoiMeshExtractor::voronoiMeshExtractor
(
    const meshOctree& octree,
    const IOdictionary& meshDict,
    polyMeshGen& mesh
)
:
    tetCreator_(octree, meshDict),
    mesh_(mesh),
    pointEdgesPtr_(nullptr),
    edgesPtr_(nullptr),
    edgeTetsPtr_(nullptr),
    boundaryEdgePtr_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::Module::voronoiMeshExtractor::~voronoiMeshExtractor()
{
    clearOut();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::voronoiMeshExtractor::createMesh()
{
    Info<< "Extracting voronoi mesh" << endl;

    // copy tet points into the mesh
    createPoints();

    // create the mesh
    createPolyMesh();

    polyMeshGenModifier(mesh_).reorderBoundaryFaces();
    polyMeshGenModifier(mesh_).removeUnusedVertices();

    Info<< "Mesh has :" << nl
        << mesh_.points().size() << " vertices " << nl
        << mesh_.faces().size() << " faces" << nl
        << mesh_.cells().size() << " cells" << endl;

    Info<< "Finished extracting voronoi mesh" << endl;
}


// ************************************************************************* //
