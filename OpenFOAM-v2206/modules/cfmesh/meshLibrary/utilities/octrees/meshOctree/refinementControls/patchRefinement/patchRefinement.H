/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | cfMesh: A library for mesh generation
   \\    /   O peration     |
    \\  /    A nd           | www.cfmesh.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2014-2017 Creative Fields, Ltd.
    Copyright (C) 2020 OpenCFD Ltd.
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
    Foam::Module::patchRefinement

Description
    A class used for octree refinement. Refinement of boxes intersected
    by a given patch to the prescribed size.

SourceFiles
    patchRefinement.C

\*---------------------------------------------------------------------------*/

#ifndef patchRefinement_H
#define patchRefinement_H

#include "scalar.H"
#include "word.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

// Forward Declarations
class triSurf;
class patchRefinement;
Istream& operator>>(Istream& is, patchRefinement& pr);
Ostream& operator<<(Ostream& os, const patchRefinement& pr);

/*---------------------------------------------------------------------------*\
                       Class patchRefinement Declaration
\*---------------------------------------------------------------------------*/

class patchRefinement
{
    // Private Data

        word patchName_;

        scalar cellSize_;


public:

    // Generated Methods: copy construct, copy assign


    // Constructors

        //- Default contruct, cell-size = 0
        patchRefinement();

        //- Construct from word and desired cell size
        patchRefinement(const word&, const scalar);

        //- Construct from stream
        patchRefinement(Istream&);


    // Member Functions

        const word& patchName() const;

        scalar cellSize() const;

        label patchInSurface(const triSurf& ts) const;


    // IOstream Operators

        friend Istream& operator>>(Istream& is, patchRefinement& pr);
        friend Ostream& operator<<(Ostream& os, const patchRefinement& pr);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Global Operators
bool operator==(const patchRefinement& lhs, const patchRefinement& rhs);
bool operator!=(const patchRefinement& lhs, const patchRefinement& rhs);


} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
