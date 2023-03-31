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

#include "triSurfPoints.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::Module::triSurfPoints::triSurfPoints()
:
    points_(),
    pointSubsets_()
{}


Foam::Module::triSurfPoints::triSurfPoints(const pointField& points)
:
    points_(points),
    pointSubsets_()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::label Foam::Module::triSurfPoints::addPointSubset(const word& subsetName)
{
    label id = pointSubsetIndex(subsetName);
    if (id >= 0)
    {
        Warning << "Point subset " << subsetName << " already exists!" << endl;
        return id;
    }

    id = 0;
    forAllConstIters(pointSubsets_, it)
    {
        id = Foam::max(id, it.key()+1);
    }

    pointSubsets_.insert
    (
        id,
        meshSubset(subsetName, meshSubset::POINTSUBSET)
    );

    return id;
}


void Foam::Module::triSurfPoints::removePointSubset(const label subsetID)
{
    if (pointSubsets_.find(subsetID) == pointSubsets_.end())
    {
        return;
    }

    pointSubsets_.erase(subsetID);
}


Foam::word Foam::Module::triSurfPoints::pointSubsetName
(
    const label subsetID
) const
{
    Map<meshSubset>::const_iterator it = pointSubsets_.cfind(subsetID);
    if (it == pointSubsets_.end())
    {
        Warning << "Subset " << subsetID << " is not a point subset" << endl;
        return word();
    }

    return it().name();
}


Foam::label Foam::Module::triSurfPoints::pointSubsetIndex
(
    const word& subsetName
) const
{
    forAllConstIters(pointSubsets_, it)
    {
        if (it().name() == subsetName)
        {
            return it.key();
        }
    }

    return -1;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
