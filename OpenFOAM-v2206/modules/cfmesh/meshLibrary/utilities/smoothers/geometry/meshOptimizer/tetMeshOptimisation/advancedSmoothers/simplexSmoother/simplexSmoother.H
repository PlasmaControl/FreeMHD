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
    Foam::Module::simplexSmoother

Description
    Mesh smoothing without any topological changes. The vertex is put into
    location which minimises the objective function.

SourceFiles
    simplexSmoother.C

\*---------------------------------------------------------------------------*/

#ifndef simplexSmoother_H
#define simplexSmoother_H

#include "point.H"
#include "DynList.H"
#include "partTet.H"
#include "boundBox.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

// Forward declarations
class partTetMeshSimplex;

/*---------------------------------------------------------------------------*\
                       Class simplexSmoother Declaration
\*---------------------------------------------------------------------------*/

class simplexSmoother
{
protected:

    // Protected data

        //- mesh points
        DynList<point, 128>& points_;

        //- list tets around the given vertex
        const DynList<partTet, 128>& tets_;

        //- label of the point
        const label pointI_;

        //- bound box
        boundBox bb_;


private:

    // Private member functions

        //- Disallow default bitwise copy construct
        simplexSmoother(const simplexSmoother&);

        //- Disallow default bitwise assignment
        void operator=(const simplexSmoother&);


public:

    //- construct from partTetMeshSimplex
    simplexSmoother(partTetMeshSimplex& simplex);

    //- Destructor
    virtual ~simplexSmoother() = default;


    // Member functions

        //- improve the position of the centre node
        virtual void optimizeNodePosition(const scalar tol = 0.001) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
