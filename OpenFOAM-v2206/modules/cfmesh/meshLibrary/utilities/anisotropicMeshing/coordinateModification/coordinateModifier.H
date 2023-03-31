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
    Foam::Module::coordinateModifier

Description
    Base class for modification of point coordinates
    in surface and volume meshes.

SourceFiles
    coordinateModifier.C

\*---------------------------------------------------------------------------*/

#ifndef coordinateModifier_H
#define coordinateModifier_H

#include "word.H"
#include "point.H"
#include "coordinateModificationList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
// Forward declarations
class boundBox;

namespace Module
{

/*---------------------------------------------------------------------------*\
                     Class coordinateModifier Declaration
\*---------------------------------------------------------------------------*/

class coordinateModifier
{
    // Private data

        //- Reference to dictionary
        const dictionary& modificationDict_;

        //- list of coordinate modification objects
        coordinateModificationList modifiers_;

        //- list of backward coordinate modifiers
        coordinateModificationList backwardModifiers_;


    // Private member functions

        //- check if the combination of modifiers has a valid inverse
        void checkForValidInverse() const;


public:

    // Constructors

        //- Construct from dictionary
        coordinateModifier(const dictionary&);

    //- Destructor
    ~coordinateModifier() = default;


    // Member Functions

        //- calculate the modified coordinate of the point
        point modifiedPoint(const point&) const;

        //- calculate the displacement vector for the backward modification
        point backwardModifiedPoint(const point&) const;

        //- Print modification objects
        void printObjects() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
