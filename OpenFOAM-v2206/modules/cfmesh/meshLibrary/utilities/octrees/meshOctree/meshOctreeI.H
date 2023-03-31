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

#include "meshOctree.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline bool Foam::Module::meshOctree::isQuadtree() const
{
    return isQuadtree_;
}


inline const Foam::FixedList<Foam::Vector<Foam::label>, 8>&
Foam::Module::meshOctree::octantVectors() const
{
    return octantVectors_;
}


inline Foam::label Foam::Module::meshOctree::numberOfLeaves() const
{
    if (leaves_.size() == 0)
        FatalErrorInFunction
            << "Leaves are not yet created!!" << exit(FatalError);

    return leaves_.size();
}


inline const Foam::Module::meshOctreeCubeBasic&
Foam::Module::meshOctree::returnLeaf
(
    const label leafI
) const
{
    return *leaves_[leafI];
}


inline short Foam::Module::meshOctree::leafAtProc
(
    const label leafI
) const
{
    return leaves_[leafI]->procNo();
}


inline bool Foam::Module::meshOctree::hasContainedTriangles
(
    const label leafI
) const
{
    if (leaves_[leafI]->hasContainedElements())
        return true;

    return false;
}


inline void Foam::Module::meshOctree::containedTriangles
(
    const label leafI,
    DynList<label>& triangles
) const
{
    triangles.clear();

    if (!leaves_[leafI]->hasContainedElements())
    {
        triangles.clear();
        return;
    }

    const VRWGraph& containedTriangles =
        leaves_[leafI]->slotPtr()->containedTriangles_;
    constRow elmts = containedTriangles[leaves_[leafI]->containedElements()];
    forAll(elmts, elI)
        triangles.append(elmts[elI]);
}


inline bool Foam::Module::meshOctree::hasContainedEdges
(
    const label leafI
) const
{
    if (leaves_[leafI]->hasContainedEdges())
        return true;

    return false;
}


inline void Foam::Module::meshOctree::containedEdges
(
    const label leafI,
    DynList<label>& edges
) const
{
    edges.clear();

    if (!leaves_[leafI]->hasContainedEdges())
    {
        edges.clear();
        return;
    }

    const VRWGraph& containedEdges =
        leaves_[leafI]->slotPtr()->containedEdges_;
    constRow elmts = containedEdges[leaves_[leafI]->containedEdges()];
    forAll(elmts, eI)
        edges.append(elmts[eI]);
}


inline const Foam::Module::triSurf& Foam::Module::meshOctree::surface() const
{
    return surface_;
}


inline const Foam::boundBox& Foam::Module::meshOctree::rootBox() const
{
    return rootBox_;
}


inline const Foam::FixedList<Foam::Module::meshOctreeCubeCoordinates, 26>&
Foam::Module::meshOctree::regularityPositions() const
{
    return regularityPositions_;
}


inline const Foam::FixedList
<
    Foam::FixedList<Foam::Module::meshOctreeCubeCoordinates, 8>,
    8
>&
Foam::Module::meshOctree::positionsOfLeavesAtNodes() const
{
    return vrtLeavesPos_;
}


inline const Foam::labelList& Foam::Module::meshOctree::neiProcs() const
{
    return neiProcs_;
}


inline Foam::label Foam::Module::meshOctree::findNeighbourOverNode
(
    const label leafI,
    const label nodeI
) const
{
    return findNeighbourOverNode(leaves_[leafI]->coordinates(), nodeI);
}


inline void Foam::Module::meshOctree::findNeighboursOverEdge
(
    const label leafI,
    const label eI,
    DynList<label>& neighbourLeaves
) const
{
    findNeighboursOverEdge(leaves_[leafI]->coordinates(), eI, neighbourLeaves);
}


inline void Foam::Module::meshOctree::findNeighboursInDirection
(
    const label leafI,
    const label dir,
    DynList<label>& neiLeaves
) const
{
    findNeighboursInDirection(leaves_[leafI]->coordinates(), dir, neiLeaves);
}


inline void Foam::Module::meshOctree::findNeighboursForLeaf
(
    const label leafI,
    DynList<label>& neighbourLeaves
) const
{
    findNeighboursForLeaf(leaves_[leafI]->coordinates(), neighbourLeaves);
}


inline void Foam::Module::meshOctree::findAllLeafNeighbours
(
    const label leafI,
    DynList<label>& neighbourLeaves
) const
{
    findAllLeafNeighbours(leaves_[leafI]->coordinates(), neighbourLeaves);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
