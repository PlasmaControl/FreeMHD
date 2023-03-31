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

#include "triSurfFacets.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline Foam::label Foam::Module::triSurfFacets::size() const
{
    return triangles_.size();
}


inline const Foam::Module::LongList<Foam::labelledTri>&
Foam::Module::triSurfFacets::facets() const
{
    return triangles_;
}


inline const Foam::geometricSurfacePatchList&
Foam::Module::triSurfFacets::patches() const
{
    return patches_;
}


inline void Foam::Module::triSurfFacets::appendTriangle
(
    const labelledTri& tri
)
{
    triangles_.append(tri);
}


inline void Foam::Module::triSurfFacets::addFacetToSubset
(
    const label setI,
    const label triI
)
{
    Map<meshSubset>::iterator it = facetSubsets_.find(setI);
    if (it == facetSubsets_.end())
    {
        return;
    }

    it().addElement(triI);
}


inline void Foam::Module::triSurfFacets::removeFacetFromSubset
(
    const label setI,
    const label triI
)
{
    Map<meshSubset>::iterator it = facetSubsets_.find(setI);
    if (it == facetSubsets_.end())
    {
        return;
    }

    it().removeElement(triI);
}


inline void Foam::Module::triSurfFacets::facetInSubsets
(
    const label triI,
    DynList<label>& facetSubsets
) const
{
    facetSubsets.clear();

    forAllConstIters(facetSubsets_, it)
    {
        if (it().found(triI))
        {
            facetSubsets.append(it.key());
        }
    }
}


inline void Foam::Module::triSurfFacets::facetSubsetIndices
(
    DynList<label>& indices
) const
{
    indices.clear();

    forAllConstIters(facetSubsets_, it)
    {
        indices.append(it.key());
    }
}


template<class ListType>
inline void Foam::Module::triSurfFacets::facetsInSubset
(
    const label setI,
    ListType& facetsLabels
) const
{
    facetsLabels.clear();

    Map<meshSubset>::const_iterator it = facetSubsets_.cfind(setI);
    if (it == facetSubsets_.end())
    {
        return;
    }

    it().containedElements(facetsLabels);
}


template<class ListType>
inline void Foam::Module::triSurfFacets::updateFacetsSubsets
(
    const ListType& newFacetsLabels
)
{
    forAllIters(facetSubsets_, it)
    {
        it().updateSubset(newFacetsLabels);
    }
}


inline const Foam::labelledTri&
Foam::Module::triSurfFacets::operator[](const label triI) const
{
    return triangles_[triI];
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
