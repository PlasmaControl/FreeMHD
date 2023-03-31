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

#include "VRWGraphList.H"
#include "token.H"
#include "labelList.H"

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Foam::Ostream& Foam::Module::operator<<
(
    Ostream& os,
    const Foam::Module::VRWGraphList& DL
)
{
    os << DL.size() << nl << token::BEGIN_LIST;

    for (label i = 0; i < DL.size(); ++i)
    {
        os << nl << DL[i];
    }

    os << nl << token::END_LIST;

    // Check state of IOstream
    os.check(FUNCTION_NAME);

    return os;
}


/*
template<class T, Foam::label width>
Foam::Istream& Foam::Module::operator>>
(
    Istream& is,
    Foam::Module::VRWGraphList<T, width>& DL
)
{
    label size;
    T e;
    is >> size;
    DL.setSize(size);
    for(IndexType i=0;i<size;++i)
    {
        is >> e;
        DL[i] = e;
    }

    return is;
}
*/

// ************************************************************************* //
