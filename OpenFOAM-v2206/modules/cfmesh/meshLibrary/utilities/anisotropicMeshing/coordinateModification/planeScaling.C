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

#include "planeScaling.H"
#include "addToRunTimeSelectionTable.H"
#include "boundBox.H"
#include "plane.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{
    defineTypeNameAndDebug(planeScaling, 0);
    addToRunTimeSelectionTable
    (
        coordinateModification,
        planeScaling,
        dictionary
    );
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::planeScaling::planeScaling()
:
    coordinateModification(),
    origin_(vector::zero),
    normal_(1, 1, 1),
    scalingDistance_(0.0),
    scalingFactor_(1.0)
{}


Foam::Module::planeScaling::planeScaling
(
    const word& name,
    const point& origin,
    const vector& normal,
    const scalar scalingDistance,
    const scalar scalingFactor
)
:
    coordinateModification(),
    origin_(origin),
    normal_(normal/mag(normal)),
    scalingDistance_(scalingDistance),
    scalingFactor_(scalingFactor)
{
    if (scalingFactor_ < SMALL)
    {
        Warning << "Scaling factor for plane " << name << " is less than 0.0 "
                << endl;

        scalingFactor_ = 1.0;
    }

    setName(name);
}


Foam::Module::planeScaling::planeScaling
(
    const word& name,
    const dictionary& dict
)
:
    coordinateModification(name, dict)
{
    this->operator=(dict);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::point Foam::Module::planeScaling::origin() const
{
    return origin_;
}


void Foam::Module::planeScaling::translateAndModifyObject(const vector& disp)
{
    origin_ += disp;

    scalingDistance_ /= scalingFactor_;
}


Foam::vector Foam::Module::planeScaling::displacement
(
    const point& p
) const
{
    const scalar dist = (p - origin_) & normal_;

    const vector translationVec =
        normal_*scalingDistance_*((1.0/scalingFactor_) - 1.0);

    const scalar t = dist/scalingDistance_;

    const scalar tBnd = Foam::max(0.0, Foam::min(1.0, t));

    return tBnd*translationVec;
}


Foam::vector Foam::Module::planeScaling::backwardDisplacement
(
    const point& p
) const
{
    const scalar dist = (p - origin_) & normal_;

    const vector translationVec =
        normal_*scalingDistance_*(scalingFactor_ - 1.0);

    const scalar t = dist/scalingDistance_;

    const scalar tBnd = Foam::max(0.0, Foam::min(1.0, t));

    return tBnd*translationVec;
}


bool Foam::Module::planeScaling::combiningPossible() const
{
    return true;
}


void Foam::Module::planeScaling::boundingPlanes(PtrList<plane>& pl) const
{
    if (Foam::mag(scalingFactor_ - 1.0) > VSMALL)
    {
        pl.setSize(2);

        pl.set(0, new plane(origin_, normal_));
        pl.set(1, new plane(origin_ + scalingDistance_*normal_, normal_));
    }
    else
    {
        pl.clear();
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::dictionary Foam::Module::planeScaling::dict(bool /*ignoreType*/) const
{
    dictionary dict;

    dict.add("type", type());

    dict.add("origin", origin_);
    dict.add("normal", normal_);
    dict.add("scalingDistance", scalingDistance_);
    dict.add("scalingFactor", scalingFactor_);

    return dict;
}


void Foam::Module::planeScaling::write(Ostream& os) const
{
    os  << " type:   " << type()
        << " origin: " << origin_
        << " normal: " << normal_
        << " scalingDistance: " << scalingDistance_
        << " scalingFactor: " << scalingFactor_;
}


void Foam::Module::planeScaling::writeDict(Ostream& os, bool subDict) const
{
    if (subDict)
    {
        os << indent << token::BEGIN_BLOCK << incrIndent << nl;
    }

    // only write type for derived types
    if (type() != typeName_())
    {
        os.writeEntry("type", type());
    }

    os.writeEntry("origin", origin_);
    os.writeEntry("normal",  normal_);
    os.writeEntry("scalingDistance", scalingDistance_);
    os.writeEntry("scalingFactor", scalingFactor_);

    if (subDict)
    {
        os << decrIndent << indent << token::END_BLOCK << endl;
    }
}


void Foam::Module::planeScaling::operator=(const dictionary& d)
{
    // allow as embedded sub-dictionary "coordinateSystem"
    const dictionary& dict =
    (
        d.found(typeName_())
      ? d.subDict(typeName_())
      : d
    );

    if (!dict.readIfPresent("origin", origin_))
    {
        FatalErrorInFunction
            << "Entry origin is not specified!" << exit(FatalError);

        origin_ = vector::zero;
    }

    if (!dict.readIfPresent("normal", normal_))
    {
        FatalErrorInFunction
            << "Entry normal is not specified!" << exit(FatalError);
        normal_ = vector(1, 1, 1);
    }

    if (!dict.readIfPresent("scalingDistance", scalingDistance_))
    {
        FatalErrorInFunction
            << "Entry scalingDistance is not specified!" << exit(FatalError);

        scalingDistance_ = 0.0;
    }

    if (!dict.readIfPresent("scalingFactor", scalingFactor_))
    {
        WarningInFunction
            << "Entry scalingFactor is not specified!" << endl;

        scalingFactor_ = 1.0;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::Ostream& Foam::Module::planeScaling::operator<<
(
    Ostream& os
) const
{
    os << "name " << name() << nl;
    write(os);
    return os;
}


Foam::Ostream& Foam::Module::operator<<
(
    Ostream& os,
    const Foam::Module::planeScaling& pt
)
{
    return pt.operator<<(os);
}


// ************************************************************************* //
