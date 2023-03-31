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

#include "fvCFD.H"
#include "faCFD.H"
#include "Stoppingprofile.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace depositionModels
{
    defineTypeNameAndDebug(Stoppingprofile, 0);
    addToRunTimeSelectionTable(depositionModel, Stoppingprofile, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::depositionModels::Stoppingprofile::Stoppingprofile
(
    const dictionary& depositionProperties,
    const areaVectorField& Us,
    const areaScalarField& h,
    const areaScalarField& hentrain,
    const areaScalarField& pb,
    const areaVectorField& tau
)
:
    depositionModel(type(), depositionProperties, Us, h, hentrain, pb, tau),
    ud_("ud", coeffDict_),
    ad_("ad", coeffDict_),
    gs_(Us.db().lookupObject<areaVectorField>("gs")),
    gn_(Us.db().lookupObject<areaScalarField>("gn"))
{
    Info<< "    " << ud_ << nl
        << "    " << ad_ << nl << endl;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const Foam::areaScalarField&
Foam::depositionModels::Stoppingprofile::Sd() const
{
    const dimensionedScalar smallVel("small", dimVelocity, SMALL);

    areaScalarField a
    (
        pow
        (
            max(min(scalar(1) - mag(Us_)/ud_, scalar(1)), scalar(0)),
            ad_
        )
    );

    areaScalarField hUddt
    (
        (-tau_/rho_+gs_*h_-fac::grad(pb_*h_/(2.*rho_)))
      & Us_.oldTime()/(mag(Us_.oldTime()) + smallVel)
    );

    hUddt = min(hUddt, dimensionedScalar(hUddt.dimensions(), Zero));
    Sd_ = -a/(mag(Us_.oldTime())+smallVel)*hUddt;
    Sd_ = max(Sd_, dimensionedScalar(dimVelocity));
    Sd_ = min(Sd_, h_/Us_.db().time().deltaT());

    forAll(Sd_, i)
    {
        if (mag(Us_.oldTime()[i]) < VSMALL)
        {
            Sd_[i] = Zero;
        }
    }
    return Sd_;
}


bool Foam::depositionModels::Stoppingprofile::read
(
    const dictionary& depositionProperties
)
{
    readDict(type(), depositionProperties);

    coeffDict_.readEntry("ud", ud_);
    coeffDict_.readEntry("ad", ad_);

    return true;
}


// ************************************************************************* //
