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

inline Foam::Module::faceListPMG::faceListPMG(const IOobject& io)
:
    regIOobject(io),
    faceList(readStream(typeName)),
    nElmts_(faceList::size())
{}


inline Foam::Module::faceListPMG::faceListPMG
(
    const IOobject& io,
    const label s
)
:
    regIOobject(io),
    faceList(s),
    nElmts_(s)
{}


inline Foam::Module::faceListPMG::faceListPMG
(
    const IOobject& io,
    const faceList& fcs
)
:
    regIOobject(io),
    faceList(fcs),
    nElmts_(fcs.size())
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline Foam::label Foam::Module::faceListPMG::size() const
{
    return nElmts_;
}


inline void Foam::Module::faceListPMG::setSize(const label nElmts)
{
    if (nElmts >= faceList::size())
    {
        if (faceList::size() != 0)
        {
            Info<< "Resizing faces!" << endl;
            faceList copy(label(1.5*nElmts));
            for (label i = 0; i < nElmts_; ++i)
            {
                copy[i].transfer(this->operator[](i));
            }

            faceList::transfer(copy);
        }
        else
        {
            faceList::setSize(label(1.5*nElmts));
        }
    }

    nElmts_ = nElmts;
}


inline void Foam::Module::faceListPMG::clear()
{
    nElmts_ = 0;
}


inline void Foam::Module::faceListPMG::append(const face& f)
{
    const label i = nElmts_;
    setSize(i + 1);
    this->operator[](i) = f;
}


inline Foam::face& Foam::Module::faceListPMG::newElmt(const label fI)
{
    setSize(fI + 1);
    return this->operator[](fI);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline bool Foam::Module::faceListPMG::writeData(Ostream& os) const
{
    return (os << *this).good();
}


inline void Foam::Module::faceListPMG::operator=(const faceList& fcs)
{
    setSize(fcs.size());
    forAll(fcs, fI)
    {
        this->operator[](fI) = fcs[fI];
    }
}


inline Foam::Ostream& Foam::Module::operator<<
(
    Ostream& os,
    const Foam::Module::faceListPMG& fcs
)
{
    SubList<face> f(fcs, fcs.nElmts_, 0);

    os << f;
    return os;
}


inline Foam::Istream& Foam::Module::operator>>
(
    Istream& is,
    Foam::Module::faceListPMG& fcs
)
{
    faceList& faces = static_cast<faceList&>(fcs);
    is >> faces;
    fcs.nElmts_ = faces.size();

    return is;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
