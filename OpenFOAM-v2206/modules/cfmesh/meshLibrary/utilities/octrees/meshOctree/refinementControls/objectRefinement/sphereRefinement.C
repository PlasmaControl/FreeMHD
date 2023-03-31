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

#include "sphereRefinement.H"
#include "addToRunTimeSelectionTable.H"
#include "boundBox.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

    defineTypeNameAndDebug(sphereRefinement, 0);
    addToRunTimeSelectionTable
    (
        objectRefinement,
        sphereRefinement,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::sphereRefinement::sphereRefinement()
:
    objectRefinement(),
    centre_(),
    radius_(-1.0)
{}


Foam::Module::sphereRefinement::sphereRefinement
(
    const word& name,
    const scalar cellSize,
    const direction additionalRefLevels,
    const point& centre,
    const scalar radius
)
:
    objectRefinement(),
    centre_(centre),
    radius_(radius)
{
    setName(name);
    setCellSize(cellSize);
    setAdditionalRefinementLevels(additionalRefLevels);
}


Foam::Module::sphereRefinement::sphereRefinement
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

bool Foam::Module::sphereRefinement::intersectsObject(const boundBox& bb) const
{
    const point c = (bb.max() + bb.min()) / 2.0;

    if (magSqr(c - centre_) < sqr(radius_))
        return true;

    return false;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::dictionary
Foam::Module::sphereRefinement::dict(bool /*ignoreType*/) const
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

    dict.add("centre", centre_);
    dict.add("radius", radius_);

    return dict;
}


void Foam::Module::sphereRefinement::write(Ostream& os) const
{
    os  << " type:   " << type()
        << " centre: " << centre_
        << " radius: " << radius_;
}


void Foam::Module::sphereRefinement::writeDict
(
    Ostream& os,
    bool subDict
) const
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

    os.writeKeyword("centre") << centre_ << token::END_STATEMENT << nl;
    os.writeKeyword("radius") << radius_ << token::END_STATEMENT << nl;

    if (subDict)
    {
        os << decrIndent << indent << token::END_BLOCK << endl;
    }
}


void Foam::Module::sphereRefinement::operator=(const dictionary& d)
{
    // allow as embedded sub-dictionary "coordinateSystem"
    const dictionary& dict =
    (
        d.found(typeName_())
      ? d.subDict(typeName_())
      : d
    );

    // unspecified centre is (0 0 0)
    if (!dict.readIfPresent("centre", centre_))
    {
        FatalErrorInFunction
            << "Entry centre is not specified!" << exit(FatalError);

        centre_ = vector::zero;
    }

    if (!dict.readIfPresent("radius", radius_))
    {
        FatalErrorInFunction
            << "Entry radius is not specified!" << exit(FatalError);

        radius_ = -1.0;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::Ostream& Foam::Module::sphereRefinement::operator<<
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
