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

#include "cartesianMeshExtractor.H"
#include "meshOctree.H"
#include "demandDrivenData.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::cartesianMeshExtractor::clearOut()
{
    deleteDemandDrivenData(leafCellLabelPtr_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::cartesianMeshExtractor::cartesianMeshExtractor
(
    meshOctree& octree,
    const IOdictionary& meshDict,
    polyMeshGen& mesh
)
:
    octreeCheck_(octree, meshDict, false),
    mesh_(mesh),
    decomposeSplitHexes_(false),
    leafCellLabelPtr_(new labelList(octree.numberOfLeaves(), -1))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::Module::cartesianMeshExtractor::~cartesianMeshExtractor()
{
    clearOut();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::cartesianMeshExtractor::decomposeSplitHexes()
{
    decomposeSplitHexes_ = true;
}


void Foam::Module::cartesianMeshExtractor::createMesh()
{
    Info<< "Extracting polyMesh" << endl;

    // create points and pointLeaves addressing
    createPointsAndAddressing();

    // create the mesh
    createPolyMesh();

    // decompose split-hex cells into tetrahedra and pyramids
    decomposeSplitHexesIntoTetsAndPyramids();

    // remove unused vertices
    polyMeshGenModifier(mesh_).removeUnusedVertices();

    Info<< "Mesh has :" << nl
        << mesh_.points().size() << " vertices " << nl
        << mesh_.faces().size() << " faces" << nl
        << mesh_.cells().size() << " cells" << endl;

    if (Pstream::parRun())
    {
        label nCells = mesh_.cells().size();
        reduce(nCells, sumOp<label>());
        Info<< "Total number of cells " << nCells << endl;
    }
    if (mesh_.cells().size() == 0)
    {
        FatalErrorInFunction
            << "There are no cells in the mesh!"
            << nl << "The reasons for this can be fwofold:"
            << nl << "1. Inadequate mesh resolution."
            << nl << "2. You maxCellSize is a multiplier of the domain length."
            << " This can be reolved by reducing the maxCellSize by a fraction."
            << "i.e. 2.49999 instead of 2.5." << exit(FatalError);
    }

    Info<< "Finished extracting polyMesh" << endl;
}


// ************************************************************************* //
