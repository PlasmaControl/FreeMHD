/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
-------------------------------------------------------------------------------
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

Application
    vtkUnstructuredToFoam

Group
    grpMeshConversionUtilities

Description
    Convert legacy VTK file (ascii) containing an unstructured grid
    to an OpenFOAM mesh without boundary information.

Note
    The .vtk format does not contain any boundary information.
    It is purely a description of the internal mesh.
    Not extensively tested.

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "Time.H"
#include "polyMesh.H"
#include "IFstream.H"
#include "vtkUnstructuredReader.H"
#include "fvCFD.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


int main(int argc, char *argv[])
{
    argList::addNote
    (
        "Convert legacy VTK file (ascii) containing an unstructured grid"
        " to an OpenFOAM mesh without boundary information. Also writes B file."
    );

    argList::noParallel();
    argList::addArgument("vtk-file", "The input legacy ascii vtk file");

    #include "setRootCase.H"
    #include "createTime.H"

    IFstream mshStream(args[1]);

    vtkUnstructuredReader reader(runTime, mshStream);

    polyMesh mesh
    (
        IOobject
        (
            polyMesh::defaultRegion,
            runTime.constant(),
            runTime
        ),
        std::move(reader.points()),
        reader.cells(),
        faceListList(),
        wordList(),
        wordList(),
        "defaultFaces",
        polyPatch::typeName,
        wordList()
    );
    
    //volVectorField B
//(
    //IOobject
    //(
        //"B",
        //runTime.timeName(),
        //mesh,
        //IOobject::NO_READ,
        //IOobject::AUTO_WRITE
    //),
    //mesh,
    //reader.cellData().lookupObject<volVectorField>("B")
//);
	

    // Set the precision of the points data to 10
    IOstream::defaultPrecision(max(10u, IOstream::defaultPrecision()));

    Info<< "Writing mesh ..." << endl;

    mesh.removeFiles();
    mesh.write();

#include "createMyMesh.H"
    
vectorField B_ = reader.cellData().lookupObject<vectorField>("B");

volVectorField B
(
    IOobject
    (
        "B",
        runTime.timeName(),
        myMesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    myMesh,
    dimensionedVector(dimensionSet(1, 0, -2, 0, 0, -1, 0), Zero)
);

	forAll(B,i)
	{
		B[i]=B_[i];
	}
	
	B.write();
	
    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
