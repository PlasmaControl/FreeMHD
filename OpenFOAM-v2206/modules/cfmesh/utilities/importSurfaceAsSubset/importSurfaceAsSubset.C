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
    Import a subset of a surface

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "IFstream.H"
#include "fileName.H"
#include "triSurf.H"
#include "OFstream.H"
#include "OSspecific.H"
#include "demandDrivenData.H"
#include "triSurfaceImportSurfaceAsSubset.H"

using namespace Foam;
using namespace Foam::Module;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "(cfmesh)\n"
        "Import a subset of a surface"
    );

    argList::noParallel();
    argList::validArgs.clear();
    argList::validArgs.append("master surface file");
    argList::validArgs.append("import surface file");

    argList args(argc, argv);

    fileName inFileName(args[1]);
    fileName importFileName(args[2]);

    triSurf originalSurface(inFileName);

    triSurf importedSurface(importFileName);

    triSurfaceImportSurfaceAsSubset importSurf(originalSurface);

    importSurf.addSurfaceAsSubset(importedSurface, importFileName.lessExt());

    if (inFileName.ext() == "fms")
    {
        originalSurface.writeSurface(inFileName);
    }
    else
    {
        fileName newName = inFileName.lessExt();
        newName.append(".fms");
        Warning << "Writing surface as " << newName
            << " to preserve the subset!!" << endl;

        originalSurface.writeSurface(newName);
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
