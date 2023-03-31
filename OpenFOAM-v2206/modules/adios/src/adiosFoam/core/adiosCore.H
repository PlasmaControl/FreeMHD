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

Class
    Foam::adiosFoam::adiosCore

Description
    Core routines for starting/stopping ADIOS for OpenFOAM.

SourceFiles
    adiosCore.C

\*---------------------------------------------------------------------------*/

#ifndef adiosCore_H
#define adiosCore_H

#include "adios2.h"
#include "adiosFoam.H"
#include "DynamicList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace adiosFoam
{

/*---------------------------------------------------------------------------*\
                          Class adiosCore Declaration
\*---------------------------------------------------------------------------*/

class adiosCore
{
    // Private Data

        //- Global reference count for handling MPI_init and transfer buffers
        static unsigned refCount_;


protected:

    // Protected Data

        //- Buffer for reading/writing (streamed) content to/from ADIOS files
        static DynamicList<char> transfer_;


public:

        //- Debug flag
        static int debug;


    // Constructors

        //- Construct null
        adiosCore();


    //- Destructor
    virtual ~adiosCore();


    // Member Functions

        //- Buffer for reading/writing (streamed) content to/from ADIOS files.
        //  Also used when reading/writing dissimilar content.
        inline static DynamicList<char>& transferBuffer()
        {
            return transfer_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace adiosFoam
} // End namespace Foam

#endif

// ************************************************************************* //
