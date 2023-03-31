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

#include "boxScaling.H"
#include "addToRunTimeSelectionTable.H"
#include "boundBox.H"
#include "plane.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{
    defineTypeNameAndDebug(boxScaling, 0);
    addToRunTimeSelectionTable(coordinateModification, boxScaling, dictionary);
}
}

// * * * * * * * * * * * * * * Private member functions* * * * * * * * * * * //

void Foam::Module::boxScaling::calculateBndBox()
{
    pMin_ = centre_ - 0.5*lengthVec_;
    pMax_ = centre_ + 0.5*lengthVec_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::boxScaling::boxScaling()
:
    coordinateModification(),
    centre_(),
    lengthVec_(0.0, 0.0, 0.0),
    scaleVec_(1.0, 1.0, 1.0),
    pMin_(),
    pMax_()
{
    calculateBndBox();
}


Foam::Module::boxScaling::boxScaling
(
    const word& name,
    const point& centre,
    const scalar lengthX,
    const scalar lengthY,
    const scalar lengthZ,
    const scalar scaleX,
    const scalar scaleY,
    const scalar scaleZ
)
:
    coordinateModification(),
    centre_(centre),
    lengthVec_(lengthX, lengthY, lengthZ),
    scaleVec_(scaleX, scaleY, scaleZ),
    pMin_(),
    pMax_()
{
    calculateBndBox();
    setName(name);
}


Foam::Module::boxScaling::boxScaling
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

Foam::point Foam::Module::boxScaling::origin() const
{
    return centre_;
}


void Foam::Module::boxScaling::translateAndModifyObject(const vector& disp)
{
    centre_ += disp;

    for (direction i = 0; i < vector::nComponents; ++i)
    {
        lengthVec_[i] /= scaleVec_[i];
    }

    calculateBndBox();
}


Foam::vector Foam::Module::boxScaling::displacement
(
    const point& p
) const
{
    vector disp;

    for (direction i = 0; i < vector::nComponents; ++i)
    {
        const scalar dispVec = lengthVec_[i] * ((1.0/scaleVec_[i]) - 1.0);
        const scalar t = ((p[i] - pMin_[i]) / lengthVec_[i]);

        const scalar tBnd = Foam::max(0.0, Foam::min(t, 1.0));

        disp[i] = tBnd*dispVec;
    }

    return disp;
}


Foam::vector Foam::Module::boxScaling::backwardDisplacement
(
    const point& p
) const
{
    vector disp;

    for (direction i = 0; i < vector::nComponents; ++i)
    {
        const scalar dispVec = lengthVec_[i] * (scaleVec_[i] - 1.0);

        const scalar t = ((p[i] - pMin_[i]) / lengthVec_[i]);

        const scalar tBnd = Foam::max(0.0, Foam::min(t, 1.0));

        disp[i] = tBnd*dispVec;
    }

    return disp;
}


bool Foam::Module::boxScaling::combiningPossible() const
{
    return true;
}


void Foam::Module::boxScaling::boundingPlanes(PtrList<plane>&pl) const
{
    pl.setSize(6);
    label counter(0);
    if (Foam::mag(scaleVec_.x() - 1.0) > VSMALL)
    {
        pl.set(counter++, new plane(pMin_, vector(1, 0, 0)));
        pl.set(counter++, new plane(pMax_, vector(1, 0, 1)));
    }

    if (Foam::mag(scaleVec_.y() - 1.0) > VSMALL)
    {
        pl.set(counter++, new plane(pMin_, vector(0, 1, 0)));
        pl.set(counter++, new plane(pMax_, vector(0, 1, 0)));
    }

    if (Foam::mag(scaleVec_.z() - 1.0) > VSMALL)
    {
        pl.set(counter++, new plane(pMin_, vector(0, 0, 1)));
        pl.set(counter++, new plane(pMax_, vector(0, 0, 1)));
    }

    pl.setSize(counter);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::dictionary Foam::Module::boxScaling::dict(bool /*ignoreType*/) const
{
    dictionary dict;

    dict.add("type", type());

    dict.add("centre", centre_);
    dict.add("lengthX", lengthVec_.x());
    dict.add("lengthY", lengthVec_.y());
    dict.add("lengthZ", lengthVec_.z());

    dict.add("scaleX", scaleVec_.x());
    dict.add("scaleY", scaleVec_.y());
    dict.add("scaleZ", scaleVec_.z());

    return dict;
}


void Foam::Module::boxScaling::write(Ostream& os) const
{
    os  << " type:   " << type()
        << " centre: " << centre_
        << " lengthX: " << lengthVec_.x()
        << " lengthY: " << lengthVec_.y()
        << " lengthZ: " << lengthVec_.z()
        << " scaleX:  " << scaleVec_.x()
        << " scaleY:  " << scaleVec_.y()
        << " scaleZ:  " << scaleVec_.z()
        << endl;
}


void Foam::Module::boxScaling::writeDict(Ostream& os, bool subDict) const
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

    os.writeEntry("centre", centre_);
    os.writeEntry("lengthX", lengthVec_.x());
    os.writeEntry("lengthY", lengthVec_.y());
    os.writeEntry("lengthZ", lengthVec_.z());
    os.writeEntry("scaleX", scaleVec_.x());
    os.writeEntry("scaleY", scaleVec_.y());
    os.writeEntry("scaleZ", scaleVec_.z());

    if (subDict)
    {
        os << decrIndent << indent << token::END_BLOCK << endl;
    }
}


void Foam::Module::boxScaling::operator=(const dictionary& d)
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

    if (!dict.readIfPresent("lengthX", lengthVec_.x()))
    {
        FatalErrorInFunction
            << "Entry lengthX is not specified!" << exit(FatalError);
        lengthVec_.x() = 0.0;
    }

    if (!dict.readIfPresent("lengthY", lengthVec_.y()))
    {
        FatalErrorInFunction
            << "Entry lengthY is not specified!" << exit(FatalError);
        lengthVec_.y() = 0.0;
    }

    if (!dict.readIfPresent("lengthZ", lengthVec_.z()))
    {
        FatalErrorInFunction
            << "Entry lengthZ is not specified!" << exit(FatalError);
        lengthVec_.z() = 0.0;
    }

    scaleVec_.x() = dict.lookupOrDefault<scalar>("scaleX", 1.0);
    scaleVec_.y() = dict.lookupOrDefault<scalar>("scaleY", 1.0);
    scaleVec_.z() = dict.lookupOrDefault<scalar>("scaleZ", 1.0);

    calculateBndBox();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::Ostream& Foam::Module::boxScaling::operator<<(Ostream& os) const
{
    os << "name " << name() << nl;
    write(os);
    return os;
}


Foam::Ostream& Foam::Module::operator<<
(
    Ostream& os,
    const Foam::Module::boxScaling& bs
)
{
    return bs.operator<<(os);
}


// ************************************************************************* //
