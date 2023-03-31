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

#include "muI.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace frictionModels
{
    defineTypeNameAndDebug(MuI, 0);
    addToRunTimeSelectionTable(frictionModel, MuI, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::frictionModels::MuI::MuI
(
    const dictionary& frictionProperties,
    const areaVectorField& Us,
    const areaScalarField& h,
    const areaScalarField& p
)
:
    frictionModel(type(), frictionProperties, Us, h, p),
    d_("d", dimLength, coeffDict_),
    rhop_("rho_p", dimDensity, coeffDict_),
    mus_("mu_s", dimless, coeffDict_),
    mu2_("mu_2", dimless, coeffDict_),
    I0_("I_0", dimless, coeffDict_),
    mu_
    (
        IOobject
        (
            "mu",
            Us_.time().timeName(),
            Us_.db(),
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        Us_.mesh(),
        dimensionedScalar(dimless)
    )
{
    Info<< "    " << d_ << nl
        << "    " << rhop_ << nl
        << "    " << mus_ << nl
        << "    " << mu2_ << nl
        << "    " << I0_ << nl << endl;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const Foam::areaVectorField& Foam::frictionModels::MuI::tauSc() const
{
    resetTauSc();

    return tauSc_;
}


const Foam::areaScalarField& Foam::frictionModels::MuI::tauSp() const
{
    resetTauSp();

    const areaScalarField u(mag(Us_));
    const areaScalarField gamma(5./2.*u/(h_+h0_));
    const areaScalarField I(gamma*d_/(sqrt(p_/rhop_)+u0_));

    mu_ = mus_ + (mu2_-mus_)/(I0_/(I+SMALL)+1.);

    tauSp_ += 1./rho_ * p_ * mu_ *
              1./(u + u0_);

    return tauSp_;
}


bool Foam::frictionModels::MuI::read
(
    const dictionary& frictionProperties
)
{
    readDict(type(), frictionProperties);

    return true;
}


// ************************************************************************* //
