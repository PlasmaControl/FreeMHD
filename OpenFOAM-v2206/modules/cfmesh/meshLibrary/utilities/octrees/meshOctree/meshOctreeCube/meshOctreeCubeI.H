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

#include "meshOctreeCube.H"
#include "triSurface.H"
#include "meshOctreeSlot.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline Foam::Module::meshOctreeCube::meshOctreeCube()
:
    meshOctreeCubeBasic(),
    activeSlotPtr_(nullptr),
    subCubesPtr_(nullptr),
    cubeLabel_(-1),
    containedElementsLabel_(-1),
    containedEdgesLabel_(-1)
{}


inline Foam::Module::meshOctreeCube::meshOctreeCube(const meshOctreeCube& moc)
:
    meshOctreeCubeBasic(moc.coordinates(), moc.cubeType(), moc.procNo()),
    activeSlotPtr_(moc.activeSlotPtr_),
    subCubesPtr_(moc.subCubesPtr_),
    cubeLabel_(moc.cubeLabel_),
    containedElementsLabel_(moc.containedElementsLabel_),
    containedEdgesLabel_(moc.containedEdgesLabel_)
{}


inline const Foam::Module::meshOctreeSlot*
Foam::Module::meshOctreeCube::slotPtr() const
{
    return activeSlotPtr_;
}


inline bool Foam::Module::meshOctreeCube::isLeaf() const
{
    if (!subCubesPtr_)
    {
        return true;
    }

    return false;
}


inline Foam::Module::meshOctreeCube*
Foam::Module::meshOctreeCube::subCube(const label scI) const
{
    return subCubesPtr_[scI];
}


inline Foam::label Foam::Module::meshOctreeCube::cubeLabel() const
{
    return cubeLabel_;
}


inline bool Foam::Module::meshOctreeCube::hasContainedElements() const
{
    if (containedElementsLabel_ == -1)
    {
        return false;
    }

    return true;
}


inline Foam::label Foam::Module::meshOctreeCube::containedElements() const
{
    return containedElementsLabel_;
}


inline bool Foam::Module::meshOctreeCube::hasContainedEdges() const
{
    if (containedEdgesLabel_ == -1)
    {
        return false;
    }

    return true;
}


inline Foam::label Foam::Module::meshOctreeCube::containedEdges() const
{
    return containedEdgesLabel_;
}


inline void Foam::Module::meshOctreeCube::operator=(const meshOctreeCube& moc)
{
    static_cast<meshOctreeCubeCoordinates&>(*this) = moc.coordinates();
    activeSlotPtr_ = moc.activeSlotPtr_;
    subCubesPtr_ = moc.subCubesPtr_;
    cubeLabel_ = moc.cubeLabel_;
    containedElementsLabel_ = moc.containedElementsLabel_;
    containedEdgesLabel_ = moc.containedEdgesLabel_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
