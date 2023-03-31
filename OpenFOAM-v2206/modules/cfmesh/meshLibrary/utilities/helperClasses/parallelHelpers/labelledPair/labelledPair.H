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
    Foam::Module::labelledPair

Description
    A class containing point label and meshOctreeCubeCoordinates.
    It is used for exchanging data over processors

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef labelledPair_H
#define labelledPair_H

#include "labelPair.H"
#include "contiguous.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
                        Class labelledPair Declaration
\*---------------------------------------------------------------------------*/

class labelledPair
{
    // Private Data

        //- Label
        label pLabel_;

        //- Pair data
        labelPair pair_;


public:

    // Constructors

        //- Default construct
        labelledPair()
        :
            pLabel_(-1),
            pair_()
        {}

        //- Construct from label and pair
        labelledPair
        (
            const label pl,
            const labelPair& lp
        )
        :
            pLabel_(pl),
            pair_(lp)
        {}


    // Member Functions

        //- The label for the pair
        inline label pairLabel() const
        {
            return pLabel_;
        }

        //- The pair
        inline const labelPair& pair() const
        {
            return pair_;
        }


    // Member Operators

        inline bool operator==
        (
            const labelledPair& lpp
        ) const
        {
            if (pLabel_ != lpp.pLabel_)
            {
                return false;
            }

            const labelPair& op = lpp.pair_;

            if
            (
                (pair_.first() == op.first())
             && (pair_.second() == op.second())
            )
            {
                return true;
            }
            if
            (
                (pair_.first() == op.second())
             && (pair_.second() == op.first())
            )
            {
                return true;
            }

            return false;
        }

        inline bool operator!=(const labelledPair& rhs) const
        {
            return !(*this == rhs);
        }

        inline bool operator<(const labelledPair& lpp) const
        {
            if (pLabel_ < lpp.pLabel_)
            {
                return true;
            }
            else if (pLabel_ > lpp.pLabel_)
            {
                return false;
            }

            if
            (
                (pair_.first() + pair_.second()) <
                (lpp.pair().first() + lpp.pair().second())
            )
            {
                return true;
            }

            if
            (
                Foam::min(pair_.first(), pair_.second()) <
                Foam::min(lpp.pair().first(), lpp.pair().second())
            )
            {
                return true;
            }

            return false;
        }


    // Friend Operators

        friend Ostream& operator<<
        (
            Ostream& os,
            const labelledPair& lpp
        )
        {
            os << token::BEGIN_LIST;
            os << lpp.pLabel_ << token::SPACE;
            os << lpp.pair_ << token::END_LIST;

            os.check(FUNCTION_NAME);
            return os;
        }

        friend Istream& operator>>
        (
            Istream& is,
            labelledPair& lpp
        )
        {
            // Read beginning of labelledPair
            is.readBegin("labelledPair");

            is >> lpp.pLabel_;
            is >> lpp.pair_;

            // Read end of labelledPair
            is.readEnd("labelledPair");

            is.check(FUNCTION_NAME);
            return is;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module

//- Data for labelledPair are contiguous
template<>
struct is_contiguous<Module::labelledPair>
:
    std::true_type
{};


} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
