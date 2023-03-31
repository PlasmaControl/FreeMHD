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
    Foam::Module::VRWGraphSMPModifier

Description
    This class is a modifier for VRWGraph which allows for multi-threaded
    execution of the most time-consuimg functions

SourceFiles
    VRWGraphSMPModifier.H
    VRWGraphSMPModifier.C

\*---------------------------------------------------------------------------*/

#ifndef VRWGraphSMPModifier_H
#define VRWGraphSMPModifier_H

#include "VRWGraph.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
                     Class VRWGraphSMPModifier Declaration
\*---------------------------------------------------------------------------*/

class VRWGraphSMPModifier
{
    // Private data

        //- list containing the data
        VRWGraph& graph_;


    // Private member functions

        //- Disallow default construct
        VRWGraphSMPModifier();

        //- Disallow bitwise copy construct
        VRWGraphSMPModifier(const VRWGraphSMPModifier&);

        //- Disallow bitwise assignment
        void operator=(const VRWGraphSMPModifier&);


public:

    //- Construct from reference to VRWGraph
    VRWGraphSMPModifier(VRWGraph& graph);

    //- Destructor
    ~VRWGraphSMPModifier() = default;


    // Member Functions

        //- set the size and row sizes
        template<class ListType>
        void setSizeAndRowSize(const ListType&);

        //- merge graphs with the identical number of rows
        //- into a single one. Use for SMP parallelisation
        void mergeGraphs(const List<VRWGraph>& graphParts);

        //- set the graph to the reverse of the original graph.
        //- the rows of such graph store the rows which contain the elements
        //- of the original graph
        template<class GraphType>
        void reverseAddressing(const GraphType& origGraph);

        void reverseAddressing(const VRWGraph& origGraph);

        //- set the graph to the reverse of the original graph and mapped
        //- to another space.
        template<class ListType, class GraphType>
        void reverseAddressing(const ListType&, const GraphType&);

        template<class ListType>
        void reverseAddressing(const ListType&, const VRWGraph&);

        //- optimize memory usage
        // this should be used once the graph will not be resized any more
        void optimizeMemoryUsage();

        //- Assignment operator
        void operator=(const VRWGraph&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#    include "VRWGraphSMPModifierTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
