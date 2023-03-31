/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | cfMesh: A library for mesh generation
   \\    /   O peration     |
    \\  /    A nd           | www.cfmesh.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2014-2017 Creative Fields, Ltd.
    Copyright (C) 2020 OpenCFD Ltd.
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
    Foam::Module::labelledPointScalar

Description
    A class containing a label, coordinates and scalar. It is used for
    exchanging data over processors

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef labelledPointScalar_H
#define labelledPointScalar_H

#include "label.H"
#include "point.H"
#include "contiguous.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
                     Class labelledPointScalar Declaration
\*---------------------------------------------------------------------------*/

class labelledPointScalar
{
    // Private Data

        //- Point label
        label pLabel_;

        //- Point coordinates
        point coords_;

        //- Scalar data
        scalar weight_;


public:

    // Constructors

        //- Default construct, as invalid
        labelledPointScalar()
        :
            pLabel_(-1),
            coords_(vector::zero),
            weight_(0.0)
        {}

        //- Construct from point and label
        labelledPointScalar(const label pl, const point& p, const scalar s)
        :
            pLabel_(pl),
            coords_(p),
            weight_(s)
        {}


    // Member Functions

        //- Return point label
        inline label pointLabel() const
        {
            return pLabel_;
        }

        inline label& pointLabel()
        {
            return pLabel_;
        }

        //- Return point coordinates
        inline const point& coordinates() const
        {
            return coords_;
        }

        inline point& coordinates()
        {
            return coords_;
        }

        //- Return scalar value
        inline scalar scalarValue() const
        {
            return weight_;
        }

        inline scalar& scalarValue()
        {
            return weight_;
        }


    // Member Operators

        inline bool operator==(const labelledPointScalar& rhs) const
        {
            return (pLabel_ == rhs.pLabel_);
        }

        inline bool operator!=(const labelledPointScalar& rhs) const
        {
            return !(*this == rhs);
        }


    // IOstream Operators

        friend Ostream& operator<<(Ostream& os, const labelledPointScalar& lps)
        {
            os << token::BEGIN_LIST;
            os << lps.pLabel_ << token::SPACE;
            os << lps.coords_ << token::SPACE;
            os << lps.weight_ << token::END_LIST;

            os.check(FUNCTION_NAME);
            return os;
        }

        friend Istream& operator>>(Istream& is, labelledPointScalar& lps)
        {
            // Read beginning of labelledPointScalar
            is.readBegin("labelledPointScalar");

            is >> lps.pLabel_;
            is >> lps.coords_;
            is >> lps.weight_;

            // Read end of labelledPointScalar
            is.readEnd("labelledPointScalar");

            is.check(FUNCTION_NAME);
            return is;
        }
};



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module

//- Data for labelledPointScalar are contiguous
template<>
struct is_contiguous<Module::labelledPointScalar>
:
    std::true_type
{};


} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
