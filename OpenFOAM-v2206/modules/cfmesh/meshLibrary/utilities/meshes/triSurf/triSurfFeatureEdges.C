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

#include "triSurfFeatureEdges.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::Module::triSurfFeatureEdges::triSurfFeatureEdges()
:
    featureEdges_(),
    featureEdgeSubsets_()
{}


Foam::Module::triSurfFeatureEdges::triSurfFeatureEdges
(
    const edgeLongList& featureEdges
)
:
    featureEdges_(featureEdges),
    featureEdgeSubsets_()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::label Foam::Module::triSurfFeatureEdges::addEdgeSubset
(
    const word& subsetName
)
{
    label id = edgeSubsetIndex(subsetName);
    if (id >= 0)
    {
        Warning << "Edge subset " << subsetName << " already exists!" << endl;
        return id;
    }

    id = 0;
    forAllConstIters(featureEdgeSubsets_, it)
    {
        id = Foam::max(id, it.key()+1);
    }

    featureEdgeSubsets_.insert
    (
        id,
        meshSubset(subsetName, meshSubset::FEATUREEDGESUBSET)
    );

    return id;
}


void Foam::Module::triSurfFeatureEdges::removeEdgeSubset(const label subsetID)
{
    if (featureEdgeSubsets_.find(subsetID) == featureEdgeSubsets_.end())
    {
        return;
    }

    featureEdgeSubsets_.erase(subsetID);
}


Foam::word Foam::Module::triSurfFeatureEdges::edgeSubsetName
(
    const label subsetID
) const
{
    Map<meshSubset>::const_iterator it = featureEdgeSubsets_.find(subsetID);
    if (it == featureEdgeSubsets_.end())
    {
        Warning << "Subset " << subsetID << " is not an edge subset" << endl;
        return word();
    }

    return it().name();
}


Foam::label Foam::Module::triSurfFeatureEdges::edgeSubsetIndex
(
    const word& subsetName
) const
{
    forAllConstIters(featureEdgeSubsets_, it)
    {
        if (it().name() == subsetName)
        {
            return it.key();
        }
    }

    return -1;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
