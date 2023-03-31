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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::suspensionEntrainmentModel>
Foam::suspensionEntrainmentModel::New
(
    const dictionary& dict,
    const areaVectorField& Us,
    const areaScalarField& h,
    const areaScalarField& hentrain,
    const areaScalarField& c,
    const areaVectorField& tau
)
{
    const word modelName(dict.get<word>("suspensionEntrainmentModel"));

    Info<< "Selecting entrainment model " << modelName << endl;

    auto cstrIter = dictionaryConstructorTablePtr_->cfind(modelName);

    if (!cstrIter.found())
    {
        FatalIOErrorInLookup
        (
            dict,
            "suspensionEntrainmentModel",
            modelName,
            *dictionaryConstructorTablePtr_
        ) << exit(FatalIOError);
    }

    return autoPtr<suspensionEntrainmentModel>
    (
        cstrIter()(dict, Us, h, hentrain, c, tau)
    );
}


// ************************************************************************* //
