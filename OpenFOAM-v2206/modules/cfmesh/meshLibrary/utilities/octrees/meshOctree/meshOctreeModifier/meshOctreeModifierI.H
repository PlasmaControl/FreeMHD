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

#include "meshOctreeModifier.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline const Foam::Module::meshOctree&
Foam::Module::meshOctreeModifier::octree() const
{
    return octree_;
}


inline Foam::labelList& Foam::Module::meshOctreeModifier::neiProcsAccess()
{
    return octree_.neiProcs_;
}


inline Foam::boundBox& Foam::Module::meshOctreeModifier::rootBoxAccess()
{
    return octree_.rootBox_;
}


inline bool& Foam::Module::meshOctreeModifier::isRootInitialisedAccess()
{
    return octree_.isRootInitialised_;
}


inline Foam::scalar& Foam::Module::meshOctreeModifier::searchRangeAccess()
{
    return octree_.searchRange_;
}


inline Foam::List<Foam::Pair<Foam::Module::meshOctreeCubeCoordinates>>&
Foam::Module::meshOctreeModifier::neiRangeAccess()
{
    return octree_.neiRange_;
}


inline Foam::Module::meshOctreeCube&
Foam::Module::meshOctreeModifier::initialCubeAccess()
{
    return *octree_.initialCubePtr_;
}


inline Foam::List<Foam::Module::meshOctreeSlot>&
Foam::Module::meshOctreeModifier::dataSlotsAccess()
{
    return octree_.dataSlots_;
}


inline Foam::Module::meshOctreeCube*
Foam::Module::meshOctreeModifier::findCubeForPosition
(
    const meshOctreeCubeCoordinates& cc
) const
{
    return octree_.findCubeForPosition(cc);
}


inline void Foam::Module::meshOctreeModifier::findLeavesContainedInBox
(
    const boundBox& bb,
    DynList<const meshOctreeCube*, 256>& containedLeaves
) const
{
    containedLeaves.clear();
    octree_.findLeavesContainedInBox(bb, containedLeaves);
}


inline Foam::Module::LongList<Foam::Module::meshOctreeCube*>&
Foam::Module::meshOctreeModifier::leavesAccess()
{
    return octree_.leaves_;
}


inline void Foam::Module::meshOctreeModifier::createListOfLeaves()
{
    octree_.leaves_.clear();

    octree_.initialCubePtr_->findLeaves(octree_.leaves_);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
