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
    Foam::adiosFoam::fieldInfo

Description
    A lightweight class for handling basic field information

SourceFiles
    adiosFoamFieldInfo.H

\*---------------------------------------------------------------------------*/

#ifndef adiosFoamFieldInfo_H
#define adiosFoamFieldInfo_H

#include "adiosFoam.H"
#include "GeometricField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace adiosFoam
{

/*---------------------------------------------------------------------------*\
                          Class fieldInfo Declaration
\*---------------------------------------------------------------------------*/

class fieldInfo
:
    public word
{
    // Private Data

        //- The region name
        word region_;

        //- The field type (class)
        word type_;


public:

    // Constructors

        //- Construct null
        fieldInfo() = default;

        //- Construct from components
        fieldInfo
        (
            const word& regionName,
            const word& fieldName,
            const word& clsName
        )
        :
            word(fieldName),
            region_(regionName),
            type_(clsName)
        {}

        //- Construct from variable name and attribute
        fieldInfo
        (
            const fileName& varName,
            const word& clsName
        )
        :
            word(varName.name()),
            region_(adiosFoam::regionOf(varName)),
            type_(clsName)
        {}


        //- Construct from GeometricField
        template<class Type, template<class> class PatchField, class GeoMesh>
        explicit fieldInfo
        (
            const GeometricField<Type, PatchField, GeoMesh>& fld
        )
        :
            word(fld.name()),
            region_(fld.db().name()),
            type_(fld.type())
        {}


    // Member Functions

        //- The field name
        inline const word& name() const
        {
            return *this;
        }

        //- Write access to the field name
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

        //- The class/type (eg, volScalarField)
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
            return adiosFoam::fieldPath(region_);
        }

        //- The full variable name
        inline fileName fullName() const
        {
            return adiosFoam::fieldPath(region_, *this);
        }

        //- Output some information
        Ostream& info(Ostream& os) const
        {
            os.beginBlock();

            os.writeEntry("name", fullName());
            if (!type().empty())
            {
                os.writeEntry("class", type());
            }

            os.endBlock();

            return os;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace adiosFoam


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Ostream Operator

//- Output some information
inline Ostream& operator<<(Ostream& os, const adiosFoam::fieldInfo& obj)
{
    os  << obj.fullName() << token::SPACE << obj.type();

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
