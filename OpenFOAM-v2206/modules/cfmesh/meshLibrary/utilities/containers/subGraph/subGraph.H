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
    Foam::Module::subGraph

Description
    This class provides access to a row of a graph

SourceFiles
    subGraphI.H

\*---------------------------------------------------------------------------*/

#ifndef subGraph_H
#define subGraph_H

#include "bool.H"
#include "Ostream.H"
#include "error.H"
#include "graphRow.H"
#include "graphConstRow.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

template<class graphType>
class subGraph;

template<class graphType>
Ostream& operator<<(Ostream&, const subGraph<graphType>&);

/*---------------------------------------------------------------------------*\
                          Class subGraph Declaration
\*---------------------------------------------------------------------------*/

template<class graphType>
class subGraph
{
    // Private Data

        //- reference to the graph
        graphType& data_;

        //- starts at row
        const label start_;

        //- number of rows in the subGraph
        const label size_;


    // Private Member Functions

        //- check index
        inline void checkIndex(const label i) const;


public:

    // Constructors

        //- Construct from graph and start row and size
        inline subGraph(graphType&, const label, const label);

        //- Copy constructor
        inline subGraph(const subGraph<graphType>&);


    //- Destructor
    ~subGraph() = default;


    // Member Functions

        //- Returns the number of rows
        inline label size() const;

        //- Returns the size of a given row
        inline label sizeOfRow(const label rowI) const;


    // Member Operators

        //- Append an element to the given row
        inline void append(const label rowI, const label);

        //- Append an element to the given row if it does not exist there
        inline void appendUniq(const label rowI, const label);

        //- check if the element is in the given row (takes linear time)
        inline bool found(const label rowI, const label e) const;
        inline label find(const label rowI, const label e) const;

        //- set and get operators
        inline label operator()(const label i, const label j) const;
        inline label& operator()(const label i, const label j);
        inline const graphConstRow<graphType> operator[](const label) const;
        inline graphRow<graphType> operator[](const label);


    // IOstream operators

        // Write subGraph to Ostream.
        friend Ostream& operator<< <graphType>
        (
            Ostream&,
            const subGraph<graphType>&
        );


    // Housekeeping

        //- Same as appendUniq()
        void appendIfNotIn(const label rowI, const label val)
        {
            this->appendUniq(rowI, val);
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "subGraphI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
