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

#include "processorBoundaryPatch.H"
#include "addToRunTimeSelectionTable.H"
#include "dictionary.H"
#include "Ostream.H"
#include "Istream.H"
#include "token.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

defineTypeNameAndDebug(processorBoundaryPatch, 0);
addToRunTimeSelectionTable
(
    boundaryPatchBase,
    processorBoundaryPatch,
    dictionary
);

} // End namespace Module
} // End namespace Foam


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::Module::processorBoundaryPatch::processorBoundaryPatch
(
    const word& name,
    const word& type,
    const label nFaces,
    const label startFace,
    const label myProcNo,
    const label neighbProcNo
)
:
    boundaryPatchBase(name, type, nFaces, startFace),
    myProcNo_(myProcNo),
    neighbProcNo_(neighbProcNo)
{}


Foam::Module::processorBoundaryPatch::processorBoundaryPatch
(
    const word& name,
    const dictionary& dict
)
:
    boundaryPatchBase(name, dict),
    myProcNo_(readLabel(dict.lookup("myProcNo"))),
    neighbProcNo_(readLabel(dict.lookup("neighbProcNo")))
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::dictionary Foam::Module::processorBoundaryPatch::dict() const
{
    dictionary dict;

    dict.add("type", type_);

    dict.add("nFaces", nFaces_);
    dict.add("startFace", startFace_);
    dict.add("myProcNo", myProcNo_);
    dict.add("neighbProcNo", neighbProcNo_);

    return dict;
}


void Foam::Module::processorBoundaryPatch::write(Ostream& os) const
{
    this->operator<<(os);
}


void Foam::Module::processorBoundaryPatch::writeDict(Ostream& /*os*/) const
{}


Foam::Ostream& Foam::Module::processorBoundaryPatch::operator<<
(
    Ostream& os
) const
{
    os  << patchName() << nl << token::BEGIN_BLOCK << nl
        << "    type         " << patchType() << token::END_STATEMENT << nl
        << "    nFaces       " << patchSize() << token::END_STATEMENT << nl
        << "    startFace    " << patchStart() << token::END_STATEMENT << nl
        << "    myProcNo     " << myProcNo_ << token::END_STATEMENT << nl
        << "    neighbProcNo " << neighbProcNo_
        << token::END_STATEMENT << nl
        << token::END_BLOCK << endl;

    return os;
}


Foam::Istream& Foam::Module::processorBoundaryPatch::operator>>(Istream& is)
{
    token t;
    is >> name_ >> t;
    is >> t >> type_ >> t;
    is >> t >> nFaces_ >> t;
    is >> t >> startFace_ >> t;
    is >> t >> myProcNo_ >> t;
    is >> t >> neighbProcNo_ >> t;
    is >> t;

    return is;
}


bool Foam::Module::processorBoundaryPatch::operator!=
(
    const processorBoundaryPatch& wp
) const
{
    if (name_ != wp.name_)
    {
        return true;
    }
    else if (type_ != wp.name_)
    {
        return true;
    }
    else if ((nFaces_ != wp.nFaces_) || (startFace_ != wp.startFace_))
    {
        return true;
    }
    else if
    (
        (myProcNo_ != wp.myProcNo_) || (neighbProcNo_ != wp.neighbProcNo_)
    )
    {
        return true;
    }

    return false;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
