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

#include "PoliquenForterre.H"
#include "unitConversion.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace frictionModels
{
    defineTypeNameAndDebug(PoliquenForterre, 0);
    addToRunTimeSelectionTable(frictionModel, PoliquenForterre, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::frictionModels::PoliquenForterre::PoliquenForterre
(
    const dictionary& frictionProperties,
    const areaVectorField& Us,
    const areaScalarField& h,
    const areaScalarField& p
)
:
    frictionModel(type(), frictionProperties, Us, h, p),
    L_("L", coeffDict_),
    beta_("beta", coeffDict_),
    zeta1_("zeta1", coeffDict_),
    zeta2_("zeta2", coeffDict_),
    zeta3_("zeta3", coeffDict_),
    gamma_("gamma", coeffDict_),
    mu_
    (
        IOobject
        (
            "mu",
            Us_.time().timeName(),
            Us_.db(),
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        Us_.mesh(),
        dimensionedScalar(dimless)
    ),
    g_
    (
        IOobject
        (
            "g",
            Us_.time().constant(),
            Us_.db(),
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    )
{
    // Report information in degrees
    Info<< "    " << L_ << nl
        << "    " << beta_ << nl
        << "    " << zeta1_ << nl
        << "    " << zeta2_ << nl
        << "    " << zeta3_ << nl << endl;

    zeta1_ *= degToRad();
    zeta2_ *= degToRad();
    zeta3_ *= degToRad();
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const Foam::areaVectorField&
Foam::frictionModels::PoliquenForterre::tauSc() const
{
    resetTauSc();

    return tauSc_;
}


const Foam::areaScalarField&
Foam::frictionModels::PoliquenForterre::tauSp() const
{
    resetTauSp();

    const areaVectorField& n = Us_.mesh().faceAreaNormals();
    const areaScalarField gn(g_ & n);
    const areaScalarField u(mag(Us_));

    const areaScalarField Fr((u+u0_)/sqrt((h_+h0_)*gn));
    const areaScalarField hs(h_*beta_/Fr);

    const areaScalarField mu_stop
    (
        tan(zeta1_)+(tan(zeta2_)-tan(zeta1_))/(1.+hs/L_)
    );
    const areaScalarField mu_start
    (
        tan(zeta3_)+(tan(zeta2_)-tan(zeta1_))*exp(-hs/L_)
    );

    mu_ = mu_stop;

    forAll(mu_, i)
    {
        if (Fr[i] < beta_.value())
        {
            mu_[i] =
                pow(Fr[i]/beta_.value(), gamma_.value())
              * (mu_stop[i]-mu_start[i]) + mu_start[i];
        }
    }


    tauSp_ += 1./rho_ * p_ * mu_ * 1./(u + u0_);

    return tauSp_;
}


bool Foam::frictionModels::PoliquenForterre::read
(
    const dictionary& frictionProperties
)
{
    readDict(type(), frictionProperties);

    return true;
}


// ************************************************************************* //
