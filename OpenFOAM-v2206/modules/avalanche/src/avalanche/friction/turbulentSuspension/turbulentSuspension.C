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

#include "turbulentSuspension.H"
#include "addToRunTimeSelectionTable.H"
#include "fvCFD.H"
#include "faCFD.H"



// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace suspensionFrictionModels
{
    defineTypeNameAndDebug(turbulentSuspension, 0);
    addToRunTimeSelectionTable(suspensionFrictionModel, turbulentSuspension, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::suspensionFrictionModels::turbulentSuspension::turbulentSuspension
(
    const dictionary& suspensionFrictionProperties,
    const areaVectorField& Us,
    const areaScalarField& h,
    const areaScalarField& c
)
:
    suspensionFrictionModel(type(), suspensionFrictionProperties, Us, h, c),
    alpha_("alpha", coeffDict_),
    betaConst_("beta", coeffDict_.getOrDefault<dimensionedScalar>("beta", -1)),
    cd_("cd", coeffDict_),
    R_("R", coeffDict_),
    Ds_("Ds", coeffDict_),
    nu_("nu", coeffDict_),
    kmin_("kmin", coeffDict_),
    k_
    (
        IOobject
        (
            "k",
            Us_.time().timeName(),
            Us_.db(),
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        Us_.mesh()
    ),
    phi2s_(Us_.db().lookupObject<edgeScalarField>("phi2s")),//get mass flux
    tau_(Us_.db().lookupObject<areaVectorField>("tau")),//get friction
    es_(Us_.db().lookupObject<areaScalarField>("Sm")),//get sediment entrainment
    ds_(Us_.db().lookupObject<areaScalarField>("Sd")),//get sediment sink
    ew_(Us_.db().lookupObject<areaScalarField>("ew")),//get entrainment
    gs_(Us_.db().lookupObject<areaVectorField>("gs")),
    gn_(Us_.db().lookupObject<areaScalarField>("gn")),
    vs_(R_*gn_*Ds_*Ds_/18./nu_)
{
    Info<< "    " << alpha_ << nl
        << "    " << betaConst_ << nl
        << "    " << cd_ << nl
        << "    " << R_ << nl
        << "    " << nu_ << nl
        << "    " << kmin_ << nl << endl;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const Foam::areaVectorField&
Foam::suspensionFrictionModels::turbulentSuspension::tauSc() const
{
    resetTauSc();

    return tauSc_;
}


const Foam::areaScalarField&
Foam::suspensionFrictionModels::turbulentSuspension::tauSp() const
{
    resetTauSp();

    //const areaScalarField eps0kr = beta_*sqrt(k_)/(h_+h0_);

    const dimensionedScalar u0("u0", dimVelocity, 1e-5);
    const dimensionedScalar h0("h0", dimLength, 1e-5);
    //Parker et al. (1986), Eq. (10)
    const areaScalarField Ri(R_*gn_*c_*h_/sqr(mag(Us_)+u0));
     //Parker et al. (1986), Eq. (31). Note that ew is different by a factor Us
    areaScalarField beta
    (
        (1./2.*ew_/(mag(Us_)+u0)*(1.-Ri-2.*cd_/alpha_)+cd_)
      / pow(cd_/alpha_, 1.5)
    );

    beta.clip(1e-4, 1e4);

    if (betaConst_.value() > 0.)
    {
        beta.ref() = betaConst_;
    }
    //Parker et al. (1986), Eq. (8)
    faScalarMatrix kEqn
    (
        fam::ddt(h_, k_)
      + fam::div(phi2s_, k_)
     ==
        //mag(tau_)*mag(Us_)
        fam::Sp(alpha_*mag(Us_), k_) //implicit treatment of frictionc
      + 1./2.*pow(mag(Us_), 2.)*ew_
      - fam::Sp(beta*sqrt(k_), k_)
      - R_*gn_*vs_*h_*c_
      - 1./2.*R_*gn_*h_*c_*ew_
      - 1./2.*R_*gn_*h_*(es_-ds_)
    );

    kEqn.relax();

    solverPerformance kResidual = kEqn.solve();

    k_ = max(k_, kmin_);

    tauSp_ += alpha_*k_/(mag(Us_)+u0);

    return tauSp_;
}


bool Foam::suspensionFrictionModels::turbulentSuspension::read
(
    const dictionary& suspensionFrictionProperties
)
{
    readDict(type(), suspensionFrictionProperties);

    return true;
}


// ************************************************************************* //
