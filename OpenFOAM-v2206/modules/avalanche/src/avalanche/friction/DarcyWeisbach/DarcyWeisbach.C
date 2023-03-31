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

#include "DarcyWeisbach.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace frictionModels
{
    defineTypeNameAndDebug(DarcyWeisbach, 0);
    addToRunTimeSelectionTable(frictionModel, DarcyWeisbach, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::frictionModels::DarcyWeisbach::DarcyWeisbach
(
    const dictionary& frictionProperties,
    const areaVectorField& Us,
    const areaScalarField& h,
    const areaScalarField& p
)
:
    frictionModel(type(), frictionProperties, Us, h, p),
    Cf_("Cf", coeffDict_),
    g_("g", coeffDict_)
{
    Info<< "    " << Cf_ << nl << endl;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const Foam::areaVectorField&
Foam::frictionModels::DarcyWeisbach::tauSc() const
{
//tauSc_.internalField() = vector(0, 0, 0);

    return tauSc_;
}


const Foam::areaScalarField&
Foam::frictionModels::DarcyWeisbach::tauSp() const
{
    resetTauSp();

    tauSp_ += Cf_ * g_ * mag(Us_);

    return tauSp_;
}


bool Foam::frictionModels::DarcyWeisbach::read
(
    const dictionary& frictionProperties
)
{
    readDict(type(), frictionProperties);

    return true;
}


// ************************************************************************* //
