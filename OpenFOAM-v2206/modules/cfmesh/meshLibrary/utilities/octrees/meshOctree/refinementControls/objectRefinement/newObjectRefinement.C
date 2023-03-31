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

#include "objectRefinement.H"
#include "dictionary.H"
#include "error.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::autoPtr<Foam::Module::objectRefinement>
Foam::Module::objectRefinement::New
(
    const word& name,
    const dictionary& dict
)
{
    DebugInfo
        << "constructing objectRefinement"
        << endl;

    // Default type is self
    word refType(typeName_());
    dict.readIfPresent("type", refType);

    auto cstrIter = dictionaryConstructorTablePtr_->cfind(refType);

    if (!cstrIter.found())
    {
        FatalIOErrorInFunction(dict)
            << "Unknown objectRefinement type " << refType << nl << nl
            << "Valid objectRefinement types :" << nl
            << "[default: " << typeName_() << "]"
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return autoPtr<objectRefinement>(cstrIter()(name, dict));
}


// ************************************************************************* //
