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
    Foam::Module::decomposeFaces

Description
    Decompose selected mesh faces into triangles using midnode subdivision

SourceFiles
    decomposeFaces.C

\*---------------------------------------------------------------------------*/

#ifndef decomposeFaces_H
#define decomposeFaces_H

#include "polyMeshGenModifier.H"
#include "DynList.H"

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
                       Class decomposeFaces Declaration
\*---------------------------------------------------------------------------*/

class decomposeFaces
{
    // private data

        //- reference to the mesh
        polyMeshGen& mesh_;

        //- number of points
        VRWGraph newFacesForFace_;

        //- is decomposition performed
        bool done_;

        //- disallows bitwise construct
        void operator=(const decomposeFaces&);

        //- copy constructor
        decomposeFaces(const decomposeFaces&);


public:

    //- construct components
    decomposeFaces(polyMeshGen& mesh);

    //- Destructor
    ~decomposeFaces() = default;


    // Member functions

        //- decompose selected faces into triangles using midnode subdivision
        void decomposeMeshFaces(const boolList& decomposeFace);

        //- decompose internal faces containing concave nodes
        void decomposeConcaveInternalFaces(const boolList& concaveVertex);

        //- reference to a graph containing label of new faces generated
        //- from each face
        const VRWGraph& newFacesForFace() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
