/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2018 OpenCFD Ltd.
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

\*---------------------------------------------------------------------------*/

#include "catalystInput.H"
#include "dictionary.H"
#include "Time.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace catalyst
{
    defineTypeNameAndDebug(catalystInput, 0);
    defineRunTimeSelectionTable(catalystInput, dictionary);
}
} // End namespace Foam


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::catalyst::catalystInput::catalystInput(const word& channel)
:
    name_(channel)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::autoPtr<Foam::catalyst::catalystInput>
Foam::catalyst::catalystInput::New
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
{
    const word modelType(dict.lookupOrDefault<word>("type", "default"));

    auto cstrIter = dictionaryConstructorTablePtr_->cfind(modelType);

    if (!cstrIter.found())
    {
        FatalErrorInFunction
            << "Unknown catalystInput " << modelType << nl << nl
            << "Valid inputs:" << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<catalystInput>(cstrIter()(name, runTime, dict));
}


bool Foam::catalyst::catalystInput::read(const dictionary&)
{
    return true;
}


void Foam::catalyst::catalystInput::update(polyMesh::readUpdateState state)
{}


Foam::Ostream& Foam::catalyst::catalystInput::print(Ostream& os) const
{
    return os;
}


// ************************************************************************* //
