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

#include "fvCFD.H"
#include "faCFD.H"
#include "suspensionParkerFukushimaEntrainment.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace suspensionEntrainmentModels
{
    defineTypeNameAndDebug(suspensionParkerFukushimaEntrainment, 0);
    addToRunTimeSelectionTable(suspensionEntrainmentModel, suspensionParkerFukushimaEntrainment, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::suspensionEntrainmentModels::suspensionParkerFukushimaEntrainment::suspensionParkerFukushimaEntrainment
(
    const dictionary& entrainmentProperties,
    const areaVectorField& Us,
    const areaScalarField& h,
    const areaScalarField& hentrain,
    const areaScalarField& c,
    const areaVectorField& tau
)
:
    suspensionEntrainmentModel(type(), entrainmentProperties, Us, h, hentrain, c, tau),
    R_("R", dimless, coeffDict_),
    Ds_("Ds", dimLength, coeffDict_),
    nu_("nu", dimViscosity, coeffDict_),
    Zm_("Zm", dimless, coeffDict_),
    Zc_("Zc", dimless, coeffDict_),
    gs_(Us_.db().lookupObject<areaVectorField>("gs")),
    gn_(Us_.db().lookupObject<areaScalarField>("gn")),
    Rp_(sqrt(R_*gn_*Ds_)*Ds_/nu_), //Parker et al. (1986), Eq. (24)
    vs_(R_*gn_*Ds_*Ds_/18./nu_) //Terminal velocity from wikipedia.org
{
    Info << "    " << R_  << nl
         << "    " << Ds_ << nl
         << "    " << nu_ << nl
         << "    " << Zm_ << nl
         << "    " << Zc_ << nl << endl;

}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const Foam::areaScalarField&
Foam::suspensionEntrainmentModels::suspensionParkerFukushimaEntrainment::Sm() const
{
    areaScalarField mu(sqrt(mag(tau_))/vs_); //Parker et al. (1986), Eq. (21)
    areaScalarField Z(sqrt(Rp_)*mu); //Parker et al. (1986), Eq. (23)

    //Parker et al. (1986), Eq. (22) -> Es
    //The additional terminal velocity comes from Eq. (4)

    /*
    forAll(Sm_, i)
    {
        if (Z[i] > Zm_.value())
             Sm_[i] = vs_[i]*0.3;
        else if (Z[i] > Zc_.value())
            Sm_[i] = vs_[i]*3e-12*pow(Z[i],10)*(1-Zc_.value()/(Z[i]+VSMALL));
        else
            Sm_[i] = 0;
    }
    */

    Sm_ = vs_*(
         0.3*pos(Z-Zm_)+
         3e-12*pow(Z,10)*(1-Zc_/(Z+VSMALL))*pos(Z-Zc_)*pos(Zm_-Z)+
         0*pos(Zc_-Z)
         );

    return Sm_;
}


bool Foam::suspensionEntrainmentModels::suspensionParkerFukushimaEntrainment::read
(
    const dictionary& entrainmentProperties
)
{
    readDict(type(), entrainmentProperties);

    coeffDict_.readEntry("R", R_);
    coeffDict_.readEntry("Ds", Ds_);
    coeffDict_.readEntry("nu", nu_);
    coeffDict_.readEntry("Zm", Zm_);
    coeffDict_.readEntry("Zc", Zc_);

    return true;
}


// ************************************************************************* //
