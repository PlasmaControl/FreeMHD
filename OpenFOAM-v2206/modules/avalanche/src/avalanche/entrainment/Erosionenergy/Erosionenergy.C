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
#include "Erosionenergy.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace entrainmentModels
{
    defineTypeNameAndDebug(Erosionenergy, 0);
    addToRunTimeSelectionTable(entrainmentModel, Erosionenergy, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::entrainmentModels::Erosionenergy::Erosionenergy
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
    eb_("eb", sqr(dimLength/dimTime), coeffDict_),
    gs_(Us_.db().lookupObject<areaVectorField>("gs")),
    gn_(Us_.db().lookupObject<areaScalarField>("gn"))
{
    Info << "    " << eb_ << nl << endl;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const Foam::areaScalarField&
Foam::entrainmentModels::Erosionenergy::Sm() const
{
    Sm_ = (tau_&Us_)/eb_/rho_;
    Sm_ = min(Sm_, hentrain_/Us_.db().time().deltaT());

    return Sm_;
}


bool Foam::entrainmentModels::Erosionenergy::read
(
    const dictionary& entrainmentProperties
)
{
    readDict(type(), entrainmentProperties);

    coeffDict_.readEntry("eb", eb_);

    return true;
}


// ************************************************************************* //
