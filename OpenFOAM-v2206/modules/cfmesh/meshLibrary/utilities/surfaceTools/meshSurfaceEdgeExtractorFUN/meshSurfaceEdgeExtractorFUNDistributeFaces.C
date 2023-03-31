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

#include "demandDrivenData.H"
#include "meshSurfaceEdgeExtractorFUN.H"
#include "meshOctree.H"
#include "triSurf.H"
#include "meshSurfaceEngine.H"
#include "meshSurfaceMapper.H"
#include "helperFunctions.H"
#include "createFundamentalSheetsJFS.H"
#include "meshSurfaceOptimizer.H"
#include "meshSurfaceCheckEdgeTypes.H"
#include "meshSurfaceEngine.H"

# ifdef USE_OMP
#include <omp.h>
# endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::meshSurfaceEdgeExtractorFUN::distributeBoundaryFaces()
{
    meshSurfaceEngine mse(mesh_);

    const faceList::subList& bFaces = mse.boundaryFaces();
    const labelList& faceOwner = mse.faceOwners();
    const pointFieldPMG& points = mse.points();

    // set size of patchNames, newBoundaryFaces_ and newBoundaryOwners_
    const triSurf& surface = meshOctree_.surface();
    const label nPatches = surface.patches().size();

    wordList patchNames(nPatches);
    VRWGraph newBoundaryFaces;
    labelLongList newBoundaryOwners(bFaces.size());
    labelLongList newBoundaryPatches(bFaces.size());

    // set patchNames
    forAll(surface.patches(), patchI)
        patchNames[patchI] = surface.patches()[patchI].name();

    // append boundary faces
    forAll(bFaces, bfI)
    {
        newBoundaryFaces.appendList(bFaces[bfI]);
        newBoundaryOwners[bfI] = faceOwner[bfI];
    }

    // find the region for face by finding the patch nearest
    // to the face centre
    # ifdef USE_OMP
    # pragma omp parallel for if (bFaces.size() > 100) schedule(guided)
    # endif
    forAll(bFaces, bfI)
    {
        const point c = bFaces[bfI].centre(points);

        label facePatch, nt;
        point p;
        scalar distSq;

        meshOctree_.findNearestSurfacePoint(p, distSq, nt, facePatch, c);

        if ((facePatch > -1) && (facePatch < nPatches))
        {
            newBoundaryPatches[bfI] = facePatch;
        }
        else
        {
            FatalErrorInFunction
                << "Cannot distribute a face " << bFaces[bfI] << " into any "
                << "surface patch!. Exiting.." << exit(FatalError);
        }
    }

    polyMeshGenModifier(mesh_).replaceBoundary
    (
        patchNames,
        newBoundaryFaces,
        newBoundaryOwners,
        newBoundaryPatches
    );
}


void Foam::Module::meshSurfaceEdgeExtractorFUN::reviseCorners()
{}


void Foam::Module::meshSurfaceEdgeExtractorFUN::reviseEdges()
{}


void Foam::Module::meshSurfaceEdgeExtractorFUN::remapBoundaryPoints()
{
    meshSurfaceEngine& mse = surfaceEngine();
    meshSurfaceMapper mapper(mse, meshOctree_);

    mapper.mapVerticesOntoSurfacePatches();
}


void Foam::Module::meshSurfaceEdgeExtractorFUN::createBasicFundamentalSheets()
{
    createFundamentalSheetsJFS edgeSheets(mesh_, createWrapperSheet_);

    clearOut();
}


void Foam::Module::meshSurfaceEdgeExtractorFUN::smoothMeshSurface()
{
    meshSurfaceEngine& mse = surfaceEngine();

    meshSurfaceOptimizer mso(mse, meshOctree_);
    mso.optimizeSurface();
}


void Foam::Module::meshSurfaceEdgeExtractorFUN::
improveQualityOfFundamentalSheets()
{
    const meshSurfaceEngine& mse = surfaceEngine();
    const edgeList& edges = mse.edges();

    meshSurfaceCheckEdgeTypes edgeCheck(mse);

    label id = mesh_.addPointSubset("convexEdges");
    labelLongList helper;
    edgeCheck.convexEdges(helper);
    forAll(helper, i)
    {
        mesh_.addPointToSubset(id, edges[helper[i]].start());
        mesh_.addPointToSubset(id, edges[helper[i]].end());
    }

    id = mesh_.addPointSubset("concaveEdges");
    edgeCheck.concaveEdges(helper);
    forAll(helper, i)
    {
        mesh_.addPointToSubset(id, edges[helper[i]].start());
        mesh_.addPointToSubset(id, edges[helper[i]].end());
    }
}


// ************************************************************************* //
