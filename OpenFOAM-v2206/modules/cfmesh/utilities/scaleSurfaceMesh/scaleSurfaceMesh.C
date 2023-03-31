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
    Scales surface vertices by a prescribed factor.

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "IFstream.H"
#include "fileName.H"
#include "triSurf.H"
#include "triSurfModifier.H"

using namespace Foam;
using namespace Foam::Module;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "(cfmesh)\n"
        "Scales surface vertices by a prescribed factor"
    );

    argList::noParallel();
    argList::validArgs.clear();
    argList::validArgs.append("input surface file");
    argList::validArgs.append("output surface file");
    argList::validArgs.append("scalingFactor");
    argList args(argc, argv);

    const fileName inFileName(args[1]);
    const fileName outFileName(args[2]);
    const scalar scalingFactor(args.get<scalar>(3));

    // read the surface mesh
    triSurf surface(inFileName);
    triSurfModifier sMod(surface);
    pointField& pts = sMod.pointsAccess();

    // scales the vertices
    # ifdef USE_OMP
    # pragma omp parallel for schedule(dynamic, 100)
    # endif
    forAll(pts, pointI)
        pts[pointI] *= scalingFactor;

    // write the mesh back on disk
    surface.writeSurface(outFileName);

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
