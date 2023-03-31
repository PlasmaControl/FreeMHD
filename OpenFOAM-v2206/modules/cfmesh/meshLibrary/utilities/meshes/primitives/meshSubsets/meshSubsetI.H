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

#include "meshSubset.H"
#include "labelList.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline Foam::Module::meshSubset::meshSubset()
:
    name_(),
    type_(UNKNOWN),
    data_()
{}


inline Foam::Module::meshSubset::meshSubset
(
    const word& name,
    const meshSubset::subsetType_& t
)
:
    name_(name),
    type_(t),
    data_()
{}


template<class ListType>
inline Foam::Module::meshSubset::meshSubset
(
    const word& name,
    const meshSubset::subsetType_& type,
    const ListType& elements
)
:
    name_(name),
    type_(type),
    data_()
{
    forAll(elements, i)
    {
        data_.insert(elements[i]);
    }
}


inline Foam::Module::meshSubset::meshSubset(const meshSubset& ms)
:
    name_(ms.name_),
    type_(ms.type_),
    data_()
{
    data_ = ms.data_;
}


inline Foam::Module::meshSubset::meshSubset(Istream& is)
:
    name_(),
    type_(UNKNOWN),
    data_()
{
    is >> *this;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const Foam::word& Foam::Module::meshSubset::name() const
{
    return name_;
}


inline Foam::label Foam::Module::meshSubset::type() const
{
    return type_;
}


template<class ListType>
inline void Foam::Module::meshSubset::containedElements(ListType& l) const
{
    l.setSize(data_.size());

    label counter(0);
    for (const label elemi : data_)
    {
        l[counter++] = elemi;
    }
}


inline void Foam::Module::meshSubset::addElement(const label elmt)
{
    data_.insert(elmt);
}


inline void Foam::Module::meshSubset::removeElement(const label elmt)
{
    data_.erase(elmt);
}


template<class ListType>
inline void Foam::Module::meshSubset::updateSubset(const ListType& newLabels)
{
    std::set<label> newData;

    for (const label elemi : data_)
    {
        if (newLabels[elemi] < 0)
        {
            continue;
        }

        newData.insert(newLabels[elemi]);
    }

    data_ = newData;
}


inline void Foam::Module::meshSubset::updateSubset(const VRWGraph& newLabels)
{
    std::set<label> newData;

    for (const label elemi : data_)
    {
        forAllRow(newLabels, elemi, i)
        {
            newData.insert(newLabels(elemi, i));
        }
    }

    data_ = newData;
}


inline bool Foam::Module::meshSubset::found(const label elmt) const
{
    return (data_.find(elmt) != data_.end());
}


inline void Foam::Module::meshSubset::operator=(const meshSubset& ms)
{
    name_ = ms.name_;
    type_ = ms.type_;

    data_.clear();
    data_ = ms.data_;
}


inline bool Foam::Module::meshSubset::operator==(const meshSubset& ms) const
{
    if (ms.name_ != name_)
    {
        return false;
    }
    if (ms.type_ != type_)
    {
        return false;
    }

    for (const label elemi : data_)
    {
        if (ms.data_.find(elemi) == ms.data_.end())
        {
            return false;
        }
    }

    return true;
}


inline bool Foam::Module::meshSubset::operator!=(const meshSubset& ms) const
{
    return !operator==(ms);
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

inline Foam::Ostream& Foam::Module::operator<<
(
    Ostream& os,
    const Foam::Module::meshSubset& sel
)
{
    os.check(FUNCTION_NAME);

    os << sel.name_ << nl << sel.type_;

    labelList data(sel.data_.size());
    label counter(0);
    for (const label elemi : sel.data_)
    {
        data[counter++] = elemi;
    }

    os << nl << data;

    return os;
}


inline Foam::Istream& Foam::Module::operator>>
(
    Istream& is,
    Foam::Module::meshSubset& sel
)
{
    is.check(FUNCTION_NAME);

    labelList data;
    is >> sel.name_ >> sel.type_ >> data;

    sel.data_.clear();
    for (const label elemi : data)
    {
        sel.data_.insert(elemi);
    }

    is.check(FUNCTION_NAME);
    return is;
}


// ************************************************************************* //
