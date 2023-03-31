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
    Foam::Module::polyMeshGenModifierAddCellByCell

Description
    Modifier for polyMeshGen

SourceFiles
    polyMeshGenModifierAddCellByCell.C

\*---------------------------------------------------------------------------*/

#ifndef polyMeshGenModifierAddCellByCell_H
#define polyMeshGenModifierAddCellByCell_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "polyMeshGenModifier.H"

namespace Foam
{
namespace Module
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class polyMeshGenModifierAddCellByCell
:
    public polyMeshGenModifier
{
    // Private data

        //- number of faces
        label nFaces_;
        LongList<face> newFaces_;

        //- number of cells
        label nCells_;
        LongList<cell> newCells_;


public:

    //- Construct from the reference to the mesh
    polyMeshGenModifierAddCellByCell(polyMeshGen& mesh);

    //- Destructor
    ~polyMeshGenModifierAddCellByCell();


    // Member functions

        //- add a single cell (vertices must be added);
        void addCell(const faceList& cell);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
