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
    Foam::Module::coordinateModification

Description
    Base class for modifiers of point coordinates.

SourceFiles
    coordinateModification.C

\*---------------------------------------------------------------------------*/

#ifndef coordinateModification_H
#define coordinateModification_H

#include "word.H"
#include "point.H"
#include "dictionary.H"
#include "PtrList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
// Forward declarations
class boundBox;
class plane;

namespace Module
{
class coordinateModification;
Ostream& operator<<(Ostream&, const coordinateModification&);

/*---------------------------------------------------------------------------*\
                   Class coordinateModification Declaration
\*---------------------------------------------------------------------------*/

class coordinateModification
{
    // Private data

        //- Modification name
        word name_;


public:

    //- Runtime type information
    TypeName("coordinateModification");


    // Constructors

        //- Null construct
        coordinateModification();

        //- Construct from dictionary
        coordinateModification(const word& name, const dictionary&);


    //- Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        coordinateModification,
        dictionary,
        (
            const word& name,
            const dictionary& dict
        ),
        (name, dict)
    );

    //- Select constructed from dictionary
    static autoPtr<coordinateModification> New
    (
        const word& name,
        const dictionary& dict
    );

    //- Destructor
    virtual ~coordinateModification() = default;


    // Member Functions

        //- return the origin point of the modification object
        virtual point origin() const = 0;

        //- translate the object for backward transformation
        virtual void translateAndModifyObject(const vector&) = 0;

        //- calculate the displacement vector for this modification
        virtual vector displacement(const point&) const = 0;

        //- calculate the displacement vector for the backward modification
        virtual vector backwardDisplacement(const point&) const = 0;

        //- can this modification object be combined with other ones
        virtual bool combiningPossible() const = 0;

        //- return that "bounding planes" of the scaling region for
        //- the given object
        virtual void boundingPlanes(PtrList<plane>&) const = 0;


        // Access

            //- Return name
            const word& name() const
            {
                return name_;
            }

            //- set the name
            void setName(const word& name)
            {
                name_ = name;
            }

            //- Return as dictionary of entries
            virtual dictionary dict(bool ignoreType = false) const = 0;


        // Write

            //- Write
            virtual void write(Ostream&) const = 0;

            //- Write dictionary
            virtual void writeDict(Ostream&, bool subDict = true) const = 0;


    // Member Operators

        //- assign from dictionary
        virtual void operator=(const dictionary&) = 0;


    // IOstream Operators

        friend Ostream& operator<<(Ostream&, const coordinateModification&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
