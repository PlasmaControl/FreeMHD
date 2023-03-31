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

Class
    Foam::Module::meshOctreeCubeCoordinatesScalar

Description
    A class containing meshOctreeCubeCoordinates and a scalar value.
    It is used for exchanging data over processors

\*---------------------------------------------------------------------------*/

#ifndef meshOctreeCubeCoordinatesScalar_H
#define meshOctreeCubeCoordinatesScalar_H

#include "scalar.H"
#include "meshOctreeCubeCoordinates.H"
#include "contiguous.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
               Class meshOctreeCubeCoordinatesScalar Declaration
\*---------------------------------------------------------------------------*/

class meshOctreeCubeCoordinatesScalar
{
    // Private data

        //- scalar
        scalar sValue_;

        //- cube coordinates
        meshOctreeCubeCoordinates coordinates_;


public:

    // Constructors

        //- Null construct
        meshOctreeCubeCoordinatesScalar()
        :
            sValue_(0.0),
            coordinates_()
        {}


        //- Construct from label and cube coordinates
        meshOctreeCubeCoordinatesScalar
        (
            const scalar s,
            const meshOctreeCubeCoordinates& cc
        )
        :
            sValue_(s),
            coordinates_(cc)
        {}


    //- Destructor
    ~meshOctreeCubeCoordinatesScalar() = default;


    // Member functions

        //- return cube label
        inline scalar scalarValue() const
        {
            return sValue_;
        }

        //- return the value
        inline const meshOctreeCubeCoordinates& coordinates() const
        {
            return coordinates_;
        }


    // Member operators

        inline bool operator<(const meshOctreeCubeCoordinatesScalar& scc) const
        {
            if (coordinates_ < scc.coordinates_)
            {
                return true;
            }

            return false;
        }

        inline void operator=(const meshOctreeCubeCoordinatesScalar& scc)
        {
            sValue_ = scc.sValue_;
            coordinates_ = scc.coordinates_;
        }

        inline bool operator==
        (
            const meshOctreeCubeCoordinatesScalar& scc
        ) const
        {
            if (coordinates_ == scc.coordinates_)
            {
                return true;
            }

            return false;
        }

        inline bool operator!=
        (
            const meshOctreeCubeCoordinatesScalar& scc
        ) const
        {
            return !this->operator==(scc);
        }


    // Friend operators

        friend Ostream& operator<<
        (
            Ostream& os,
            const meshOctreeCubeCoordinatesScalar& scc
        )
        {
            os << token::BEGIN_LIST;
            os << scc.sValue_ << token::SPACE;
            os << scc.coordinates_ << token::END_LIST;

            os.check(FUNCTION_NAME);
            return os;
        }

        friend Istream& operator>>
        (
            Istream& is,
            meshOctreeCubeCoordinatesScalar& scc
        )
        {
            // Read beginning of meshOctreCubeCoordinatesScalar
            is.readBegin("meshOctreCubeCoordinatesScalar");

            is >> scc.sValue_;
            is >> scc.coordinates_;

            // Read end of meshOctreCubeCoordinatesScalar
            is.readEnd("meshOctreCubeCoordinatesScalar");

            is.check(FUNCTION_NAME);
            return is;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module

//- Data for meshOctreCubeCoordinatesScalar are contiguous
template<>
struct is_contiguous<Module::meshOctreeCubeCoordinatesScalar>
:
    std::true_type
{};


} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
