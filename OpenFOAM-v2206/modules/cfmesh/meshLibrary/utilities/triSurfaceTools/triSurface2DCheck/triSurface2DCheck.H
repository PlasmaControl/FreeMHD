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
    Foam::Module::triSurface2DCheck

Description
    triSurf 2D checks

SourceFiles
    triSurface2DCheck.C

\*---------------------------------------------------------------------------*/

#ifndef triSurface2DCheck_H
#define triSurface2DCheck_H

#include "triSurf.H"
#include "symmTensor.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

/*---------------------------------------------------------------------------*\
                      Class triSurface2DCheck Declaration
\*---------------------------------------------------------------------------*/

class triSurface2DCheck
{
    // Private data

        //- reference to triSurf
        const triSurf& surf_;

        //- covariance matrix
        symmTensor covarianceMatrix_;


    // Private member functions

        //- create covariance matrix
        void createCovarianceMatrix();

        //- Disallow default bitwise copy construct
        triSurface2DCheck(const triSurface2DCheck&);

        //- Disallow default bitwise assignment
        void operator=(const triSurface2DCheck&);


public:

    //- Construct from octree
    triSurface2DCheck(const triSurf& surface);

    //- Destructor
    ~triSurface2DCheck() = default;


    // Member Functions

        //- checks if the surface is a 2D triangulation
        bool is2DSurface() const;

        //- create subset containing invalid facets
        void createSubsets();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
