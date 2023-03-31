/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | avalanche module
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017 Matthias Rauter
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

#include "frictionModel.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(frictionModel, 0);
    defineRunTimeSelectionTable(frictionModel, dictionary);
}

// file-local
static const Foam::dimensionSet dimTauSc(0, 2, -2, 0, 0, 0, 0);


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void Foam::frictionModel::readDict
(
    const word& type,
    const dictionary& dict
)
{
    frictionProperties_ = dict;
    coeffDict_ = frictionProperties_.optionalSubDict(type + "Coeffs");
}


void Foam::frictionModel::resetTauSc() const
{
    tauSc_ = dimensionedVector(dimTauSc);
}


void Foam::frictionModel::resetTauSp() const
{
    tauSp_ = dimensionedScalar(dimVelocity);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::frictionModel::frictionModel
(
    const word& type,
    const dictionary& frictionProperties,
    const areaVectorField& Us,
    const areaScalarField& h,
    const areaScalarField& p
)
:
    frictionProperties_(frictionProperties),
    coeffDict_
    (
        frictionProperties_.optionalSubDict(type + "Coeffs")
    ),
    rho_("rho", dimDensity, frictionProperties_),
    u0_("u0", dimVelocity, frictionProperties_),
    h0_("h0", dimLength, frictionProperties_),
    Us_(Us),
    h_(h),
    p_(p),
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
        << "    " << rho_ << nl
        << "    " << u0_ << nl
        << "    " << h0_ << endl;
}


// ************************************************************************* //
