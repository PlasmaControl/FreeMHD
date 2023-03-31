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

Description
    Performs point relocations in the mesh (smoothing) in order to
    improve quality measures. It does not make the mesh invalid.

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "polyMeshGenModifier.H"
#include "meshOptimizer.H"

using namespace Foam;
using namespace Foam::Module;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "(cfmesh)\n"
        "Performs point relocations in the mesh (smoothing) in order to"
        " improve quality measures."
    );

    argList::validArgs.clear();

    argList::addOption("nLoops", "int");
    argList::addOption("nIterations", "int");
    argList::addOption("nSurfaceIterations", "int");
    argList::addOption("qualityThreshold", "scalar");
    argList::addOption("constrainedCellsSet", "word");

    #include "setRootCase.H"
    #include "createTime.H"

    // Defaults
    label nIterations(50);
    label nLoops(10);
    label nSurfaceIterations(2);
    scalar qualityThreshold(0.1);

    // Read the settings

    if (!args.readIfPresent("nLoops", nLoops))
    {
        Info<< "Default number of loops is "
            << nLoops << endl;
    }

    if (!args.readIfPresent("nIterations", nIterations))
    {
        Info<< "Default number of iterations is "
            << nIterations << endl;
    }

    if (!args.readIfPresent("nSurfaceIterations", nSurfaceIterations))
    {
        Info<< "Default number of surface iterations is "
            << nSurfaceIterations << endl;
    }

    if (!args.readIfPresent("qualityThreshold", qualityThreshold))
    {
        Info<< "Using default quality threshold 0.1" << endl;
    }

    word constrainedCellSet;
    if (!args.readIfPresent("constrainedCellSet", constrainedCellSet))
    {
        Info<< "No constraints applied on the smoothing procedure" << endl;
    }

    // load the mesh from disk
    polyMeshGen pmg(runTime);
    pmg.read();

    // construct the smoother
    meshOptimizer mOpt(pmg);

    if (!constrainedCellSet.empty())
    {
        // lock cells in constrainedCellSet
        mOpt.lockCellsInSubset(constrainedCellSet);

        // find boundary faces which shall be locked
        labelLongList lockedBndFaces, selectedCells;

        const label sId = pmg.cellSubsetIndex(constrainedCellSet);
        pmg.cellsInSubset(sId, selectedCells);

        boolList activeCell(pmg.cells().size(), false);
        forAll(selectedCells, i)
            activeCell[selectedCells[i]] = true;
    }

    // clear geometry information before volume smoothing
    pmg.clearAddressingData();

    // perform optimisation using the laplace smoother and
    mOpt.optimizeMeshFV
    (
        nLoops,
        nLoops,
        nIterations,
        nSurfaceIterations
    );

    // perform optimisation of worst quality faces
    mOpt.optimizeMeshFVBestQuality(nLoops, qualityThreshold);

    // check the mesh again and untangl bad regions if any of them exist
    mOpt.untangleMeshFV(nLoops, nIterations, nSurfaceIterations);

    Info<< "Writing mesh" << endl;
    pmg.write();

    Info<< "End\n" << endl;
    return 0;
}


// ************************************************************************* //
