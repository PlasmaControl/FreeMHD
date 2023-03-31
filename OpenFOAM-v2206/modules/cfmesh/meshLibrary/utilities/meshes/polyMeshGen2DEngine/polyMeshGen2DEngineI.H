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

inline const Foam::boolList&
Foam::Module::polyMeshGen2DEngine::activeFace() const
{
    if (!activeFacePtr_)
    {
        findActiveFaces();
    }

    return *activeFacePtr_;
}


inline const Foam::labelList&
Foam::Module::polyMeshGen2DEngine::activeFaceLabels() const
{
    if (!activeFaceLabelsPtr_)
    {
        findActiveFaceLabels();
    }

    return *activeFaceLabelsPtr_;
}


inline const Foam::boolList&
Foam::Module::polyMeshGen2DEngine::zMinPoints() const
{
    if (!zMinPointPtr_)
    {
        findZMinPoints();
    }

    return *zMinPointPtr_;
}


inline const Foam::labelList&
Foam::Module::polyMeshGen2DEngine::zMinPointLabels() const
{
    if (!zMinPointLabelsPtr_)
    {
        findZMinPointLabels();
    }

    return *zMinPointLabelsPtr_;
}


inline const Foam::labelList&
Foam::Module::polyMeshGen2DEngine::zMinToZMax() const
{
    if (!zMinToZMaxPtr_)
    {
        findZMinOffsetPoints();
    }

    return *zMinToZMaxPtr_;
}


inline const Foam::boolList&
Foam::Module::polyMeshGen2DEngine::zMaxPoints() const
{
    if (!zMaxPointPtr_)
    {
        findZMaxPoints();
    }

    return *zMaxPointPtr_;
}


inline const Foam::labelList&
Foam::Module::polyMeshGen2DEngine::zMaxPointLabels() const
{
    if (!zMaxPointLabelsPtr_)
    {
        findZMaxPointLabels();
    }

    return *zMaxPointLabelsPtr_;
}


inline const Foam::labelList&
Foam::Module::polyMeshGen2DEngine::zMaxToZMin() const
{
    if (!zMaxToZMinPtr_)
    {
        findZMaxOffsetPoints();
    }

    return *zMaxToZMinPtr_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
