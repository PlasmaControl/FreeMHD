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
    Foam::Module::cellIOGraph

Description
    A graph of cells which support automated output

SourceFiles
    cellIOGraph.C

\*---------------------------------------------------------------------------*/

#ifndef cellIOGraph_H
#define cellIOGraph_H

#include "VRWGraph.H"
#include "regIOobject.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
                         Class cellIOGraph Declaration
\*---------------------------------------------------------------------------*/

class cellIOGraph
:
    public regIOobject,
    public VRWGraph
{
public:

    //- Runtime type information
    TypeName("cellIOGraph");

    // Constructors

        //- Construct from IOobject
        cellIOGraph(const IOobject&);

        //- Construct from IOobject and size of cellIOGraph
        cellIOGraph(const IOobject&, const label);

        //- Construct from IOobject and a VRWGraph
        cellIOGraph(const IOobject&, const VRWGraph&);


    // Member functions

        bool writeData(Ostream&) const;


    // Member operators

        void operator=(const cellIOGraph&);
        void operator=(const VRWGraph&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
