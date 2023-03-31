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

#include "boxRefinement.H"
#include "addToRunTimeSelectionTable.H"
#include "boundBox.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{
    defineTypeNameAndDebug(boxRefinement, 0);
    addToRunTimeSelectionTable
    (
        objectRefinement,
        boxRefinement,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::boxRefinement::boxRefinement()
:
    objectRefinement(),
    centre_(),
    lengthX_(-1.0),
    lengthY_(-1.0),
    lengthZ_(-1.0)
{}


Foam::Module::boxRefinement::boxRefinement
(
    const word& name,
    const scalar cellSize,
    const direction additionalRefLevels,
    const point& centre,
    const scalar lengthX,
    const scalar lengthY,
    const scalar lengthZ
)
:
    objectRefinement(),
    centre_(centre),
    lengthX_(lengthX),
    lengthY_(lengthY),
    lengthZ_(lengthZ)
{
    setName(name);
    setCellSize(cellSize);
    setAdditionalRefinementLevels(additionalRefLevels);
}


Foam::Module::boxRefinement::boxRefinement
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

bool Foam::Module::boxRefinement::intersectsObject(const boundBox& bb) const
{
    vector v(0.5*lengthX_, 0.5*lengthY_, 0.5*lengthZ_);
    boundBox box(centre_ - v, centre_ + v);

    if (box.overlaps(bb))
        return true;

    return false;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::dictionary Foam::Module::boxRefinement::dict(bool /*ignoreType*/) const
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
    dict.add("lengthX", lengthX_);
    dict.add("lengthY", lengthY_);
    dict.add("lengthZ", lengthZ_);

    return dict;
}


void Foam::Module::boxRefinement::write(Ostream& os) const
{
    os  << " type:   " << type()
        << " centre: " << centre_
        << " lengthX: " << lengthX_
        << " lengthY: " << lengthY_
        << " lengthZ: " << lengthZ_;
}


void Foam::Module::boxRefinement::writeDict
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
    os.writeKeyword("lengthX") << lengthX_ << token::END_STATEMENT << nl;
    os.writeKeyword("lengthY") << lengthY_ << token::END_STATEMENT << nl;
    os.writeKeyword("lengthZ") << lengthZ_ << token::END_STATEMENT << nl;

    if (subDict)
    {
        os << decrIndent << indent << token::END_BLOCK << endl;
    }
}


void Foam::Module::boxRefinement::operator=(const dictionary& d)
{
    // allow as embedded sub-dictionary "coordinateSystem"
    const dictionary& dict =
    (
        d.found(typeName_())
      ? d.subDict(typeName_())
      : d
    );

    if (!dict.readIfPresent("centre", centre_))
    {
        FatalErrorInFunction
            << "Entry centre is not specified!" << exit(FatalError);

        centre_ = vector::zero;
    }

    if (!dict.readIfPresent("lengthX", lengthX_))
    {
        FatalErrorInFunction
            << "Entry lengthX is not specified!" << exit(FatalError);

        lengthX_ = -1.0;
    }

    if (!dict.readIfPresent("lengthY", lengthY_))
    {
        FatalErrorInFunction
            << "Entry lengthY is not specified!" << exit(FatalError);

        lengthY_ = -1.0;
    }

    if (!dict.readIfPresent("lengthZ", lengthZ_))
    {
        FatalErrorInFunction
            << "Entry lengthZ is not specified!" << exit(FatalError);

        lengthZ_ = -1.0;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::Ostream& Foam::Module::boxRefinement::operator<<
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
