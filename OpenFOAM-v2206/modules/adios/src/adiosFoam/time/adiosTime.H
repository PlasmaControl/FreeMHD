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
    Foam::adiosFoam::adiosTime

Description
    A partial mirroring of TimeState for time management with ADIOS files.

SourceFiles
    adiosTime.C

\*---------------------------------------------------------------------------*/

#ifndef adiosTime_H
#define adiosTime_H

#include "adiosCore.H"
#include "Enum.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class TimeState;

namespace adiosFoam
{

// Forward Declarations
class adiosReader;


/*---------------------------------------------------------------------------*\
                          Class adiosTime Declaration
\*---------------------------------------------------------------------------*/

class adiosTime
{
    // Private Data

        label  index_;
        scalar value_;
        scalar deltaT_;
        scalar deltaT0_;

public:

    // Static Data

        //- Time-attribute names
        enum attrType
        {
            INDEX = 0,
            VALUE = 1,
            DT    = 2,
            DT0   = 3
        };

        //- General time variables within ADIOS file
        static const Enum<attrType> attrNames;


    // Constructors

        //- Construct null
        adiosTime();

        //- Copy values from TimeState
        adiosTime(const TimeState& t);

        //- Construct by extracting values from adios reader
        adiosTime(adiosReader& r);


    // Member Functions

        inline const label& timeIndex() const
        {
            return index_;
        }


        inline const scalar& timeValue() const
        {
            return value_;
        }


        inline const scalar& deltaT() const
        {
            return deltaT_;
        }


        inline const scalar& deltaT0() const
        {
            return deltaT0_;
        }


        //- Has valid content?
        explicit operator bool() const;

        //- Has valid content?
        bool valid() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace adiosFoam
} // End namespace Foam

#endif

// ************************************************************************* //
