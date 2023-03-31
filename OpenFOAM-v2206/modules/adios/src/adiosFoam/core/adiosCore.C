/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2019 OpenCFD Ltd.
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

#include "adiosCore.H"
#include "Pstream.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

int Foam::adiosFoam::adiosCore::debug(0);

unsigned Foam::adiosFoam::adiosCore::refCount_ = 0;

Foam::DynamicList<char> Foam::adiosFoam::adiosCore::transfer_;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::adiosFoam::adiosCore::adiosCore()
{
    if (refCount_ == 0)
    {
        // First one in

        transfer_.reserve(4096);

        // Initialize MPI as required - needed for ADIOS
        UPstream::initNull();
    }

    ++refCount_;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::adiosFoam::adiosCore::~adiosCore()
{
    if (refCount_ == 1)
    {
        // Last one out

        // Cleanup transfer buffer
        transfer_.clearStorage();

        // OpenFOAM will cleanup and finalize MPI
    }

    --refCount_;
}


// ************************************************************************* //
