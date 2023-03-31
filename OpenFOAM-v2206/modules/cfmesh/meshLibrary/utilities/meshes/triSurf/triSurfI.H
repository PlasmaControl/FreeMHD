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

inline Foam::Module::LongList<Foam::labelledTri>&
Foam::Module::triSurf::accessToFacets()
{
    return triSurfFacets::triangles_;
}


inline Foam::geometricSurfacePatchList&
Foam::Module::triSurf::accessToPatches()
{
    return triSurfFacets::patches_;
}


/*
inline Foam::label Foam::Module::triSurf::numberOfFaceGroups() const
{
    if (!faceGroupPtr_)
    {
        calculateFaceGroups();
    }

    return nFaceGroups_;
}

inline const Foam::Module::labelLongList&
Foam::Module::triSurf::faceGroups() const
{
    if (!faceGroupPtr_)
    {
        calculateFaceGroups();
    }

    return *faceGroupPtr_;
}
*/


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
