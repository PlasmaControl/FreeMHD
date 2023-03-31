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
    Reads the surface mesh, remove the selected facets
    and writes the modified mesh into a new file

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "triSurf.H"
#include "triSurfaceRemoveFacets.H"

using namespace Foam;
using namespace Foam::Module;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "(cfmesh)\n"
        "Reads the surface mesh, remove the selected facets"
        " and writes the modified mesh into a new file"
    );

    argList::noParallel();
    argList::validArgs.clear();

    argList::validArgs.append("input surface file");
    argList::validArgs.append("output surface file");
    argList::validArgs.append("Subset/patch name");

    argList args(argc, argv);

    fileName inFileName(args[1]);
    fileName outFileName(args[2]);
    word patchName(args[3]);

    // read the input surface
    triSurf origSurf(inFileName);

    // remove the selected facets
    triSurfaceRemoveFacets rsf(origSurf);

    rsf.selectFacetsInPatch(patchName);

    rsf.removeFacets();

    // write the modified surface mesh
    origSurf.writeSurface(outFileName);

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
