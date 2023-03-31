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

# ifdef USE_OMP
#include <omp.h>
# endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline const Foam::Module::faceListPMG&
Foam::Module::polyMeshGenFaces::faces() const
{
    return faces_;
}


inline Foam::label Foam::Module::polyMeshGenFaces::nInternalFaces() const
{
    if (!(ownerPtr_ && neighbourPtr_))
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
        {
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        }
        # endif

        calculateOwnersAndNeighbours();
    }

    return nIntFaces_;
}


inline const Foam::labelList& Foam::Module::polyMeshGenFaces::owner() const
{
    if (!ownerPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
        {
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        }
        # endif

        calculateOwnersAndNeighbours();
    }

    return *ownerPtr_;
}


inline const Foam::labelList& Foam::Module::polyMeshGenFaces::neighbour() const
{
    if (!neighbourPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
        {
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        }
        # endif

        calculateOwnersAndNeighbours();
    }

    return *neighbourPtr_;
}


inline const Foam::PtrList<Foam::Module::processorBoundaryPatch>&
Foam::Module::polyMeshGenFaces::procBoundaries() const
{
    return procBoundaries_;
}


inline const Foam::PtrList<Foam::Module::boundaryPatch>&
Foam::Module::polyMeshGenFaces::boundaries() const
{
    return boundaries_;
}


inline void Foam::Module::polyMeshGenFaces::addFaceToSubset
(
    const label setID,
    const label faceI
)
{
    std::map<label, meshSubset>::iterator it = faceSubsets_.find(setID);
    if (it == faceSubsets_.end())
    {
        return;
    }

    it->second.addElement(faceI);
}


inline void Foam::Module::polyMeshGenFaces::removeFaceFromSubset
(
    const label setI,
    const label faceI)
{
    std::map<label, meshSubset>::iterator it = faceSubsets_.find(setI);
    if (it == faceSubsets_.end())
    {
        return;
    }

    it->second.removeElement(faceI);
}


inline void Foam::Module::polyMeshGenFaces::faceInSubsets
(
    const label faceI,
    DynList<label>& faceSubsets
) const
{
    faceSubsets.clear();

    forAllConstIters(faceSubsets_, it)
    {
        if (it->second.found(faceI))
        {
            faceSubsets.append(it->first);
        }
    }
}


inline void Foam::Module::polyMeshGenFaces::faceSubsetIndices
(
    DynList<label>& indices
) const
{
    indices.clear();

    forAllConstIters(faceSubsets_, it)
    {
        indices.append(it->first);
    }
}


template<class ListType>
inline void Foam::Module::polyMeshGenFaces::facesInSubset
(
    const label setI,
    ListType& faceLabels
) const
{
    faceLabels.clear();

    std::map<label, meshSubset>::const_iterator it = faceSubsets_.find(setI);
    if (it == faceSubsets_.end())
    {
        return;
    }

    it->second.containedElements(faceLabels);
}


template<class ListType>
inline void Foam::Module::polyMeshGenFaces::updateFaceSubsets
(
    const ListType& newFaceLabels
)
{
    forAllIters(faceSubsets_, it)
    {
        it->second.updateSubset(newFaceLabels);
    }
}


inline void Foam::Module::polyMeshGenFaces::updateFaceSubsets
(
    const VRWGraph& newFacesForFace
)
{
    forAllIters(faceSubsets_, it)
    {
        it->second.updateSubset(newFacesForFace);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
