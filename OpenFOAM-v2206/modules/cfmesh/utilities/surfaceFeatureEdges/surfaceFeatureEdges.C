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
    Finds feature edges and corners of a triangulated surface

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "IFstream.H"
#include "fileName.H"
#include "triSurf.H"
#include "OFstream.H"
#include "OSspecific.H"
#include "demandDrivenData.H"
#include "triSurfaceDetectFeatureEdges.H"
#include "triSurfacePatchManipulator.H"

using namespace Foam;
using namespace Foam::Module;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "(cfmesh)\n"
        "Finds feature edges and corners of a triangulated surface"
    );

    argList::noParallel();
    argList::validArgs.clear();
    argList::validArgs.append("input surface file");
    argList::validArgs.append("output surface file");
    argList::addOption("angle", "scalar", "feature angle (degrees)");
    argList args(argc, argv);

    const fileName inFileName(args[1]);
    const fileName outFileName(args[2]);

    if (outFileName == inFileName)
    {
        FatalErrorInFunction
            << "Output file " << outFileName
            << " would overwrite the input file."
            << exit(FatalError);
    }

    scalar tol(45.0);
    if (!args.readIfPresent("angle", tol))
    {
        Info<< "Using 45 deg as default angle!" << endl;
    }

    triSurf originalSurface(inFileName);

    triSurfaceDetectFeatureEdges edgeDetector(originalSurface, tol);
    edgeDetector.detectFeatureEdges();

    if (outFileName.ext() == "fms" || outFileName.ext() == "FMS")
    {
        Info<< "Writing : " << outFileName << endl;
        originalSurface.writeSurface(outFileName);
    }
    else
    {
        triSurfacePatchManipulator manipulator(originalSurface);
        const triSurf* newSurfPtr = manipulator.surfaceWithPatches();

        Info<< "Writing : " << outFileName << endl;
        newSurfPtr->writeSurface(outFileName);

        deleteDemandDrivenData(newSurfPtr);
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
