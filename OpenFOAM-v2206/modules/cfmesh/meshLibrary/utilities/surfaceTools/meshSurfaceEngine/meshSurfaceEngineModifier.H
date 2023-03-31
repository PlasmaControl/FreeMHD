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
    Foam::Module::meshSurfaceEngineModifier

Description
    Modifier for the meshSurfaceEngine

SourceFiles
    meshSurfaceEngine.C

\*---------------------------------------------------------------------------*/

#ifndef meshSurfaceEngineModifier_H
#define meshSurfaceEngineModifier_H

#include "meshSurfaceEngine.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
                  Class meshSurfaceEngineModifier Declaration
\*---------------------------------------------------------------------------*/

class meshSurfaceEngineModifier
{
    // Private data

        //- reference to the meshSurfaceEngine
        meshSurfaceEngine& surfaceEngine_;

        //- Disallow default bitwise copy construct
        meshSurfaceEngineModifier(const meshSurfaceEngineModifier&);

        //- Disallow default bitwise assignment
        void operator=(const meshSurfaceEngineModifier&);


public:

    // Constructors

        //- Construct from reference to meshSurfaceEngine
        meshSurfaceEngineModifier(meshSurfaceEngine& surfaceEngine);

        //- Construct from const reference to meshSurfaceEngine
        meshSurfaceEngineModifier(const meshSurfaceEngine& surfaceEngine);


    //- Destructor
    ~meshSurfaceEngineModifier() = default;


    // Member Functions

        //- relocate the selected boundary vertex
        void moveBoundaryVertexNoUpdate(const label bpI, const point& newP);

        //- relocate the selected boundary vertex and update geometry data
        void moveBoundaryVertex(const label bpI, const point& newP);

        //- update normals of boundary vertices at processor boundaries
        inline void updateVertexNormals()
        {
            if (surfaceEngine_.pointNormalsPtr_)
                surfaceEngine_.updatePointNormalsAtProcBoundaries();
        }

        //- updates faceCentres, face normal, and point normals
        //- of the boundary faces attached to the selected boundary points
        void updateGeometry(const labelLongList&);
        void updateGeometry();

        //- makes sure that all surface vertices at parallel boundaries
        //- have the same coordinates
        void syncVerticesAtParallelBoundaries();
        void syncVerticesAtParallelBoundaries(const labelLongList&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
