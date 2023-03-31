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
    Foam::Module::labelledScalar

Description
    A class containing point label and its coordinates. It is used for
    exchanging data over processors

\*---------------------------------------------------------------------------*/

#ifndef labelledScalar_H
#define labelledScalar_H

#include "label.H"
#include "scalar.H"
#include "contiguous.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
                       Class labelledScalar Declaration
\*---------------------------------------------------------------------------*/

class labelledScalar
{
    // Private data

        //- label
        label sLabel_;

        //- value
        scalar value_;


public:

    // Constructors

        //- Null construct
        labelledScalar()
        :
            sLabel_(-1),
            value_(0.0)
        {}


        //- Construct from label and value
        labelledScalar(const label sl, const scalar s)
        :
            sLabel_(sl),
            value_(s)
        {}


    //- Destructor
    ~labelledScalar() = default;


    // Member functions

        //- return scalar label
        inline label scalarLabel() const
        {
            return sLabel_;
        }

        //- return the value
        inline scalar value() const
        {
            return value_;
        }


    // Member operators

        inline void operator=(const labelledScalar& ls)
        {
            sLabel_ = ls.sLabel_;
            value_ = ls.value_;
        }

        inline bool operator==(const labelledScalar& ls) const
        {
            if (sLabel_ == ls.sLabel_)
            {
                return true;
            }

            return false;
        }

        inline bool operator<(const labelledScalar& ls) const
        {
            if (value_ < ls.value_)
            {
                return true;
            }

            return false;
        }

        inline bool operator<=(const labelledScalar& ls) const
        {
            if (value_ <= ls.value_)
            {
                return true;
            }

            return false;
        }

        inline bool operator>(const labelledScalar& ls) const
        {
            if (value_ > ls.value_)
            {
                return true;
            }

            return false;
        }

        inline bool operator>=(const labelledScalar& ls) const
        {
            if (value_ >= ls.value_)
            {
                return true;
            }

            return false;
        }

        inline bool operator!=(const labelledScalar& ls) const
        {
            return !this->operator==(ls);
        }


    // Friend operators

        friend Ostream& operator<<(Ostream& os, const labelledScalar& ls)
        {
            os << token::BEGIN_LIST;
            os << ls.sLabel_ << token::SPACE;
            os << ls.value_ << token::END_LIST;

            os.check(FUNCTION_NAME);
            return os;
        }

        friend Istream& operator>>(Istream& is, labelledScalar& ls)
        {
            // Read beginning of labelledScalar
            is.readBegin("labelledScalar");

            is >> ls.sLabel_;
            is >> ls.value_;

            // Read end of labelledScalar
            is.readEnd("labelledScalar");

            is.check(FUNCTION_NAME);
            return is;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module

//- Data for labelledScalar are contiguous
template<>
struct is_contiguous<Module::labelledScalar>
:
    std::true_type
{};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
