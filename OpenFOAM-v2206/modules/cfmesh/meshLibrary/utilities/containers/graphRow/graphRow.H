/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | cfMesh: A library for mesh generation
   \\    /   O peration     |
    \\  /    A nd           | www.cfmesh.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2014-2017 Creative Fields, Ltd.
    Copyright (C) 2021 OpenCFD Ltd.
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
    Foam::Module::graphRow

Description
    This class provides non-const access to a row of a graph

SourceFiles
    graphRowI.H

\*---------------------------------------------------------------------------*/

#ifndef graphRow_H
#define graphRow_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
class Ostream;

namespace Module
{
template<class graphType> class graphRow;
template<class graphType> class graphConstRow;

template<class graphType>
Ostream& operator<<(Ostream&, const graphRow<graphType>&);


/*---------------------------------------------------------------------------*\
                          Class graphRow Declaration
\*---------------------------------------------------------------------------*/

template<class graphType>
class graphRow
{
    // Private data

        //- Reference to the graph
        graphType& data_;

        //- Row number
        const label rowI_;

    // Allow copy construct from non-const to const version
    friend class graphConstRow<graphType>;


public:

    // Constructors

        //- Construct from graph and row number
        inline graphRow(graphType& g, const label i);

        //- Copy contructor
        inline graphRow(const graphRow<graphType>& r);


    //- Destructor
    ~graphRow() = default;


    // Member Functions

        //- Return the number of elements in the row
        inline label size() const;

        //- Reset the number of elements in the row
        inline void setSize(const label s);

        //- Clear the row
        inline void clear();


    // Member Operators

        //- Append an element to the given row
        inline void append(const label);

        //- Append an element to the given row if it does not exist there
        inline void appendUniq(const label);

        //- check if the element is in the given row (takes linear time)
        inline bool found(const label e) const;
        inline label find(const label e) const;

        //- set and get operators
        inline label operator[](const label) const;
        inline label& operator[](const label);

        //- Copy contents from another row
        inline void operator=(const graphRow<graphType>& rhs);

        //- Copy contents from a list
        template<class ListType>
        inline void operator=(const ListType& rhs);


    // IOstream operators

        //- Write graphRow contents to Ostream.
        friend Ostream& operator<< <graphType>
        (
            Ostream& os,
            const graphRow<graphType>& r
        );


    // Housekeeping

        //- Same as appendUniq()
        void appendIfNotIn(const label val)
        {
            this->appendUniq(val);
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "graphRowI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
