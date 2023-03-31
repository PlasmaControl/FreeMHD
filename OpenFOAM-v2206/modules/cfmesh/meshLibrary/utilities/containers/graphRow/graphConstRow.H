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
    Foam::Module::graphConstRow

Description
    This class provides const access to a row of a graph

SourceFiles
    graphConstRowI.H

\*---------------------------------------------------------------------------*/

#ifndef graphConstRow_H
#define graphConstRow_H

#include "Ostream.H"
#include "error.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
class Ostream;

namespace Module
{
template<class graphType> class graphRow;
template<class graphType> class graphConstRow;

template<class graphType>
Ostream& operator<<(Ostream&, const graphConstRow<graphType>&);


/*---------------------------------------------------------------------------*\
                        Class graphConstRow Declaration
\*---------------------------------------------------------------------------*/

template<class graphType>
class graphConstRow
{
    // Private data

        //- Const reference to the graph
        const graphType& data_;

        //- Row number
        const label rowI_;


    // Private Member Functions

        //- Avoid possible confusion
        void operator=(const graphConstRow<graphType>&) = delete;

public:

    // Constructors

        //- Construct from graph and row number
        inline graphConstRow(const graphType& g, const label i);

        //- Copy contructor
        inline graphConstRow(const graphConstRow<graphType>& r);

        //- Copy contructor from non-const version
        inline graphConstRow(const graphRow<graphType>& r);


    //- Destructor
    ~graphConstRow() = default;


    // Member Functions

        //- Return the number of elements in the row
        inline label size() const;


    // Member Operators

        //- check if the element is in the given row (takes linear time)
        inline bool found(const label e) const;
        inline label find(const label e) const;

        //- Get operator
        inline label operator[](const label) const;

    // IOstream operators

        //- Write graphConstRow contents to Ostream.
        friend Ostream& operator<< <graphType>
        (
            Ostream& os,
            const graphConstRow<graphType>& r
        );

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "graphConstRowI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
