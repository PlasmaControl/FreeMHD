/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2019 OpenCFD Ltd.
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
    Foam::adiosFoam::cloudInfo

Description
    A lightweight class for handling basic cloud information

SourceFiles
    adiosFoamCloudInfo.H

\*---------------------------------------------------------------------------*/

#ifndef adiosFoamCloudInfo_H
#define adiosFoamCloudInfo_H

#include "adiosFoam.H"
#include "cloud.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace adiosFoam
{

/*---------------------------------------------------------------------------*\
                          Class cloudInfo Declaration
\*---------------------------------------------------------------------------*/

class cloudInfo
:
    public word
{
    // Private Data

        //- The region name
        word region_;

        //- The cloud type (class)
        word type_;


public:

    // Constructors

        //- Construct null
        cloudInfo() = default;

        //- Construct from components
        cloudInfo
        (
            const word& regionName,
            const word& cloudName,
            const word& clsName
        )
        :
            word(cloudName),
            region_(regionName),
            type_(clsName)
        {}


        //- Construct from variable name and attribute
        cloudInfo
        (
            const fileName& varName,
            const word& clsName
        )
        :
            word(varName.name()),
            region_(adiosFoam::regionOf(varName)),
            type_(clsName)
        {}


        //- Construct from cloud
        explicit cloudInfo(const cloud& c)
        :
            cloudInfo(c.db().name(), c.name(), c.type())
        {}


    //- Destructor
    virtual ~cloudInfo() = default;


    // Member Functions

        //- The cloud name
        inline const word& name() const
        {
            return *this;
        }

        //- Write access to the cloud name
        inline word& name()
        {
            return *this;
        }

        //- The region name
        inline const word& regionName() const
        {
            return region_;
        }

        //- Write access to the region name
        inline word& regionName()
        {
            return region_;
        }

        //- Type of the cloud
        inline const word& type() const
        {
            return type_;
        }

        //- Write access to the type
        inline word& type()
        {
            return type_;
        }

        //- The variable path
        inline fileName path() const
        {
            return adiosFoam::cloudPath(region_);
        }

        //- The full variable name
        inline fileName fullName() const
        {
            return adiosFoam::cloudPath(region_, *this);
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace adiosFoam


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Ostream Operator

//- Output some information
inline Ostream& operator<<(Ostream& os, const adiosFoam::cloudInfo& obj)
{
    os  << obj.fullName() << token::SPACE << obj.type();

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
