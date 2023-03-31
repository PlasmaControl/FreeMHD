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
    Foam::Module::refLabelledPoint

Description
    A class containing the label of the object it is associated to and
    a labelledPoint

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef refLabelledPoint_H
#define refLabelledPoint_H

#include "labelledPoint.H"
#include "contiguous.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
                      Class refLabelledPoint Declaration
\*---------------------------------------------------------------------------*/

class refLabelledPoint
{
    // Private data

        //- label of the object it is associated to
        label objectLabel_;

        //- point to be transferred
        labelledPoint p_;


public:

    // Constructors

        //- Null construct
        refLabelledPoint()
        :
            objectLabel_(-1),
            p_()
        {}

        //- Construct from label and labelledPoint
        refLabelledPoint(const label pl, const labelledPoint& p)
        :
            objectLabel_(pl),
            p_(p)
        {}


    //- Destructor
    ~refLabelledPoint() = default;


    // Member functions

        //- return label of the object it is associated to
        inline label objectLabel() const
        {
            return objectLabel_;
        }

        //- return labelledPoint
        inline const labelledPoint& lPoint() const
        {
            return p_;
        }


    // Member operators

        inline void operator=(const refLabelledPoint& lp)
        {
            objectLabel_ = lp.objectLabel_;
            p_ = lp.p_;
        }

        inline bool operator==(const refLabelledPoint& lp) const
        {
            return (objectLabel_ == lp.objectLabel_);
        }

        inline bool operator!=(const refLabelledPoint& lp) const
        {
            return !this->operator==(lp);
        }


    // Friend operators

        friend Ostream& operator<<(Ostream& os, const refLabelledPoint& lp)
        {
            os << token::BEGIN_LIST;
            os << lp.objectLabel_ << token::SPACE;
            os << lp.p_ << token::END_LIST;

            os.check(FUNCTION_NAME);
            return os;
        }

        friend Istream& operator>>(Istream& is, refLabelledPoint& lp)
        {
            // Read beginning of refLabelledPoint
            is.readBegin("refLabelledPoint");

            is >> lp.objectLabel_;
            is >> lp.p_;

            // Read end of refLabelledPoint
            is.readEnd("refLabelledPoint");

            is.check(FUNCTION_NAME);
            return is;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module

//- Data for refLabelledPoint are contiguous
template<>
struct is_contiguous<Module::refLabelledPoint>
:
    std::true_type
{};


} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
