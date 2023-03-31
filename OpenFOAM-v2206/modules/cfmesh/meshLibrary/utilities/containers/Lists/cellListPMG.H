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
    Foam::Module::cellListPMG

Description
    This is a container with additional size to prevent re-allocation
    every time it is resized

SourceFiles


\*---------------------------------------------------------------------------*/

#ifndef cellListPMG_H
#define cellListPMG_H

#include "cellList.H"
#include "Istream.H"
#include "Ostream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{
class cellListPMG;
inline Ostream& operator<<(Ostream&, const cellListPMG&);
inline Istream& operator>>(Istream&, cellListPMG&);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class cellListPMG
:
    public cellList
{
    // Private data

        //- number of used elements
        label nElmts_;


    // Disallow bitwise assignment

        void operator=(const cellListPMG&);

        cellListPMG(const cellListPMG&);


    // Disallow transfer from cellList

        void transfer(cellList&);


public:

    //- null construct
    inline cellListPMG();


    //- Destructor
    ~cellListPMG() = default;


    // Member functions

        //- return the number of used elements
        inline label size() const;

        //- set the number of used elements
        inline void setSize(const label nElmts);

        //- set the size to zero
        inline void clear();

        //- add a cell at the end of the list
        inline void append(const cell&);

        //- return an element with bound checking
        inline cell& newElmt(const label);


    // Member operators

        inline void operator=(const cellList&);

        inline friend Ostream& operator<<(Ostream&, const cellListPMG&);

        inline friend Istream& operator>>(Istream&, cellListPMG&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "cellListPMGI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
