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

#include "Ostream.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class graphType>
inline Foam::Module::graphConstRow<graphType>::graphConstRow
(
    const graphType& g,
    const label i
)
:
    data_(g),
    rowI_(i)
{}


template<class graphType>
inline Foam::Module::graphConstRow<graphType>::graphConstRow
(
    const graphConstRow<graphType>& r
)
:
    data_(r.data_),
    rowI_(r.rowI_)
{}


template<class graphType>
inline Foam::Module::graphConstRow<graphType>::graphConstRow
(
    const graphRow<graphType>& r
)
:
    data_(r.data_),
    rowI_(r.rowI_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class graphType>
inline Foam::label Foam::Module::graphConstRow<graphType>::size() const
{
    return data_.sizeOfRow(rowI_);
}


template<class graphType>
inline bool Foam::Module::graphConstRow<graphType>::found(const label e) const
{
    return data_.found(rowI_, e);
}


template<class graphType>
inline Foam::label Foam::Module::graphConstRow<graphType>::find
(
    const label e
) const
{
    return data_.find(rowI_, e);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class graphType>
inline Foam::label Foam::Module::graphConstRow<graphType>::operator[]
(
    const label i
) const
{
    return data_(rowI_, i);
}


template<class graphType>
inline Foam::Ostream& Foam::Module::operator<<
(
    Ostream& os,
    const Foam::Module::graphConstRow<graphType>& r
)
{
    const label len = r.size();

    os << len << '(';
    for (label i = 0; i < len; ++i)
    {
        if (i) os << ' ';
        os << r[i];
    }
    os << ')';

    return os;
}


// ************************************************************************* //
