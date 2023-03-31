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

template<class graphType>
inline void Foam::Module::subGraph<graphType>::checkIndex(const label i) const
{
    if ((i < 0) || (i >= size_))
    {
        FatalErrorInFunction
            << "Row index " << i
            << " is not in range " << Foam::label(0)
            << " and " << size_ << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class graphType>
inline Foam::Module::subGraph<graphType>::subGraph
(
    graphType& g,
    const label start,
    const label size
)
:
    data_(g),
    start_(start),
    size_(size)
{}


template<class graphType>
inline Foam::Module::subGraph<graphType>::subGraph
(
    const subGraph<graphType>& ol
)
:
    data_(ol.data_),
    start_(ol.start_),
    size_(ol.size_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class graphType>
inline Foam::label Foam::Module::subGraph<graphType>::size() const
{
    return size_;
}


template<class graphType>
inline Foam::label Foam::Module::subGraph<graphType>::sizeOfRow
(
    const label rowI
) const
{
    return data_.sizeOfRow(start_ + rowI);
}


template<class graphType>
inline void Foam::Module::subGraph<graphType>::append
(
    const label rowI,
    const label el
)
{
    data_.append(start_ + rowI, el);
}


template<class graphType>
inline void Foam::Module::subGraph<graphType>::appendUniq
(
    const label rowI,
    const label el
)
{
    data_.appendUniq(start_ + rowI, el);
}


template<class graphType>
inline bool Foam::Module::subGraph<graphType>::found
(
    const label rowI,
    const label e
) const
{
    return data_.found(start_ + rowI, e);
}


template<class graphType>
inline Foam::label Foam::Module::subGraph<graphType>::find
(
    const label rowI,
    const label e
) const
{
    return data_.find(start_ + rowI, e);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class graphType>
inline Foam::label Foam::Module::subGraph<graphType>::operator()
(
    const label i,
    const label j
) const
{
    # ifdef FULLDEBUG
    checkIndex(i);
    # endif

    return data_(start_ + i, j);
}


template<class graphType>
inline Foam::label& Foam::Module::subGraph<graphType>::operator()
(
    const label i,
    const label j
)
{
    return data_(start_ + i, j);
}


template<class graphType>
inline const Foam::Module::graphConstRow<graphType>
Foam::Module::subGraph<graphType>::operator[]
(
    const label i
) const
{
    return data_[start_ + i];
}


template<class graphType>
inline Foam::Module::graphRow<graphType>
Foam::Module::subGraph<graphType>::operator[](const label i)
{
    return data_[start_ + i];
}


template<class graphType>
inline Foam::Ostream& Foam::Module::operator<<
(
    Ostream& os,
    const Foam::Module::subGraph<graphType>& sg
)
{
    os << sg.size() << "\n" << "(";
    for (Foam::label i = 0; i < sg.size(); ++i)
    {
        os << "\n" << sg.sizeOfRow(i) << "(";
        for (Foam::label j = 0; j < sg.sizeOfRow(i); ++j)
        {
            if (j > 0) os << " ";

            os << sg(i, j);
        }

        os << ")";
    }

    os << "\n" << ")";

    return os;
}


// ************************************************************************* //
