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

#include "SubList.H"

// * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * * //

inline Foam::Module::cellListPMG::cellListPMG()
:
    cellList(),
    nElmts_(0)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline Foam::label Foam::Module::cellListPMG::size() const
{
    return nElmts_;
}


inline void Foam::Module::cellListPMG::setSize(const label nElmts)
{
    if (nElmts >= cellList::size())
    {
        if (cellList::size() != 0)
        {
            Info<< "Resizing cells!" << endl;
            cellList copy(label(1.5*nElmts));
            for (label i = 0; i < nElmts_; ++i)
            {
                copy[i].transfer(this->operator[](i));
            }

            cellList::transfer(copy);
        }
        else
        {
            cellList::setSize(label(1.5*nElmts));
        }
    }

    nElmts_ = nElmts;
}


inline void Foam::Module::cellListPMG::clear()
{
    nElmts_ = 0;
}


inline void Foam::Module::cellListPMG::append(const cell& c)
{
    const label i = nElmts_;
    setSize(i + 1);
    this->operator[](i) = c;
}


inline Foam::cell& Foam::Module::cellListPMG::newElmt(const label cI)
{
    setSize(cI + 1);
    return this->operator[](cI);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline void Foam::Module::cellListPMG::operator=(const cellList& cls)
{
    setSize(cls.size());
    forAll(cls, cI)
    {
        this->operator[](cI) = cls[cI];
    }
}


inline Foam::Ostream& Foam::Module::operator<<
(
    Ostream& os,
    const Foam::Module::cellListPMG& cls
)
{
    SubList<cell> c(cls, cls.nElmts_, 0);

    os << c;
    return os;
}


inline Foam::Istream& Foam::Module::operator>>
(
    Istream& is,
    Foam::Module::cellListPMG& cls
)
{
    cellList& cells = static_cast<cellList&>(cls);
    is >> cells;
    cls.nElmts_ = cells.size();

    return is;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
