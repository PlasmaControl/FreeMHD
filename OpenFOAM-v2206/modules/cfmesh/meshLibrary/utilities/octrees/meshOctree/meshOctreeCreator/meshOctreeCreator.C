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

#include "meshOctreeCreator.H"
#include "triSurf.H"
#include "boundBox.H"
#include "demandDrivenData.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::meshOctreeCreator::meshOctreeCreator(meshOctree& mo)
:
    octree_(mo),
    scalingFactor_(1.0),
    meshDictPtr_(nullptr),
    hexRefinement_(false),
    globalRefLevel_(0),
    surfRefLevel_(mo.surface().size())
{}


Foam::Module::meshOctreeCreator::meshOctreeCreator
(
    meshOctree& mo,
    const IOdictionary& dict
)
:
    octree_(mo),
    scalingFactor_(1.0),
    meshDictPtr_(&dict),
    hexRefinement_(false),
    globalRefLevel_(0),
    surfRefLevel_(mo.surface().size())
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::meshOctreeCreator::setScalingFactor(const scalar s)
{
    scalingFactor_ = s;
}


void Foam::Module::meshOctreeCreator::activateHexRefinement()
{
    hexRefinement_ = true;
}


// ************************************************************************* //
