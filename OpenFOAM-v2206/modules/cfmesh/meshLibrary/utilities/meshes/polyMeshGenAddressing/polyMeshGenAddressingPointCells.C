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

#include "polyMeshGenAddressing.H"
#include "VRWGraphSMPModifier.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::Module::polyMeshGenAddressing::calcPointCells() const
{
    if (pcPtr_)
    {
        FatalErrorInFunction
            << "pointCells already calculated"
            << abort(FatalError);
    }
    else
    {
        const VRWGraph& cellPoints = this->cellPoints();

        // create the storage
        pcPtr_ = new VRWGraph();
        VRWGraph& pointCellAddr = *pcPtr_;

        VRWGraphSMPModifier(pointCellAddr).reverseAddressing(cellPoints);
        pointCellAddr.setSize(mesh_.points().size());
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::Module::VRWGraph&
Foam::Module::polyMeshGenAddressing::pointCells() const
{
    if (!pcPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
        {
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        }
        # endif

        calcPointCells();
    }

    return *pcPtr_;
}


// ************************************************************************* //
