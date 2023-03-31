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

#include "triSurfFacets.H"
#include "pointIOField.H"
#include "IOobjectList.H"
#include "pointSet.H"
#include "stringListOps.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::Module::triSurfFacets::triSurfFacets()
:
    triangles_(),
    patches_(),
    facetSubsets_()
{}


Foam::Module::triSurfFacets::triSurfFacets
(
    const LongList<labelledTri>& triangles
)
:
    triangles_(triangles),
    patches_(1),
    facetSubsets_()
{
    forAll(triangles_, triI)
    {
        triangles_[triI].region() = 0;
    }

    patches_[0].name() = "patch";
}


Foam::Module::triSurfFacets::triSurfFacets
(
    const LongList<labelledTri>& triangles,
    const geometricSurfacePatchList& patches
)
:
    triangles_(triangles),
    patches_(patches),
    facetSubsets_()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::wordList Foam::Module::triSurfFacets::patchNames() const
{
    wordList t(patches_.size());

    forAll(patches_, patchI)
    {
        t[patchI] = patches_[patchI].name();
    }

    return t;
}


Foam::labelList Foam::Module::triSurfFacets::findPatches
(
    const word& patchName
) const
{
    const labelList patchIDs =
        findMatchingStrings(regExp(patchName), patchNames());

    # ifdef DEBUGtriSurf
    if (patchIDs.empty())
    {
        WarningInFunction
            << "Cannot find any patch names matching " << patchName << endl;
    }
    # endif

    return patchIDs;
}


Foam::label Foam::Module::triSurfFacets::addFacetSubset(const word& subsetName)
{
    label id = facetSubsetIndex(subsetName);
    if (id >= 0)
    {
        Warning << "Point subset " << subsetName << " already exists!" << endl;
        return id;
    }

    id = 0;
    forAllConstIters(facetSubsets_, it)
    {
        id = Foam::max(id, it.key()+1);
    }

    facetSubsets_.insert
    (
        id,
        meshSubset(subsetName, meshSubset::FACESUBSET)
    );

    return id;
}


void Foam::Module::triSurfFacets::removeFacetSubset(const label subsetID)
{
    if (facetSubsets_.find(subsetID) == facetSubsets_.end())
    {
        return;
    }

    facetSubsets_.erase(subsetID);
}


Foam::word Foam::Module::triSurfFacets::facetSubsetName
(
    const label subsetID
) const
{
    Map<meshSubset>::const_iterator it = facetSubsets_.cfind(subsetID);
    if (it == facetSubsets_.end())
    {
        Warning << "Subset " << subsetID << " is not a facet subset" << endl;
        return word();
    }

    return it().name();
}


Foam::label Foam::Module::triSurfFacets::facetSubsetIndex
(
    const word& subsetName
) const
{
    forAllConstIters(facetSubsets_, it)
    {
        if (it().name() == subsetName)
        {
            return it.key();
        }
    }

    return -1;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
