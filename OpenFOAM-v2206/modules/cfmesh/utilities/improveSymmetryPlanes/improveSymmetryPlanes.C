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
    Ensures that all mesh points belonging to a symmetryPlane are
    in a plane.

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "polyMeshGenModifier.H"
#include "symmetryPlaneOptimisation.H"

using namespace Foam;
using namespace Foam::Module;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "(cfmesh)\n"
        "Ensures that all mesh points belonging to a symmetryPlane are"
        " in a plane."
    );

    #include "setRootCase.H"
    #include "createTime.H"

    polyMeshGen pmg(runTime);
    pmg.read();

    Info<< "Starting optimisation of symmetry planes" << endl;
    symmetryPlaneOptimisation(pmg).optimizeSymmetryPlanes();

    Info<< "Writing mesh" << endl;
    pmg.write();

    Info<< "End\n" << endl;
    return 0;
}


// ************************************************************************* //
