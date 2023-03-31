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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline Foam::label Foam::Module::triSurfPoints::nPoints() const
{
    return points_.size();
}


inline const Foam::pointField& Foam::Module::triSurfPoints::points() const
{
    return points_;
}


inline void Foam::Module::triSurfPoints::appendVertex(const point& p)
{
    const label s = points_.size();
    points_.setSize(s + 1);
    points_[s] = p;
}


inline void Foam::Module::triSurfPoints::addPointToSubset
(
    const label setI,
    const label pointI
)
{
    Map<meshSubset>::iterator it = pointSubsets_.find(setI);
    if (it == pointSubsets_.end())
    {
        return;
    }

    it().addElement(pointI);
}


inline void Foam::Module::triSurfPoints::removePointFromSubset
(
    const label setI,
    const label pointI
)
{
    Map<meshSubset>::iterator it = pointSubsets_.find(setI);
    if (it == pointSubsets_.end())
    {
        return;
    }

    it().removeElement(pointI);
}


inline void Foam::Module::triSurfPoints::pointInSubsets
(
    const label pointI,
    DynList<label>& pointSubsets
) const
{
    pointSubsets.clear();

    forAllConstIters(pointSubsets_, it)
    {
        if (it().found(pointI))
        {
            pointSubsets.append(it.key());
        }
    }
}


inline void Foam::Module::triSurfPoints::pointSubsetIndices
(
    DynList<label>& indices
) const
{
    indices.clear();

    forAllConstIters(pointSubsets_, it)
    {
        indices.append(it.key());
    }
}


template<class ListType>
inline void Foam::Module::triSurfPoints::pointsInSubset
(
    const label setI,
    ListType& pointLabels
) const
{
    pointLabels.clear();

    Map<meshSubset>::const_iterator it = pointSubsets_.cfind(setI);

    if (it == pointSubsets_.end())
    {
        return;
    }

    it().containedElements(pointLabels);
}


template<class ListType>
inline void Foam::Module::triSurfPoints::updatePointSubsets
(
    const ListType& newNodeLabels
)
{
    forAllIters(pointSubsets_, it)
    {
        it().updateSubset(newNodeLabels);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
