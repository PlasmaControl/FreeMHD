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

#include "triSurf.H"
#include "demandDrivenData.H"
#include "IFstream.H"
#include "OFstream.H"
#include "gzstream.h"
#include "triSurface.H"

#include "helperFunctions.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::triSurf::readFromFTR(const fileName& fName)
{
    IFstream is(fName);

    is >> triSurfFacets::patches_;
    is >> triSurfPoints::points_;
    is >> triSurfFacets::triangles_;
}


void Foam::Module::triSurf::writeToFTR(const fileName& fName) const
{
    OFstream os(fName);

    os << triSurfFacets::patches_ << nl;
    os << triSurfPoints::points_ << nl;
    os << triSurfFacets::triangles_;
}


void Foam::Module::triSurf::readFromFMS(const fileName& fName)
{
    IFstream is(fName);

    // read the list of patches defined on the surface mesh
    is >> triSurfFacets::patches_;

    // read points
    is >> triSurfPoints::points_;

    // read surface triangles
    is >> triSurfFacets::triangles_;

    // read feature edges
    is >> triSurfFeatureEdges::featureEdges_;

    List<meshSubset> subsets;

    // read point subsets
    is >> subsets;
    forAll(subsets, subsetI)
    {
        triSurfPoints::pointSubsets_.insert(subsetI, subsets[subsetI]);
    }

    subsets.clear();

    // read facet subsets
    is >> subsets;
    forAll(subsets, subsetI)
    {
        triSurfFacets::facetSubsets_.insert(subsetI, subsets[subsetI]);
    }

    subsets.clear();

    // read subsets on feature edges
    is >> subsets;
    forAll(subsets, subsetI)
    {
        triSurfFeatureEdges::featureEdgeSubsets_.insert
        (
            subsetI,
            subsets[subsetI]
        );
    }
}


void Foam::Module::triSurf::writeToFMS(const fileName& fName) const
{
    OFstream os(fName);

    // write patches
    os << triSurfFacets::patches_ << nl;

    // write points
    os << triSurfPoints::points_ << nl;

    // write triangles
    os << triSurfFacets::triangles_ << nl;

    // write feature edges
    os << triSurfFeatureEdges::featureEdges_ << nl;

    // write point subsets
    List<meshSubset> subsets;
    label i(0);
    subsets.setSize(pointSubsets_.size());
    forAllConstIters(pointSubsets_, it)
    {
        subsets[i++] = it();
    }
    os << subsets;

    os << nl;

    // write subsets of facets
    subsets.setSize(triSurfFacets::facetSubsets_.size());
    i = 0;
    forAllConstIters(triSurfFacets::facetSubsets_, it)
    {
        subsets[i++] = it();
    }
    os << subsets;

    os << nl;

    // write subets of feature edges
    subsets.setSize(triSurfFeatureEdges::featureEdgeSubsets_.size());
    i = 0;
    forAllConstIters(triSurfFeatureEdges::featureEdgeSubsets_, it)
    {
        subsets[i++] = it();
    }
    os << subsets;
}


void Foam::Module::triSurf::topologyCheck()
{
    const pointField& pts = this->points();
    const LongList<labelledTri>& trias = this->facets();

    // check for inf and nan points
    # ifdef USE_OMP
    # pragma omp parallel for schedule(dynamic, 100)
    # endif
    forAll(pts, pointI)
    {
        const point& p = pts[pointI];

        if (help::isnan(p) || help::isinf(p))
        {
            # ifdef USE_OMP
            # pragma omp critical
            # endif
            {
                FatalErrorInFunction
                    << "Point " << pointI << " has invalid coordinates "
                    << p << exit(FatalError);
            }
        }
    }

    // check whether the nodes are within the scope
    // report duplicate nodes in the same triangle
    # ifdef USE_OMP
    # pragma omp parallel for schedule(dynamic, 100)
    # endif
    forAll(trias, triI)
    {
        const labelledTri& ltri = trias[triI];

        forAll(ltri, pI)
        {
            if (ltri[pI] < 0 || (ltri[pI] >= pts.size()))
            {
                # ifdef USE_OMP
                # pragma omp critical
                # endif
                FatalErrorInFunction
                    << "Point " << ltri[pI] << " in triangle " << ltri
                    << " is out of scope 0 " << pts.size() << exit(FatalError);
            }

            if (ltri[pI] == ltri[(pI + 1)%3] || ltri[pI] == ltri[(pI + 2)%3])
            {
                # ifdef USE_OMP
                # pragma omp critical
                # endif
                WarningInFunction
                    << "Triangle " << ltri << " has duplicated points. "
                    << "This may cause problems in the meshing process!" << endl;
            }
        }
    }

    // check feature edges
    const edgeLongList& featureEdges = this->featureEdges();

    # ifdef USE_OMP
    # pragma omp parallel for schedule(dynamic, 100)
    # endif
    forAll(featureEdges, eI)
    {
        const edge& fe = featureEdges[eI];

        forAll(fe, pI)
        {
            if (fe[pI] < 0 || (fe[pI] >= pts.size()))
            {
                # ifdef USE_OMP
                # pragma omp critical
                # endif
                FatalErrorInFunction
                    << "Feature edge " << fe << " point " << fe[pI]
                    << " is out of scope 0 " << pts.size() << exit(FatalError);
            }
        }

        if (fe.start() == fe.end())
        {
            # ifdef USE_OMP
            # pragma omp critical
            # endif
            WarningInFunction
                << "Feature edge " << fe << " has duplicated points. "
                << "This may cause problems in the meshing process!" << endl;
        }
    }

    // check point subsets
    DynList<label> subsetIds;
    this->pointSubsetIndices(subsetIds);
    forAll(subsetIds, i)
    {
        labelLongList elmts;
        this->pointsInSubset(subsetIds[i], elmts);

        forAll(elmts, elmtI)
        {
            const label elI = elmts[elmtI];

            if (elI < 0 || elI >= pts.size())
            {
                # ifdef USE_OMP
                # pragma omp critical
                # endif
                FatalErrorInFunction
                    << "Point " << elI << " in point subset "
                    << this->pointSubsetName(subsetIds[i])
                    << " is out of scope 0 " << pts.size() << exit(FatalError);
            }
        }
    }

    // check face subsets
    subsetIds.clear();
    this->facetSubsetIndices(subsetIds);
    forAll(subsetIds, i)
    {
        labelLongList elmts;
        this->facetsInSubset(subsetIds[i], elmts);

        forAll(elmts, elmtI)
        {
            const label elI = elmts[elmtI];

            if (elI < 0 || elI >= trias.size())
            {
                # ifdef USE_OMP
                # pragma omp critical
                # endif
                FatalErrorInFunction
                    << "Triangle " << elI << " in facet subset "
                    << this->facetSubsetName(subsetIds[i])
                    << " is out of scope 0 " << trias.size() << exit(FatalError);
            }
        }
    }

    // check feature edge subsets
    subsetIds.clear();
    this->edgeSubsetIndices(subsetIds);
    forAll(subsetIds, i)
    {
        labelLongList elmts;
        this->edgesInSubset(subsetIds[i], elmts);

        forAll(elmts, elmtI)
        {
            const label elI = elmts[elmtI];

            if (elI < 0 || elI >= featureEdges.size())
            {
                # ifdef USE_OMP
                # pragma omp critical
                # endif
                FatalErrorInFunction
                    << "Feature edge " << elI << " in edge subset "
                    << this->edgeSubsetName(subsetIds[i])
                    << " is out of scope 0 " << featureEdges.size()
                    << exit(FatalError);
            }
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::Module::triSurf::triSurf()
:
    triSurfPoints(),
    triSurfFacets(),
    triSurfFeatureEdges(),
    triSurfAddressing(triSurfPoints::points_, triSurfFacets::triangles_)
{}


Foam::Module::triSurf::triSurf
(
    const LongList<labelledTri>& triangles,
    const geometricSurfacePatchList& patches,
    const edgeLongList& featureEdges,
    const pointField& points
)
:
    triSurfPoints(points),
    triSurfFacets(triangles, patches),
    triSurfFeatureEdges(featureEdges),
    triSurfAddressing(triSurfPoints::points_, triSurfFacets::triangles_)
{
    topologyCheck();
}


Foam::Module::triSurf::triSurf(const fileName& fName)
:
    triSurfPoints(),
    triSurfFacets(),
    triSurfFeatureEdges(),
    triSurfAddressing(triSurfPoints::points_, triSurfFacets::triangles_)
{
    readSurface(fName);

    topologyCheck();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::triSurf::readSurface(const fileName& fName)
{
    if (fName.ext() == "fms" || fName.ext() == "FMS")
    {
        readFromFMS(fName);
    }
    else if (fName.ext() == "ftr" || fName.ext() == "FTR")
    {
        readFromFTR(fName);
    }
    else
    {
        triSurface copySurface(fName);

        // copy the points
        triSurfPoints::points_.setSize(copySurface.points().size());
        forAll(copySurface.points(), pI)
        {
            triSurfPoints::points_[pI] = copySurface.points()[pI];
        }

        // copy the triangles
        triSurfFacets::triangles_.setSize(copySurface.size());
        forAll(copySurface, tI)
        {
            triSurfFacets::triangles_[tI] = copySurface[tI];
        }

        // copy patches
        triSurfFacets::patches_ = copySurface.patches();
    }
}


void Foam::Module::triSurf::writeSurface(const fileName& fName) const
{
    if (fName.ext() == "fms" || fName.ext() == "FMS")
    {
        writeToFMS(fName);
    }
    else if (fName.ext() == "ftr" || fName.ext() == "FTR")
    {
        writeToFTR(fName);
    }
    else
    {
        const pointField& pts = this->points();
        const LongList<labelledTri>& facets = this->facets();
        const geometricSurfacePatchList& patches = this->patches();

        List<labelledTri> newTrias(facets.size());
        forAll(facets, tI)
        {
            newTrias[tI] = facets[tI];
        }

        triSurface newSurf(newTrias, patches, pts);
        newSurf.write(fName);
    }
}


// ************************************************************************* //
