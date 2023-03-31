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

void Foam::Module::polyMeshGenAddressing::calcPointEdges() const
{
    if (pePtr_)
    {
        FatalErrorInFunction
            << "pointEdges already calculated"
            << abort(FatalError);
    }
    else
    {
        const edgeList& e = edges();

        // create the addressing
        pePtr_ = new VRWGraph();
        VRWGraph& pointEdgeAddr = *pePtr_;

        VRWGraphSMPModifier(pointEdgeAddr).reverseAddressing(e);
        pointEdgeAddr.setSize(mesh_.points().size());
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::Module::VRWGraph&
Foam::Module::polyMeshGenAddressing::pointEdges() const
{
    if (!pePtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
        {
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        }
        # endif

        calcPointEdges();
    }

    return *pePtr_;
}


// ************************************************************************* //
