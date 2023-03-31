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

#include "boundaryPatchBase.H"
#include "Ostream.H"
#include "Istream.H"
#include "token.H"
#include "IOPtrList.H"
#include "dictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
defineTemplateTypeNameAndDebugWithName
(
    IOPtrList<Module::boundaryPatchBase>,
    "polyBoundaryMesh",
    0
);

namespace Module
{
defineTypeNameAndDebug(boundaryPatchBase, 0);
defineRunTimeSelectionTable(boundaryPatchBase, dictionary);

}
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::Module::boundaryPatchBase>
Foam::Module::boundaryPatchBase::New
(
    const word& name,
    const dictionary& dict
)
{
    word type(dict.lookup("type"));

    // Check patch type - allowed types are processor and patch
    // Other patch types are treated as ordinary patches
    if (type != "processor")
    {
        type = "patch";
    }

    auto cstrIter = dictionaryConstructorTablePtr_->cfind(type);

    if (!cstrIter.found())
    {
        FatalIOErrorInFunction(dict)
            << "Unknown boundaryPatchBase type " << type << nl << nl
            << "Valid boundaryPatchBase types:" << nl
            << "[default: " << typeName_() << "]"
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return autoPtr<boundaryPatchBase>(cstrIter()(name, dict));
}


Foam::autoPtr<Foam::Module::boundaryPatchBase>
Foam::Module::boundaryPatchBase::New
(
    Istream& is
)
{
    word name(is);
    dictionary dict(is);

    return boundaryPatchBase::New(name, dict);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::boundaryPatchBase::boundaryPatchBase
(
    const word& n,
    const word& t,
    const label nF,
    const label sF
)
:
    name_(n),
    type_(t),
    nFaces_(nF),
    startFace_(sF)
{}


Foam::Module::boundaryPatchBase::boundaryPatchBase
(
    const word& name,
    const dictionary& dict
)
:
    name_(name),
    type_(dict.lookup("type")),
    nFaces_(readLabel(dict.lookup("nFaces"))),
    startFace_(readLabel(dict.lookup("startFace")))
{}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::Ostream&
Foam::Module::operator<<
(
    Foam::Ostream& os,
    const Foam::Module::boundaryPatchBase& obj
)
{
    obj.write(os);
    os.check(FUNCTION_NAME);
    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
