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
    Foam::Module::decomposeCells

Description
    Decomposes selected cells into pyramids

SourceFiles
    decomposeCells.C
    decomposeCellsDecomposition.C
    decomposeCellsPyramids.C

\*---------------------------------------------------------------------------*/

#ifndef decomposeCells_H
#define decomposeCells_H

#include "polyMeshGenModifier.H"
#include "VRWGraphList.H"
#include "DynList.H"

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
                       Class decomposeCells Declaration
\*---------------------------------------------------------------------------*/

class decomposeCells
{
    // private data

        //- reference to the mesh
        polyMeshGen& mesh_;

        //- new boundary faces
        wordList patchNames_;
        wordList patchTypes_;
        VRWGraph newBoundaryFaces_;
        labelLongList newBoundaryPatches_;

        VRWGraphList facesOfNewCells_;


    // Private member functions

        //- check if the valid pyramids are generated from the split cells
        //- this check splits faces which could
        //- result in multiple inbetween faces
        void checkFaceConnections(const boolList& decomposeCell);

        //- create addressing needed to decompose the cell
        void findAddressingForCell
        (
            const label cellI,
            DynList<label, 32>& vrt,
            DynList<edge, 64>& edges,
            DynList<DynList<label, 8>>& faceEdges,
            DynList<DynList<label, 2>, 64>& edgeFaces
        ) const;

        //- find the apex of the pyramids
        label findTopVertex
        (
            const label cellI,
            const DynList<label, 32>& vrt,
            const DynList<edge, 64>& edges,
            const DynList<DynList<label, 2>, 64>& edgeFaces
        );

        void decomposeCellIntoPyramids(const label cellI);

        void createPointsAndCellFaces(const boolList& decomposeCell);

        void storeBoundaryFaces(const boolList& decomposeCell);

        void removeDecomposedCells(const boolList& decomposeCell);

        void addNewCells();

        //- disallows bitwise construct
        void operator=(const decomposeCells&);

        //- copy constructor
        decomposeCells(const decomposeCells&);


public:

    //- construct from polyMeshGen and a list containing patches
    //- for each point
    decomposeCells(polyMeshGen& mesh);

    //- Destructor
    ~decomposeCells() = default;


    // Member functions

        //- perform decomposition of selected cell into pyramids
        void decomposeMesh(const boolList&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
