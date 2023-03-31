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
    Foam::Module::labelledPoint

Description
    A class containing point label and its coordinates. It is used for
    exchanging data over processors

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef labelledPoint_H
#define labelledPoint_H

#include "label.H"
#include "point.H"
#include "contiguous.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
                        Class labelledPoint Declaration
\*---------------------------------------------------------------------------*/

class labelledPoint
{
    // Private Data

        //- Point label
        label pLabel_;

        //- Point coordinates
        point coords_;


public:

    // Constructors

        //- Default construct, as invalid
        labelledPoint()
        :
            pLabel_(-1),
            coords_(vector::zero)
        {}


        //- Construct from point and label
        labelledPoint(const label pl, const point& p)
        :
            pLabel_(pl),
            coords_(p)
        {}


    // Member Functions

        //- return point label
        inline label pointLabel() const
        {
            return pLabel_;
        }

        inline label& pointLabel()
        {
            return pLabel_;
        }

        //- return point coordinates
        inline const point& coordinates() const
        {
            return coords_;
        }

        inline point& coordinates()
        {
            return coords_;
        }


    // Member Operators

        inline bool operator==(const labelledPoint& rhs) const
        {
            return (pLabel_ == rhs.pLabel_);
        }

        inline bool operator!=(const labelledPoint& rhs) const
        {
            return !(*this == rhs);
        }


    // IOstream Operators

        friend Ostream& operator<<(Ostream& os, const labelledPoint& lp)
        {
            os << token::BEGIN_LIST;
            os << lp.pLabel_ << token::SPACE;
            os << lp.coords_ << token::END_LIST;

            os.check(FUNCTION_NAME);
            return os;
        }

        friend Istream& operator>>(Istream& is, labelledPoint& lp)
        {
            // Read beginning of labelledPoint
            is.readBegin("labelledPoint");

            is >> lp.pLabel_;
            is >> lp.coords_;

            // Read end of labelledPoint
            is.readEnd("labelledPoint");

            is.check(FUNCTION_NAME);
            return is;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module


//- Data for labelledPoint are contiguous
template<>
struct is_contiguous<Module::labelledPoint>
:
    std::true_type
{};


} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
