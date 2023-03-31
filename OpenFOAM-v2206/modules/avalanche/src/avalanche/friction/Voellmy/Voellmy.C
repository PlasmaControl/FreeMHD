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

#include "Voellmy.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace frictionModels
{
    defineTypeNameAndDebug(Voellmy, 0);
    addToRunTimeSelectionTable(frictionModel, Voellmy, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::frictionModels::Voellmy::Voellmy
(
    const dictionary& frictionProperties,
    const areaVectorField& Us,
    const areaScalarField& h,
    const areaScalarField& p
)
:
    frictionModel(type(), frictionProperties, Us, h, p),
    mu_("mu", dimless, coeffDict_),
    xi_("xi", dimAcceleration, coeffDict_)
{
    Info<< "    " << mu_ << nl
        << "    " << xi_ << nl << endl;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const Foam::areaVectorField& Foam::frictionModels::Voellmy::tauSc() const
{
    resetTauSc();

    return tauSc_;
}


const Foam::areaScalarField& Foam::frictionModels::Voellmy::tauSp() const
{
    resetTauSp();

    areaScalarField u(mag(Us_));
    dimensionedScalar lambda
    (
        rho_
      * dimensionedScalar("g", dimAcceleration, 9.81)
      / xi_
    );

    // Coulomb Part
    tauSp_ += 1./rho_ * p_ * mu_ *
              1./(u + u0_);

    // Kinetic Part
    tauSp_ += 1./rho_ * lambda * u;

    return tauSp_;
}


bool Foam::frictionModels::Voellmy::read
(
    const dictionary& frictionProperties
)
{
    readDict(type(), frictionProperties);

    //frictionProperties_.readEntry("nu", nu0_);

    return true;
}


// ************************************************************************* //
