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

#include "triSurfFeatureEdges.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline Foam::label Foam::Module::triSurfFeatureEdges::nFeatureEdges() const
{
    return featureEdges_.size();
}


inline const Foam::Module::edgeLongList&
Foam::Module::triSurfFeatureEdges::featureEdges() const
{
    return featureEdges_;
}


inline void Foam::Module::triSurfFeatureEdges::appendFeatureEdge(const edge& e)
{
    featureEdges_.append(e);
}


inline void Foam::Module::triSurfFeatureEdges::addEdgeToSubset
(
    const label setI,
    const label eI
)
{
    Map<meshSubset>::iterator it = featureEdgeSubsets_.find(setI);
    if (it == featureEdgeSubsets_.end())
    {
        return;
    }

    it().addElement(eI);
}


inline void Foam::Module::triSurfFeatureEdges::removeEdgeFromSubset
(
    const label setI,
    const label eI
)
{
    Map<meshSubset>::iterator it = featureEdgeSubsets_.find(setI);
    if (it == featureEdgeSubsets_.end())
    {
        return;
    }

    it().removeElement(eI);
}


inline void Foam::Module::triSurfFeatureEdges::edgeInSubsets
(
    const label eI,
    DynList<label>& edgeSubsets
) const
{
    edgeSubsets.clear();

    forAllConstIters(featureEdgeSubsets_, it)
    {
        if (it().found(eI))
        {
            edgeSubsets.append(it.key());
        }
    }
}


inline void Foam::Module::triSurfFeatureEdges::edgeSubsetIndices
(
    DynList<label>& indices
) const
{
    indices.clear();

    forAllConstIters(featureEdgeSubsets_, it)
    {
        indices.append(it.key());
    }
}


template<class ListType>
inline void Foam::Module::triSurfFeatureEdges::edgesInSubset
(
    const label setI,
    ListType& edgeLabels
) const
{
    edgeLabels.clear();

    Map<meshSubset>::const_iterator it = featureEdgeSubsets_.find(setI);
    if (it == featureEdgeSubsets_.end())
    {
        return;
    }

    it().containedElements(edgeLabels);
}


template<class ListType>
inline void Foam::Module::triSurfFeatureEdges::updateEdgeSubsets
(
    const ListType& newEdgeLabels
)
{
    forAllIters(featureEdgeSubsets_, it)
    {
        it().updateSubset(newEdgeLabels);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
