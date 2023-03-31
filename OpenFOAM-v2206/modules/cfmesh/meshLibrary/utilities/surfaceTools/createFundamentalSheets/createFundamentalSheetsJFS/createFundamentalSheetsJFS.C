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

#include "createFundamentalSheetsJFS.H"
#include "demandDrivenData.H"
#include "meshSurfaceEngine.H"
#include "extrudeLayer.H"

#include "addToRunTimeSelectionTable.H"

# ifdef USE_OMP
#include <omp.h>
# endif

//#define DEBUGSheets

# ifdef DEBUGSheets
#include "helperFunctions.H"
# endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

    defineTypeNameAndDebug(createFundamentalSheetsJFS, 0);
    addToRunTimeSelectionTable
    (
        createFundamentalSheets,
        createFundamentalSheetsJFS,
        polyMeshGen
    );
}
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

bool Foam::Module::createFundamentalSheetsJFS::isTopologyOk() const
{
    const PtrList<boundaryPatch>& boundaries = mesh_.boundaries();

    const label start = boundaries[0].patchStart();
    const label end
    (
        boundaries[boundaries.size()-1].patchStart() +
        boundaries[boundaries.size()-1].patchSize()
    );

    const labelList& owner = mesh_.owner();

    // count the number of boundary faces in every cell
    // cells with more than one boundary face cause problem to the
    // sheet insertion procedure
    labelList nBndFacesInCell(mesh_.cells().size(), 0);

    bool isOkTopo(true);
    for (label faceI = start; faceI < end; ++faceI)
    {
        ++nBndFacesInCell[owner[faceI]];

        if (nBndFacesInCell[owner[faceI]] > 1)
        {
            isOkTopo = false;
            break;
        }
    }

    reduce(isOkTopo, minOp<bool>());

    return isOkTopo;
}


void Foam::Module::createFundamentalSheetsJFS::createInitialSheet()
{
    if (!createWrapperSheet_)
    {
        if (isTopologyOk())
            return;

        Warning << "Found invalid topology!"
                << "\nStarting creating initial wrapper sheet" << endl;
    }

    Info<< "Creating initial wrapper sheet" << endl;

    const PtrList<boundaryPatch>& boundaries = mesh_.boundaries();

    const label start = boundaries[0].patchStart();
    const label end
    (
        boundaries[boundaries.size()-1].patchStart() +
        boundaries[boundaries.size()-1].patchSize()
    );

    const labelList& owner = mesh_.owner();

    LongList<labelPair> extrudeFaces(end - start);

    # ifdef USE_OMP
    # pragma omp parallel for schedule(guided, 100)
    # endif
    for (label faceI = start; faceI < end; ++faceI)
        extrudeFaces[faceI - start] = labelPair(faceI, owner[faceI]);

    extrudeLayer(mesh_, extrudeFaces);

    Info<< "Finished creating initial wrapper sheet" << endl;
}


void Foam::Module::createFundamentalSheetsJFS::createSheetsAtFeatureEdges()
{
    Info<< "Starting creating sheets at feature edges" << endl;

    const PtrList<boundaryPatch>& boundaries = mesh_.boundaries();

    if (returnReduce(boundaries.size(), maxOp<label>()) < 2 )
    {
        Info<< "Skipping creating sheets at feature edges" << endl;
        return;
    }

    const cellListPMG& cells = mesh_.cells();
    const labelList& owner = mesh_.owner();
    const labelList& neighbour = mesh_.neighbour();

    const label start = boundaries[0].patchStart();
    const label end
    (
        boundaries[boundaries.size()-1].patchStart() +
        boundaries[boundaries.size()-1].patchSize()
    );

    faceListPMG::subList bFaces(mesh_.faces(), end - start, start);
    labelList facePatch(bFaces.size());

    forAll(boundaries, patchI)
    {
        const label patchStart = boundaries[patchI].patchStart();
        const label patchEnd = patchStart + boundaries[patchI].patchSize();

        for (label faceI = patchStart; faceI < patchEnd; ++faceI)
            facePatch[faceI - start] = patchI;
    }

    labelList patchCell(mesh_.cells().size(), -1);
    forAll(facePatch, bfI)
        patchCell[owner[start + bfI]] = facePatch[bfI];

    # ifdef DEBUGSheets
    labelList patchSheetId(boundaries.size());
    forAll(patchSheetId, patchI)
    {
        patchSheetId[patchI] =
            mesh_.addCellSubset("sheetPatch_" + Foam::name(patchI));
    }

    forAll(patchCell, cellI)
    {
        if (patchCell[cellI] < 0)
            continue;

        mesh_.addCellToSubset(patchSheetId[patchCell[cellI]], cellI);
    }
    # endif

    LongList<labelPair> front;

    # ifdef USE_OMP
    const label nThreads = 3*omp_get_num_procs();
    # pragma omp parallel num_threads(nThreads)
    # endif
    {
        // create the front faces
        LongList<labelPair> localFront;

        # ifdef USE_OMP
        # pragma omp for
        # endif
        forAll(facePatch, bfI)
        {
            const label faceI = start + bfI;
            const label cellI = owner[faceI];

            const cell& c = cells[cellI];
            const label patchI = facePatch[bfI];

            forAll(c, fI)
            {
                if (neighbour[c[fI]] < 0)
                    continue;

                label nei = owner[c[fI]];
                if (nei == cellI)
                    nei = neighbour[c[fI]];

                if (patchCell[nei] != patchI)
                    localFront.append(labelPair(c[fI], cellI));
            }
        }

        label frontStart(-1);
        # ifdef USE_OMP
        # pragma omp critical
        # endif
        {
            frontStart = front.size();
            front.setSize(front.size()+localFront.size());
        }

        # ifdef USE_OMP
        # pragma omp barrier
        # endif

        // copy the local front into the global front
        forAll(localFront, lfI)
            front[frontStart + lfI] = localFront[lfI];
    }

    # ifdef DEBUGSheets
    const label fId = mesh_.addFaceSubset("facesForFundamentalSheets");
    const label cId = mesh_.addCellSubset("cellsForFundamentalSheets");

    forAll(front, fI)
    {
        mesh_.addFaceToSubset(fId, front[fI].first());
        mesh_.addCellToSubset(cId, front[fI].second());
    }

    mesh_.write();
    # endif

    // extrude the layer
    extrudeLayer(mesh_, front);

    Info<< "Finished creating sheets at feature edges" << endl;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::createFundamentalSheetsJFS::createFundamentalSheetsJFS
(
    polyMeshGen& mesh,
    const bool createWrapperSheet
)
:
    createFundamentalSheets(mesh, createWrapperSheet)
{
    createInitialSheet();

    createSheetsAtFeatureEdges();
}


// ************************************************************************* //
