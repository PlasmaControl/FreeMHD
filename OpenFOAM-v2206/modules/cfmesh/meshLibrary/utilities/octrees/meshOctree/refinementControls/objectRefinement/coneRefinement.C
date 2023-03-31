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

#include "coneRefinement.H"
#include "addToRunTimeSelectionTable.H"
#include "boundBox.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{
    defineTypeNameAndDebug(coneRefinement, 0);
    addToRunTimeSelectionTable
    (
        objectRefinement,
        coneRefinement,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::coneRefinement::coneRefinement()
:
    objectRefinement(),
    p0_(),
    r0_(-1.0),
    p1_(),
    r1_(-1.0)
{}


Foam::Module::coneRefinement::coneRefinement
(
    const word& name,
    const scalar cellSize,
    const direction additionalRefLevels,
    const point& p0,
    const scalar radius0,
    const point& p1,
    const scalar radius1
)
:
    objectRefinement(),
    p0_(p0),
    r0_(radius0),
    p1_(p1),
    r1_(radius1)
{
    setName(name);
    setCellSize(cellSize);
    setAdditionalRefinementLevels(additionalRefLevels);
}


Foam::Module::coneRefinement::coneRefinement
(
    const word& name,
    const dictionary& dict
)
:
    objectRefinement(name, dict)
{
    this->operator=(dict);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

bool Foam::Module::coneRefinement::intersectsObject(const boundBox& bb) const
{
    // check if the centre is inside the cone
    const point c = (bb.max() + bb.min()) / 2.0;

    const vector v = p1_ - p0_;
    const scalar d = magSqr(v);

    if (d < VSMALL)
        return false;

    const scalar t = ((c - p0_) & v) / d;
    if ((t > 1.0) || (t < 0.0))
        return false;

    const scalar r = r0_ + (r1_ - r0_)*t;

    if (mag(p0_ + t*v - c) < r)
        return true;

    return false;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::dictionary Foam::Module::coneRefinement::dict(bool /*ignoreType*/) const
{
    dictionary dict;

    if (additionalRefinementLevels() == 0 && cellSize() >= 0.0)
    {
        dict.add("cellSize", cellSize());
    }
    else
    {
        dict.add("additionalRefinementLevels", additionalRefinementLevels());
    }

    dict.add("type", type());

    dict.add("p0", p0_);
    dict.add("radius0", r0_);
    dict.add("p1", p1_);
    dict.add("radius1", r1_);

    return dict;
}


void Foam::Module::coneRefinement::write(Ostream& os) const
{
    os  << " type:   " << type()
        << " p0: " << p0_
        << " radius0: " << r0_
        << " p1: " << p1_
        << " radius1: " << r1_;
}


void Foam::Module::coneRefinement::writeDict(Ostream& os, bool subDict) const
{
    if (subDict)
    {
        os << indent << token::BEGIN_BLOCK << incrIndent << nl;
    }

    if (additionalRefinementLevels() == 0 && cellSize() >= 0.0)
    {
        os.writeKeyword("cellSize") << cellSize() << token::END_STATEMENT << nl;
    }
    else
    {
        os.writeKeyword("additionalRefinementLevels")
            << additionalRefinementLevels()
            << token::END_STATEMENT << nl;
    }

    // only write type for derived types
    if (type() != typeName_())
    {
        os.writeKeyword("type") << type() << token::END_STATEMENT << nl;
    }

    os.writeKeyword("p0") << p0_ << token::END_STATEMENT << nl;
    os.writeKeyword("radius0") << r0_ << token::END_STATEMENT << nl;
    os.writeKeyword("p1") << p1_ << token::END_STATEMENT << nl;
    os.writeKeyword("radius1") << r1_ << token::END_STATEMENT << nl;

    if (subDict)
    {
        os << decrIndent << indent << token::END_BLOCK << endl;
    }
}


void Foam::Module::coneRefinement::operator=(const dictionary& d)
{
    // allow as embedded sub-dictionary "coordinateSystem"
    const dictionary& dict =
    (
        d.found(typeName_())
      ? d.subDict(typeName_())
      : d
    );

    if (!dict.readIfPresent("p0", p0_))
    {
        FatalErrorInFunction
            << "Entry p0 is not specified!" << exit(FatalError);

        p0_ = vector::zero;
    }

    if (!dict.readIfPresent("radius0", r0_))
    {
        FatalErrorInFunction
            << "Entry radius0 is not specified!" << exit(FatalError);

        r0_ = -1.0;
    }

    if (!dict.readIfPresent("p1", p1_))
    {
        FatalErrorInFunction
            << "Entry p1 is not specified!" << exit(FatalError);

        p1_ = vector::zero;
    }

    if (!dict.readIfPresent("radius1", r1_))
    {
        FatalErrorInFunction
            << "Entry radius1 is not specified!" << exit(FatalError);

        r1_ = -1.0;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::Ostream& Foam::Module::coneRefinement::operator<<
(
    Ostream& os
) const
{
    os << "name " << name() << nl;
    os << "cell size " << cellSize() << nl;
    os << "additionalRefinementLevels " << additionalRefinementLevels() << endl;

    write(os);
    return os;
}


// ************************************************************************* //
