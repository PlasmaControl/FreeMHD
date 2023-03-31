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
    Foam::Module::refLabelledPointScalar

Description
    A class containing a label and labelledPointScalar. It is used for
    exchanging data over processors

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef refLabelledPointScalar_H
#define refLabelledPointScalar_H

#include "labelledPointScalar.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
                   Class refLabelledPointScalar Declaration
\*---------------------------------------------------------------------------*/

class refLabelledPointScalar
{
    // Private data

        //- point label
        label pLabel_;

        //- labelledPointScalar
        labelledPointScalar lps_;


public:

    // Constructors

        //- Null construct
        refLabelledPointScalar()
        :
            pLabel_(-1),
            lps_()
        {}

        //- Construct from label and labelledPointScalar
        refLabelledPointScalar(const label pl, const labelledPointScalar& lps)
        :
            pLabel_(pl),
            lps_(lps)
        {}


    //- Destructor
    ~refLabelledPointScalar() = default;


    // Member functions

        //- return object label
        inline label objectLabel() const
        {
            return pLabel_;
        }

        //- return labelledPointScalar
        inline const labelledPointScalar& lps() const
        {
            return lps_;
        }


    // Member operators

        inline void operator=(const refLabelledPointScalar& lps)
        {
            pLabel_ = lps.pLabel_;
            lps_ = lps.lps_;
        }

        inline bool operator==(const refLabelledPointScalar& lps) const
        {
            if (pLabel_ == lps.pLabel_)
            {
                return true;
            }

            return false;
        }

        inline bool operator!=(const refLabelledPointScalar& lps) const
        {
            return !this->operator==(lps);
        }


    // Friend operators

        friend Ostream& operator<<
        (
            Ostream& os,
            const refLabelledPointScalar& lps
        )
        {
            os << token::BEGIN_LIST;
            os << lps.pLabel_ << token::SPACE;
            os << lps.lps_ << token::END_LIST;

            os.check(FUNCTION_NAME);
            return os;
        }

        friend Istream& operator>>(Istream& is, refLabelledPointScalar& lps)
        {
            // Read beginning of refLabelledPointScalar
            is.readBegin("refLabelledPointScalar");

            is >> lps.pLabel_;
            is >> lps.lps_;

            // Read end of refLabelledPointScalar
            is.readEnd("refLabelledPointScalar");

            is.check(FUNCTION_NAME);
            return is;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module

//- Data for refLabelledPointScalar are contiguous
template<>
struct is_contiguous<Module::refLabelledPointScalar>
:
    std::true_type
{};


} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
