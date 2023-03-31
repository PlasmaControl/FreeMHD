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
#include "Medina.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace entrainmentModels
{
    defineTypeNameAndDebug(Medina, 0);
    addToRunTimeSelectionTable(entrainmentModel, Medina, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::entrainmentModels::Medina::Medina
(
    const dictionary& entrainmentProperties,
    const areaVectorField& Us,
    const areaScalarField& h,
    const areaScalarField& hentrain,
    const areaScalarField& pb,
    const areaVectorField& tau
)
:
    entrainmentModel(type(), entrainmentProperties, Us, h, hentrain, pb, tau),
    tauc_("tauc", coeffDict_),
    mu_("mu", coeffDict_),
    relaxationFactor_(coeffDict_.getOrDefault<scalar>("relax", 1.)),
    gs_(Us_.db().lookupObject<areaVectorField>("gs")),
    gn_(Us_.db().lookupObject<areaScalarField>("gn"))
{
    Info << "    " << tauc_ << nl
         << "    " << mu_ << nl << endl;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const Foam::areaScalarField&
Foam::entrainmentModels::Medina::Sm() const
{
    areaScalarField taures(tauc_ + pb_.oldTime()*mu_);
    //no entrainment in "dry" areas:
    areaScalarField eflag(pos(h_-dimensionedScalar("small", dimLength, 1e-2)));
    areaScalarField eflag2(pos(mag(tau_.oldTime()) - taures));
    areaScalarField hent(eflag*eflag2*(mag(tau_.oldTime()) - taures)/(rho_*max(gn_*mu_-mag(gs_), dimensionedScalar("small", dimAcceleration, 1e-2))));

    Sm_ = hent/Us_.db().time().deltaT() * relaxationFactor_;

    Sm_ = max(Sm_, dimensionedScalar(dimVelocity));
    Sm_ = min(Sm_, hentrain_/Us_.db().time().deltaT());

    return Sm_;
}


bool Foam::entrainmentModels::Medina::read
(
    const dictionary& entrainmentProperties
)
{
    readDict(type(), entrainmentProperties);

    coeffDict_.readEntry("tauc", tauc_);
    coeffDict_.readEntry("mu", mu_);

    return true;
}


// ************************************************************************* //
