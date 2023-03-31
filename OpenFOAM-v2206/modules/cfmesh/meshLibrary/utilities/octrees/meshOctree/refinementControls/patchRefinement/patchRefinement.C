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

#include "patchRefinement.H"
#include "triSurf.H"
#include "Istream.H"
#include "demandDrivenData.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::patchRefinement::patchRefinement()
:
    patchName_(),
    cellSize_(0.0)
{}


Foam::Module::patchRefinement::patchRefinement(const word& n, const scalar s)
:
    patchName_(n),
    cellSize_(s)
{}


Foam::Module::patchRefinement::patchRefinement(Istream& is)
:
    patchName_(is),
    cellSize_(readScalar(is))
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

const Foam::word& Foam::Module::patchRefinement::patchName() const
{
    return patchName_;
}


Foam::scalar Foam::Module::patchRefinement::cellSize() const
{
    return cellSize_;
}


Foam::label
Foam::Module::patchRefinement::patchInSurface(const triSurf& ts) const
{
    forAll(ts.patches(), patchI)
    {
        if (ts.patches()[patchI].name() == patchName_)
            return patchI;
    }

    FatalErrorInFunction
        << "Patch " << patchName_
        << " does not exist in surface " << ts.patches()
        << exit(FatalError);

    return -1;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::Istream& Foam::Module::operator>>
(
    Istream& is,
    Foam::Module::patchRefinement& pr
)
{
    pr.patchName_ = word(is);
    pr.cellSize_ = readScalar(is);
    return is;
}


Foam::Ostream& Foam::Module::operator<<
(
    Ostream& os,
    const Foam::Module::patchRefinement& pr
)
{
    os << pr.patchName_ << token::SPACE << pr.cellSize_ << nl;
    return os;
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

bool Foam::Module::operator==
(
    const Foam::Module::patchRefinement& lhs,
    const Foam::Module::patchRefinement& rhs
)
{
    return (lhs.patchName() == rhs.patchName());
}


bool Foam::Module::operator!=
(
    const Foam::Module::patchRefinement& lhs,
    const Foam::Module::patchRefinement& rhs
)
{
    return !(lhs == rhs);
}


// ************************************************************************* //
