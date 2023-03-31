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
    Foam::Module::FRWGraph

Description
    This class is an implementation of a graph with the fixed row width.
    The implementation is memory efficient.

SourceFiles
    FRWGraphI.H
    FRWGraph.C

\*---------------------------------------------------------------------------*/

#ifndef FRWGraph_H
#define FRWGraph_H

#include "label.H"
#include "LongList.H"
#include "FixedList.H"
#include "bool.H"
#include "error.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

// Forward declaration of template friend functions

template<class T, label width>
class FRWGraph;

template<class T, label width>
Ostream& operator<<
(
    Ostream&,
    const FRWGraph<T, width>&
);
template<class T, label width>
Istream& operator>>
(
    Istream&,
    FRWGraph<T, width>&
);

/*---------------------------------------------------------------------------*\
                          Class FRWGraph Declaration
\*---------------------------------------------------------------------------*/

template<class T, label width>
class FRWGraph
{
    // Private Data

        //- list containing the data
        LongList<T> data_;

        //- number of rows
        label nRows_;


    // Private Member Functions

        //- check index
        void checkIndex(const label i, const label j) const;


public:

    // Constructors

        //- Construct null
        inline FRWGraph();

        //- Construct given size
        explicit inline FRWGraph(const label size);

        //- Construct to given size and initialize
        explicit inline FRWGraph(const label size, const T& t);

        //- Copy constructor
        inline FRWGraph(const FRWGraph<T, width>&);


    //- Destructor
    ~FRWGraph() = default;


    // Member Functions

        // Access

            //- Returns the number of rows
            inline label size() const;

            //- Returns the size of a given row (obsolete)
            inline label sizeOfRow(const label rowI) const;


        // Edit

            //- Reset the number of rows
            void setSize(const label);

            //- Clear the graph
            //  Allocated size does not change
            void clear();


    // Member Operators

        //- Append a row at the end of the graph
        inline void appendFixedList(const FixedList<T, width>& l);

        //- Set row with the list
        inline void setRow(const label rowI, const FixedList<T, width>& l);

        //- check if the element is in the given row (takes linear time)
        inline bool found(const label rowI, const T& e) const;
        inline label find(const label rowI, const T& e) const;

        //- get and set operators
        inline const T& operator()(const label i, const label j) const;
        inline T& operator()(const label i, const label j);

        //- Assignment operator
        inline void operator=(const FRWGraph<T, width>&);


    // IOstream operators

        // Write FRWGraph to Ostream.
        friend Ostream& operator<< <T, width>
        (
            Ostream&,
            const FRWGraph<T, width>&
        );

        //- Read from Istream, discarding contents of existing FRWGraph.
/*        friend Istream& operator>> <T, width>
        (
            Istream&,
            FRWGraph<T, width>&
        );
*/
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "FRWGraphI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "FRWGraph.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
