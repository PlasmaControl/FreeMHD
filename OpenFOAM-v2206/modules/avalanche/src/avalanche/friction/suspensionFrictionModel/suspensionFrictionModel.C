/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | avalanche module
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2020 Matthias Rauter
-------------------------------------------------------------------------------
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

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#include "suspensionFrictionModel.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(suspensionFrictionModel, 0);
    defineRunTimeSelectionTable(suspensionFrictionModel, dictionary);
}

// file-local
static const Foam::dimensionSet dimTauSc(0, 2, -2, 0, 0, 0, 0);


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void Foam::suspensionFrictionModel::readDict
(
    const word& type,
    const dictionary& dict
)
{
    suspensionFrictionProperties_ = dict;
    coeffDict_ = suspensionFrictionProperties_.optionalSubDict(type + "Coeffs");
}


void Foam::suspensionFrictionModel::resetTauSc() const
{
    tauSc_ = dimensionedVector(dimTauSc);
}


void Foam::suspensionFrictionModel::resetTauSp() const
{
    tauSp_ = dimensionedScalar(dimVelocity);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::suspensionFrictionModel::suspensionFrictionModel
(
    const word& type,
    const dictionary& suspensionFrictionProperties,
    const areaVectorField& Us,
    const areaScalarField& h,
    const areaScalarField& c
)
:
    suspensionFrictionProperties_(suspensionFrictionProperties),
    coeffDict_
    (
        suspensionFrictionProperties_.optionalSubDict(type + "Coeffs")
    ),
    u0_("u0", dimVelocity, suspensionFrictionProperties_),
    h0_("h0", dimLength, suspensionFrictionProperties_),
    Us_(Us),
    h_(h),
    c_(c),
    tauSp_
    (
        IOobject
        (
            "tauSp",
            Us_.time().timeName(),
            Us_.db(),
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        Us_.mesh(),
        dimensionedScalar(dimVelocity)
    ),
    tauSc_
    (
        IOobject
        (
            "tauSc",
            Us_.time().timeName(),
            Us_.db(),
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        Us_.mesh(),
        dimensionedVector(dimTauSc)
    )
{
    Info<< "    with " << nl
        << "    " << u0_ << nl
        << "    " << h0_ << endl;
}


// ************************************************************************* //
