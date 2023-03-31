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

#include "kt.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace frictionModels
{
    defineTypeNameAndDebug(kt, 0);
    addToRunTimeSelectionTable(frictionModel, kt, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::frictionModels::kt::kt
(
    const dictionary& frictionProperties,
    const areaVectorField& Us,
    const areaScalarField& h,
    const areaScalarField& p
)
:
    frictionModel(type(), frictionProperties, Us, h, p),
    mu_("mu", coeffDict_),
    chi_("chi", coeffDict_)
{
    Info<< "    " << mu_ << nl
        << "    " << chi_ << nl << endl;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const Foam::areaVectorField& Foam::frictionModels::kt::tauSc() const
{
    resetTauSc();

    return tauSc_;
}


const Foam::areaScalarField& Foam::frictionModels::kt::tauSp() const
{
    resetTauSp();

    areaScalarField u(mag(Us_));
    dimensionedScalar lambda
    (
        dimensionedScalar("g", dimAcceleration, 9.81)
      / chi_
    );

    // Coulomb Part
    tauSp_ += 1./rho_ * p_ * mu_ *
              1./(u + u0_);

    // Kinetic Part
    tauSp_ += lambda *
            u/(sqr(h_+h0_));

    return tauSp_;
}


bool Foam::frictionModels::kt::read
(
    const dictionary& frictionProperties
)
{
    readDict(type(), frictionProperties);

    return true;
}


// ************************************************************************* //
