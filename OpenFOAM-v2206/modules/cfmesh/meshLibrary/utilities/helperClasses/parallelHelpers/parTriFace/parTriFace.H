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
    Foam::Module::parTriFace

Description
    Hold point labels and their coordinates. It is used for exchanging data
    over processors

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef parTriFace_H
#define parTriFace_H

#include "label.H"
#include "point.H"
#include "triangle.H"
#include "contiguous.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
                         Class parTriFace Declaration
\*---------------------------------------------------------------------------*/

class parTriFace
{
    // Private data

        label globalLabels_[3];

        triangle<point, point> triPoints_;


public:

    // Constructors

        inline parTriFace()
        :
            triPoints_(vector::zero, vector::zero, vector::zero)
        {}


        explicit inline parTriFace
        (
            const label globalLabel0,
            const label globalLabel1,
            const label globalLabel2,
            const triangle<point, point>& pts
        )
        :
            triPoints_(pts)
        {
            globalLabels_[0] = globalLabel0;
            globalLabels_[1] = globalLabel1;
            globalLabels_[2] = globalLabel2;
        }


    //- Destructor
    ~parTriFace() = default;


    // Member functions

        inline label globalLabelOfPoint(const label i) const
        {
            return globalLabels_[i];
        }

        inline const triangle<point, point>& trianglePoints() const
        {
            return triPoints_;
        }


    // Member operators

        inline bool operator != (const parTriFace& /*ptf*/) const
        {
            NotImplemented;
            return false;
        }


    // Friend operators

        inline friend Ostream& operator<<(Ostream& os, const parTriFace& ptf)
        {
            os << token::BEGIN_LIST;
            os << ptf.globalLabels_[0] << token::SPACE;
            os << ptf.globalLabels_[1] << token::SPACE;
            os << ptf.globalLabels_[2] << token::SPACE;
            os << ptf.triPoints_ << token::END_LIST;

            os.check(FUNCTION_NAME);
            return os;
        }

        inline friend Istream& operator>>(Istream& is, parTriFace& ptf)
        {
            // Read beginning of parTriFace
            is.readBegin("parTriFace");

            is >> ptf.globalLabels_[0];
            is >> ptf.globalLabels_[1];
            is >> ptf.globalLabels_[2];
            is >> ptf.triPoints_;

            // Read end of parHelper
            is.readEnd("parTriFace");

            is.check(FUNCTION_NAME);
            return is;
        }
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module

//- Data for parTriFace are contiguous
template<>
struct is_contiguous<Module::parTriFace>
:
    std::true_type
{};


} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
