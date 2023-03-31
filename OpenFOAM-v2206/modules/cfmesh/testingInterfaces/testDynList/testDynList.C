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
    Basic tests of DynList

\*---------------------------------------------------------------------------*/

#include "DynList.H"
#include "scalar.H"
#include "vector.H"

using namespace Foam;
using namespace Foam::Module;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    DynList<label> a;
    DynList<label> b(100);
    DynList<scalar> c(1000, 0.1);

    List<vector> v(1000, vector::zero);
    DynList<vector> d(v);

    Info<< "b " << b << endl;

    c.append(0.2);

    b.appendUniq(3);

    c(1020) = 0.5;

    Info<< "c " << c << endl;

    Info<< "d " << d << endl;

    DynList<DynList<label>> e;
    e.setSize(5);
    forAll(e, i)
        e[i].setSize(18);

    Info<< "e " << e << endl;

    c.setSize(5);
    c.shrink();

    Info<< "\nEnd" << endl;
    return 0;
}


// ************************************************************************* //
