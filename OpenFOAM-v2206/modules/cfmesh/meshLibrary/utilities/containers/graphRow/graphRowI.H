/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | cfMesh: A library for mesh generation
   \\    /   O peration     |
    \\  /    A nd           | www.cfmesh.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2014-2017 Creative Fields, Ltd.
    Copyright (C) 2021 OpenCFD Ltd.
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
inline Foam::Module::graphRow<graphType>::graphRow(graphType& g, const label i)
:
    data_(g),
    rowI_(i)
{}


template<class graphType>
inline Foam::Module::graphRow<graphType>::graphRow
(
    const graphRow<graphType>& r
)
:
    data_(r.data_),
    rowI_(r.rowI_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class graphType>
inline Foam::label Foam::Module::graphRow<graphType>::size() const
{
    return data_.sizeOfRow(rowI_);
}


template<class graphType>
inline void Foam::Module::graphRow<graphType>::setSize(const label s)
{
    data_.setRowSize(rowI_, s);
}


template<class graphType>
inline void Foam::Module::graphRow<graphType>::clear()
{
    data_.setRowSize(rowI_, 0);
}


template<class graphType>
inline void Foam::Module::graphRow<graphType>::append(const label el)
{
    data_.append(rowI_, el);
}


template<class graphType>
inline void Foam::Module::graphRow<graphType>::appendUniq(const label el)
{
    data_.appendUniq(rowI_, el);
}


template<class graphType>
inline bool Foam::Module::graphRow<graphType>::found(const label e) const
{
    return data_.found(rowI_, e);
}


template<class graphType>
inline Foam::label Foam::Module::graphRow<graphType>::find
(
    const label e
) const
{
    return data_.find(rowI_, e);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class graphType>
inline Foam::label Foam::Module::graphRow<graphType>::operator[]
(
    const label i
) const
{
    return data_(rowI_, i);
}


template<class graphType>
inline Foam::label& Foam::Module::graphRow<graphType>::operator[]
(
    const label i
)
{
    return data_(rowI_, i);
}


template<class graphType>
inline void Foam::Module::graphRow<graphType>::operator=
(
    const graphRow<graphType>& rhs
)
{
    const label len = rhs.size();

    data_.setRowSize(rowI_, len);
    for (label i = 0; i < len; ++i)
    {
        data_(rowI_, i) = rhs[i];
    }
}


template<class graphType>
template<class ListType>
inline void Foam::Module::graphRow<graphType>::operator=(const ListType& rhs)
{
    const label len = rhs.size();

    data_.setRowSize(rowI_, len);
    for (label i = 0; i < len; ++i)
    {
        data_(rowI_, i) = rhs[i];
    }
}


template<class graphType>
inline Foam::Ostream& Foam::Module::operator<<
(
    Ostream& os,
    const Foam::Module::graphRow<graphType>& r
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
