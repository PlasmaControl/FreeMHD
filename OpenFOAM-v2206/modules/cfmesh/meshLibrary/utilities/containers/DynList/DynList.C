/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | cfMesh: A library for mesh generation
   \\    /   O peration     |
    \\  /    A nd           | www.cfmesh.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2014-2017 Creative Fields, Ltd.
    Copyright (C) 2018 OpenCFD Ltd.
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

#include "DynList.H"

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class T, int SizeMin>
Foam::Module::DynList<T, SizeMin>::DynList(Istream& is)
:
    UList<T>(),
    shortList_(),
    heapList_(),
    capacity_(0)
{
    is >> *this;
}


template<class T, int SizeMin>
Foam::Ostream& Foam::Module::operator<<
(
    Ostream& os,
    const Foam::Module::DynList<T, SizeMin>& list
)
{
    os << static_cast<const UList<T>&>(list);
    return os;
}


template<class T, int SizeMin>
Foam::Istream& Foam::Module::operator>>
(
    Istream& is,
    Foam::Module::DynList<T, SizeMin>& list
)
{
    list.clearStorage();

    List<T> input(is);

    const label newLen = input.size();

    if (newLen <= SizeMin)
    {
        list.shortList_ = input;
    }
    else
    {
        list.heapList_.transfer(input);
    }

    list.setSize(newLen);

    return is;
}


// ************************************************************************* //
