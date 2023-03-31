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
#include "Front.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace entrainmentModels
{
    defineTypeNameAndDebug(Front, 0);
    addToRunTimeSelectionTable(entrainmentModel, Front, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::entrainmentModels::Front::Front
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
    htrigger_("htrigger", coeffDict_)
{
    Info << "    " << htrigger_ << endl << endl;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const Foam::areaScalarField&
Foam::entrainmentModels::Front::Sm() const
{
    const areaScalarField eflag(pos(h_-htrigger_));

    Sm_ = eflag*hentrain_/Us_.db().time().deltaT();

    return Sm_;
}


bool Foam::entrainmentModels::Front::read
(
    const dictionary& entrainmentProperties
)
{
    readDict(type(), entrainmentProperties);

    coeffDict_.readEntry("htrigger", htrigger_);

    return true;
}


// ************************************************************************* //
