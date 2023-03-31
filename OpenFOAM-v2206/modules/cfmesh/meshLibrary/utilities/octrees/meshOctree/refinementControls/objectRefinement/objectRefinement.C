/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | cfMesh: A library for mesh generation
   \\    /   O peration     |
    \\  /    A nd           | www.cfmesh.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2014-2017 Creative Fields, Ltd.
-------------------------------------------------------------------------------
Author
     Franjo Juretic (franjo.juretic@c-fields.com)

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

#include "IOstream.H"
#include "objectRefinement.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{
    defineTypeNameAndDebug(objectRefinement, 0);
    defineRunTimeSelectionTable(objectRefinement, dictionary);
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::objectRefinement::objectRefinement()
:
    name_(),
    cellSize_(-1.0),
    additionalRefLevel_(0),
    refThickness_(0.0)
{}


Foam::Module::objectRefinement::objectRefinement
(
    const word& name,
    const dictionary& dict
)
:
    name_(name),
    cellSize_(-1.0),
    additionalRefLevel_(0),
    refThickness_(0.0)
{
    if (dict.readIfPresent("cellSize", cellSize_))
    {
        if (cellSize_ < 0.0)
        {
            FatalErrorInFunction
                << "Specified cell size for object " << name_
                << " is negative" << exit(FatalError);
        }
    }
    else if
    (
        dict.readIfPresent("additionalRefinementLevels", additionalRefLevel_)
    )
    {
        if (additionalRefLevel_ < 0)
        {
            FatalErrorInFunction
                << "Specified additionalRefinementLevel for object " << name_
                << " is negative" << exit(FatalError);
        }
    }

    dict.readIfPresent("refinementThickness", refThickness_);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::Ostream& Foam::Module::operator<<
(
    Ostream& os,
    const Foam::Module::objectRefinement& obr
)
{
    os << obr.name() << nl;
    obr.writeDict(os, true);
    return os;
}


void Foam::Module::objectRefinement::calculateAdditionalRefLevels
(
    const scalar globalCellSize
)
{
    if (cellSize_ < 0.0 || additionalRefLevel_ != 0)
        return;

    scalar s = globalCellSize;

    label nMarked;
    do
    {
        nMarked = 0;

        if (cellSize_ <= s*(1.+SMALL))
        {
            ++nMarked;
            ++additionalRefLevel_;
        }

        s /= 2.0;

    } while (nMarked != 0);
}


// ************************************************************************* //
