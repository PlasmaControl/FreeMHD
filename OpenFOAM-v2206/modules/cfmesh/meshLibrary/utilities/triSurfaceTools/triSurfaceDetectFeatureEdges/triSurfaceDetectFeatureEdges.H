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
    Foam::Module::triSurfaceDetectFeatureEdges

Description
    Divides the surface mesh into regions bounded by feature edges

SourceFiles
    triSurfaceDetectFeatureEdges.C
    triSurfaceDetectFeatureEdgesFunctions.C

\*---------------------------------------------------------------------------*/

#ifndef triSurfaceDetectFeatureEdges_H
#define triSurfaceDetectFeatureEdges_H

#include "triSurf.H"
#include "VRWGraph.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
                Class triSurfaceDetectFeatureEdges Declaration
\*---------------------------------------------------------------------------*/

class triSurfaceDetectFeatureEdges
{
    // Private data

        //- reference to triSurf
        triSurf& surf_;

        //- detected feature edges
        List<direction> featureEdges_;

        //- angle tolerance
        scalar angleTolerance_;


    // Private member functions

        //- detect feature edges by check angle between the normals
        //- of the two neighbouring facets
        void detectFeatureEdgesAngleCriterion();

        //- Disallow default bitwise copy construct
        triSurfaceDetectFeatureEdges(const triSurfaceDetectFeatureEdges&);

        //- Disallow default bitwise assignment
        void operator=(const triSurfaceDetectFeatureEdges&);


public:

    //- Construct from triSurface and angle tolerance (degrees)
    triSurfaceDetectFeatureEdges
    (
        triSurf& surface,
        const scalar angleDeviation = 45.0
    );


    //- Destructor
    ~triSurfaceDetectFeatureEdges() = default;


    // Member Functions

        //- store regions in a graph
        void detectFeatureEdges();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
