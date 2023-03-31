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

\*---------------------------------------------------------------------------*/

# ifdef USE_OMP
#include <omp.h>
# endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline const Foam::Module::VRWGraph&
Foam::Module::triSurfAddressing::pointFacets() const
{
    if (!pointFacetsPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
        {
            FatalErrorInFunction
                << "Cannot calculate pointFacets" << abort(FatalError);
        }
        # endif

        calculatePointFacets();
    }

    return *pointFacetsPtr_;
}


inline const Foam::Module::edgeLongList&
Foam::Module::triSurfAddressing::edges() const
{
    if (!edgesPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
        {
            FatalErrorInFunction
                << "Cannot calculate edges" << abort(FatalError);
        }
        # endif

        calculateEdges();
    }

    return *edgesPtr_;
}


inline const Foam::Module::VRWGraph&
Foam::Module::triSurfAddressing::facetEdges() const
{
    if (!facetEdgesPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
        {
            FatalErrorInFunction
                << "Cannot calculate facetEdges" << abort(FatalError);
        }
        # endif

        calculateFacetEdges();
    }

    return *facetEdgesPtr_;
}


inline const Foam::Module::VRWGraph&
Foam::Module::triSurfAddressing::edgeFacets() const
{
    if (!edgeFacetsPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
        {
            FatalErrorInFunction
                << "Cannot calculate edgeFacets" << abort(FatalError);
        }
        # endif

        calculateEdgeFacets();
    }

    return *edgeFacetsPtr_;
}


inline const Foam::Module::VRWGraph&
Foam::Module::triSurfAddressing::pointEdges() const
{
    if (!pointEdgesPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
        {
            FatalErrorInFunction
                << "Cannot calculate pointEdges" << abort(FatalError);
        }
        # endif

        calculatePointEdges();
    }

    return *pointEdgesPtr_;
}


inline const Foam::Module::VRWGraph&
Foam::Module::triSurfAddressing::facetFacets() const
{
    if (!facetFacetsEdgesPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
        {
            FatalErrorInFunction
                << "Cannot calculate facetFacets" << abort(FatalError);
        }
        # endif

        calculateFacetFacetsEdges();
    }

    return *facetFacetsEdgesPtr_;
}


inline const Foam::vectorField&
Foam::Module::triSurfAddressing::pointNormals() const
{
    if (!pointNormalsPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
        {
            FatalErrorInFunction
                << "Cannot calculate pointNormals" << abort(FatalError);
        }
        # endif

        calculatePointNormals();
    }

    return *pointNormalsPtr_;
}


inline const Foam::vectorField&
Foam::Module::triSurfAddressing::facetNormals() const
{
    if (!facetNormalsPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
        {
            FatalErrorInFunction
                << "Cannot calculate facetNormals" << abort(FatalError);
        }
        # endif

        calculateFacetNormals();
    }

    return *facetNormalsPtr_;
}


inline const Foam::vectorField&
Foam::Module::triSurfAddressing::facetCentres() const
{
    if (!facetCentresPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
        {
            FatalErrorInFunction
                << "Cannot calculate facetCentres" << abort(FatalError);
        }
        # endif

        calculateFacetCentres();
    }

    return *facetCentresPtr_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
