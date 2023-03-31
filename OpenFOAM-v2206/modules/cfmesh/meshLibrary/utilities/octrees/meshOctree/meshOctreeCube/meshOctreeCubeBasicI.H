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

#include "meshOctreeCubeBasic.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline Foam::Module::meshOctreeCubeBasic::meshOctreeCubeBasic()
:
    meshOctreeCubeCoordinates(),
    cubeType_(UNKNOWN),
    procNo_(ALLPROCS)
{}


inline Foam::Module::meshOctreeCubeBasic::meshOctreeCubeBasic
(
    const meshOctreeCubeCoordinates& cc
)
:
    meshOctreeCubeCoordinates(cc),
    cubeType_(UNKNOWN),
    procNo_(ALLPROCS)
{}


inline Foam::Module::meshOctreeCubeBasic::meshOctreeCubeBasic
(
    const meshOctreeCubeCoordinates& cc,
    const direction cubeType,
    const short procNo
)
:
    meshOctreeCubeCoordinates(cc),
    cubeType_(cubeType),
    procNo_(procNo)
{}


// * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * * //

inline void Foam::Module::meshOctreeCubeBasic::setCubeType(const direction ct)
{
    cubeType_ = ct;
}


inline Foam::direction Foam::Module::meshOctreeCubeBasic::cubeType() const
{
    return cubeType_;
}


inline short Foam::Module::meshOctreeCubeBasic::procNo() const
{
    return procNo_;
}


inline void Foam::Module::meshOctreeCubeBasic::setProcNo(const short n)
{
    procNo_ = n;
}


inline const Foam::Module::meshOctreeCubeCoordinates&
Foam::Module::meshOctreeCubeBasic::coordinates() const
{
    return *this;
}


inline Foam::Ostream& Foam::Module::operator<<
(
    Ostream& os,
    const Foam::Module::meshOctreeCubeBasic& ocb
)
{
    os << token::BEGIN_LIST;

    os << ocb.cubeType_ << token::SPACE;
    os << ocb.procNo_ << token::SPACE;
    os << static_cast<const meshOctreeCubeCoordinates&>(ocb);

    os << token::END_LIST;

    os.check(FUNCTION_NAME);
    return os;
}


inline Foam::Istream& Foam::Module::operator>>
(
    Istream& is,
    Foam::Module::meshOctreeCubeBasic& ocb
)
{
    // Read beginning of meshOctreeCubeBasic
    is.readBegin("meshOctreeCubeBasic");

    label l;
    is >> l;
    ocb.cubeType_ = l;
    is >> l;
    ocb.procNo_ = l;
    is >> static_cast<meshOctreeCubeCoordinates&>(ocb);

    // Read end of meshOctreeCubeBasic
    is.readEnd("meshOctreeCubeBasic");

    is.check(FUNCTION_NAME);
    return is;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
