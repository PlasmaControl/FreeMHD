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

#include "SubList.H"

// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

inline Foam::Module::pointFieldPMG::pointFieldPMG(const IOobject& io)
:
    regIOobject(io),
    pointField(readStream(typeName)),
    nElmts_(pointField::size())
{
    close();
}


inline Foam::Module:: pointFieldPMG::pointFieldPMG
(
    const IOobject& io,
    const label s
)
:
    regIOobject(io),
    pointField(s),
    nElmts_(s)
{}


inline Foam::Module::pointFieldPMG::pointFieldPMG
(
    const IOobject& io,
    const pointField& pts
)
:
    regIOobject(io),
    pointField(pts),
    nElmts_(pts.size())
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline Foam::label Foam::Module::pointFieldPMG::size() const
{
    return nElmts_;
}


inline void Foam::Module::pointFieldPMG::setSize(const label nElmts)
{
    if (nElmts >= pointField::size())
    {
        Info<< "Resizing points!" << endl;
        pointField::setSize(label(1.5*nElmts)+1);
    }

    nElmts_ = nElmts;
}


inline void Foam::Module::pointFieldPMG::reserve(const label capacity)
{
    if (capacity > size())
    {
        this->setSize(capacity);
    }
}


inline void Foam::Module::pointFieldPMG::clear()
{
    nElmts_ = 0;
}


inline void Foam::Module::pointFieldPMG::append(const point& p)
{
    const label i = nElmts_;
    setSize(i + 1);
    this->operator[](i) = p;
}


inline Foam::point& Foam::Module::pointFieldPMG::newElmt(const label pI)
{
    setSize(pI + 1);
    return this->operator[](pI);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline bool Foam::Module::pointFieldPMG::writeData(Ostream& os) const
{
    return (os << *this).good();
}


inline void Foam::Module::pointFieldPMG::operator=(const pointField& pts)
{
    setSize(pts.size());
    forAll(pts, pI)
    {
        this->operator[](pI) = pts[pI];
    }
}


inline Foam::Ostream& Foam::Module::operator<<
(
    Ostream& os,
    const Foam::Module::pointFieldPMG& pts
)
{
    SubList<point> p(pts, pts.nElmts_, 0);

    os << p;
    return os;
}


inline Foam::Istream& Foam::Module::operator>>
(
    Istream& is,
    Foam::Module::pointFieldPMG& pts
)
{
    pointField& points = static_cast<pointField&>(pts);
    is >> points;
    pts.nElmts_ = points.size();

    return is;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
