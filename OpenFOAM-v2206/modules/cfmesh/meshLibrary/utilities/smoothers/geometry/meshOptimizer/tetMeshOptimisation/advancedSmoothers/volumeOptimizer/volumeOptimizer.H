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
    Foam::Module::volumeOptimizer

Description
    Mesh smoothing without any topological changes. The vertex is put into
    location which minimises the objective function.

SourceFiles
    volumeOptimizer.C

\*---------------------------------------------------------------------------*/

#ifndef volumeOptimizer_H
#define volumeOptimizer_H

#include "simplexSmoother.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{
// Forward declarations
class partTetMeshSimplex;

/*---------------------------------------------------------------------------*\
                       Class volumeOptimizer Declaration
\*---------------------------------------------------------------------------*/

class volumeOptimizer
:
    public simplexSmoother
{
    // Private static data

        //- direction vectors for divide and conquer algorithm
        static const vector dirVecs[8];


    // Private member functions

        //- evaluate functional
        scalar evaluateFunc() const;

        //- find appropriate value of K
        scalar evaluateStabilisationFactor() const;

        //- evaluate gradients of the functional
        void evaluateGradientsExact(vector&, tensor&) const;

        //- optimize position using a divide and conquer algorithm
        scalar optimiseDivideAndConquer(const scalar tol);

        //- optimise using the steepest descent
        scalar optimiseSteepestDescent(const scalar tol);


    public:

        //- Construct from partTetMeshSimplex
        volumeOptimizer(partTetMeshSimplex& simplex);

        //- Destructor
        ~volumeOptimizer() = default;


        // Member functions

            //- Find the best position for the node
            void optimizeNodePosition(const scalar tol = 0.001);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
