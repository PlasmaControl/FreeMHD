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

inline void Foam::Module::VRWGraphList::checkIndex
(
    const label i,
    const label j,
    const label k
) const
{
    if ((i < 0) || (i >= rows_.size()))
    {
        FatalErrorInFunction
            << "Position index " << Foam::label(i)
            << " is not in range " << Foam::label(0)
            << " and " << rows_.size() << abort(FatalError);
    }

    if ((j < 0) || (j >= rows_[i].size()))
    {
        FatalErrorInFunction
            << "Row index " << Foam::label(j)
            << " is not in range " << Foam::label(0)
            << " and " << rows_[i].size() << abort(FatalError);
    }

    if ((k < 0) || (k >= data_.sizeOfRow(rows_[i].start()+j)))
    {
        FatalErrorInFunction
            << "Data index " << Foam::label(k)
            << " is not in range " << Foam::label(0)
            << " and " << data_.sizeOfRow(rows_[i].start()+j)
            << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline Foam::Module::VRWGraphList::VRWGraphList()
:
    data_(),
    rows_()
{}


inline Foam::Module::VRWGraphList::VRWGraphList
(
    const VRWGraphList& ol
)
:
    data_(ol.data_),
    rows_(ol.rows_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline Foam::label Foam::Module::VRWGraphList::size() const
{
    return rows_.size();
}


inline Foam::label Foam::Module::VRWGraphList::sizeOfGraph
(
    const label posI
) const
{
    return rows_[posI].size();
}


inline Foam::label Foam::Module::VRWGraphList::sizeOfRow
(
    const label posI,
    const label rowI
) const
{
    return data_.sizeOfRow(rows_[posI].start()+rowI);
}


inline void Foam::Module::VRWGraphList::clear()
{
    data_.setSize(0);
    rows_.setSize(0);
}


template<class GraphType>
inline void Foam::Module::VRWGraphList::appendGraph
(
    const GraphType& l
)
{
    rowElement re(data_.size(), l.size());

    for (label i = 0; i < l.size(); ++i)
    {
        data_.appendList(l[i]);
    }

    rows_.append(re);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline Foam::label Foam::Module::VRWGraphList::operator()
(
    const label i,
    const label j,
    const label k
) const
{
    #ifdef FULLDEBUG
    checkIndex(i, j, k);
    #endif

    return data_(rows_[i].start() + j, k);
}


inline Foam::label& Foam::Module::VRWGraphList::operator()
(
    const label i,
    const label j,
    const label k
)
{
    #ifdef FULLDEBUG
    checkIndex(i, j, k);
    #endif

    return data_(rows_[i].start() + j, k);
}


inline const Foam::Module::subGraph<const Foam::Module::VRWGraph>
Foam::Module::VRWGraphList::operator[]
(
    const label i
) const
{
    return Foam::Module::subGraph<const VRWGraph>(data_, i, rows_[i].size());
}


inline void Foam::Module::VRWGraphList::operator=(const VRWGraphList& l)
{
    data_ = l.data_;
    rows_ = l.rows_;
}


// ************************************************************************* //
