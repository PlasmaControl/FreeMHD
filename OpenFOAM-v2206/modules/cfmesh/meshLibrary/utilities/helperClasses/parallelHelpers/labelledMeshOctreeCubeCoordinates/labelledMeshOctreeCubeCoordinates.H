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
    Foam::Module::labelledMeshOctreeCubeCoordinates

Description
    A class containing point label and meshOctreeCubeCoordinates.
    It is used for exchanging data over processors

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef labelledMeshOctreeCubeCoordinates_H
#define labelledMeshOctreeCubeCoordinates_H

#include "label.H"
#include "meshOctreeCubeCoordinates.H"
#include "contiguous.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
              Class labelledMeshOctreeCubeCoordinates Declaration
\*---------------------------------------------------------------------------*/

class labelledMeshOctreeCubeCoordinates
{
    // Private data

        //- label
        label cLabel_;

        //- cube coordinates
        meshOctreeCubeCoordinates coordinates_;


public:

    // Constructors

        //- Null construct
        labelledMeshOctreeCubeCoordinates()
        :
            cLabel_(-1),
            coordinates_()
        {}

        //- Construct from label and cube coordinates
        labelledMeshOctreeCubeCoordinates
        (
            const label cl,
            const meshOctreeCubeCoordinates& cc
        )
        :
            cLabel_(cl),
            coordinates_(cc)
        {}


    //- Destructor
    ~labelledMeshOctreeCubeCoordinates() = default;


    // Member functions

        //- return cube label
        inline label cubeLabel() const
        {
            return cLabel_;
        }

        //- return the value
        inline const meshOctreeCubeCoordinates& coordinates() const
        {
            return coordinates_;
        }


    // Member operators

        inline void operator=(const labelledMeshOctreeCubeCoordinates& lcc)
        {
            cLabel_ = lcc.cLabel_;
            coordinates_ = lcc.coordinates_;
        }

        inline bool operator==
        (
            const labelledMeshOctreeCubeCoordinates& lcc
        ) const
        {
            return (cLabel_ == lcc.cLabel_);
        }
        }

        inline bool operator!=
        (
            const labelledMeshOctreeCubeCoordinates& lcc
        ) const
        {
            return !this->operator==(lcc);
        }


    // Friend operators

        friend Ostream& operator<<
        (
            Ostream& os,
            const labelledMeshOctreeCubeCoordinates& lcc
        )
        {
            os << token::BEGIN_LIST;
            os << lcc.cLabel_ << token::SPACE;
            os << lcc.coordinates_ << token::END_LIST;

            os.check(FUNCTION_NAME);
            return os;
        }

        friend Istream& operator>>
        (
            Istream& is,
            labelledMeshOctreeCubeCoordinates& lcc
        )
        {
            // Read beginning of labelledMeshOctreeCubeCoordinates
            is.readBegin("labelledMeshOctreeCubeCoordinates");

            is >> lcc.cLabel_;
            is >> lcc.coordinates_;

            // Read end of labelledMeshOctreeCubeCoordinates
            is.readEnd("labelledMeshOctreeCubeCoordinates");

            is.check(FUNCTION_NAME);
            return is;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

//- Data for labelledMeshOctreeCubeCoordinates are contiguous
template<>
struct is_contiguous<Module::labelledMeshOctreeCubeCoordinates>
:
    std::true_type
{};


} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
