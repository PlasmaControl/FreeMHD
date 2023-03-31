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

inline const Foam::Time& Foam::Module::polyMeshGenPoints::returnTime() const
{
    return runTime_;
}


inline const Foam::Module::pointFieldPMG&
Foam::Module::polyMeshGenPoints::points() const
{
    return points_;
}


inline Foam::Module::pointFieldPMG&
Foam::Module::polyMeshGenPoints::points()
{
    return points_;
}


inline void Foam::Module::polyMeshGenPoints::appendVertex(const point& p)
{
    points_.append(p);
}


inline void Foam::Module::polyMeshGenPoints::addPointToSubset
(
    const label setI,
    const label pointI
)
{
    std::map<label, meshSubset>::iterator it = pointSubsets_.find(setI);
    if (it == pointSubsets_.end())
    {
        return;
    }

    it->second.addElement(pointI);
}


inline void Foam::Module::polyMeshGenPoints::removePointFromSubset
(
    const label setI,
    const label pointI
)
{
    std::map<label, meshSubset>::iterator it = pointSubsets_.find(setI);
    if (it == pointSubsets_.end())
    {
        return;
    }

    it->second.removeElement(pointI);
}


inline void Foam::Module::polyMeshGenPoints::pointInSubsets
(
    const label pointI,
    DynList<label>& pointSubsets
) const
{
    pointSubsets.clear();

    forAllConstIters(pointSubsets_, it)
    {
        if (it->second.found(pointI))
        {
            pointSubsets.append(it->first);
        }
    }
}


inline void Foam::Module::polyMeshGenPoints::pointSubsetIndices
(
    DynList<label>& indices
) const
{
    indices.clear();

    forAllConstIters(pointSubsets_, it)
    {
        indices.append(it->first);
    }
}


template<class ListType>
inline void Foam::Module::polyMeshGenPoints::pointsInSubset
(
    const label setI,
    ListType& pointLabels
) const
{
    pointLabels.clear();

    std::map<label, meshSubset>::const_iterator it = pointSubsets_.find(setI);
    if (it == pointSubsets_.end())
    {
        return;
    }

    it->second.containedElements(pointLabels);
}


template<class ListType>
inline void Foam::Module::polyMeshGenPoints::updatePointSubsets
(
    const ListType& newNodeLabels
)
{
    forAllIters(pointSubsets_, it)
    {
        it->second.updateSubset(newNodeLabels);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
