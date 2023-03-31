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

#include "suspensionEntrainmentModel.H"
#include "fvCFD.H"
#include "faCFD.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(suspensionEntrainmentModel, 0);
    defineRunTimeSelectionTable(suspensionEntrainmentModel, dictionary);
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void Foam::suspensionEntrainmentModel::readDict
(
    const word& type,
    const dictionary& dict
)
{
    suspensionEntrainmentProperties_ = dict;
    coeffDict_ = suspensionEntrainmentProperties_.optionalSubDict(type + "Coeffs");
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::suspensionEntrainmentModel::suspensionEntrainmentModel
(
    const word& type,
    const dictionary& suspensionEntrainmentProperties,
    const areaVectorField& Us,
    const areaScalarField& h,
    const areaScalarField& hentrain,
    const areaScalarField& c,
    const areaVectorField& tau
)
:
    suspensionEntrainmentProperties_(suspensionEntrainmentProperties),
    coeffDict_
    (
        suspensionEntrainmentProperties_.optionalSubDict(type + "Coeffs")
    ),
    Us_(Us),
    h_(h),
    hentrain_(hentrain),
    c_(c),
    tau_(tau),
    Sm_
    (
        IOobject
        (
            "Sm",
            Us_.time().timeName(),
            Us_.db(),
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        Us_.mesh(),
        dimensionedScalar(dimVelocity)
    )
{
    Info<< "    with " << nl << endl;
}


// ************************************************************************* //
