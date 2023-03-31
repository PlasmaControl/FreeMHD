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

Description
    An graph of faces which supports automated output.

\*---------------------------------------------------------------------------*/

#include "faceIOGraph.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::Module::faceIOGraph::faceIOGraph(const IOobject& io)
:
    regIOobject(io),
    VRWGraph()
{}


Foam::Module::faceIOGraph::faceIOGraph
(
    const IOobject& io,
    const label size
)
:
    regIOobject(io),
    VRWGraph(size)
{}


Foam::Module::faceIOGraph::faceIOGraph
(
    const IOobject& io,
    const VRWGraph& g
)
:
    regIOobject(io),
    VRWGraph(g)
{}


void Foam::Module::faceIOGraph::operator=(const faceIOGraph& rhs)
{
    VRWGraph::operator=(rhs);
}


void Foam::Module::faceIOGraph::operator=(const VRWGraph& rhs)
{
    VRWGraph::operator=(rhs);
}


bool Foam::Module::faceIOGraph::writeData(Ostream& os) const
{
    return (os << *this).good();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

defineTypeNameWithName(faceIOGraph, "faceList");

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// ************************************************************************* //
