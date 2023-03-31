/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | avalanche module
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017 Matthias Rauter
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
    slopeMesh

Description
    Create simple slope geometries for testing the granular flow solver.

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "faCFD.H"
#include "elevationModels/elevationModel/elevationModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "(avalanche)\n"
        "Create simple slope geometries for testing the granular flow solver"
    );

    #include "setRootCase.H"
    #include "createTime.H"

    const word dictName("slopeMeshDict");
    const word polyMeshDir = polyMesh::meshSubDir;

    IOdictionary demMeshDict
    (
         IOobject
         (
              dictName,
              runTime.constant(),
              runTime,
              IOobject::MUST_READ,
              IOobject::NO_WRITE
         )
    );


    if (!demMeshDict.headerOk())
    {
        FatalErrorIn(args.executable())
            << "Cannot open mesh description file\n    "
            << demMeshDict.objectPath()
            << nl
            << exit(FatalError);
    }

    autoPtr<elevationModel> elevation
    (
        elevationModel::New("elevationModel", demMeshDict)
    );


    const label cellCountX = demMeshDict.get<label>("cellCountX");
    const label cellCountY = demMeshDict.get<label>("cellCountY");
    const label cellCountZ = 1;

    const scalar minX = demMeshDict.get<scalar>("minX");
    const scalar maxX = demMeshDict.get<scalar>("maxX");
    const scalar minY = demMeshDict.get<scalar>("minY");
    const scalar maxY = demMeshDict.get<scalar>("maxY");
    const scalar meshH = demMeshDict.get<scalar>("h");

    const scalar cellCount = cellCountX*cellCountY;
    const scalar pointCount = (cellCountZ+1)*(cellCountX+1)*(cellCountY+1);
    const scalar dx = (maxX-minX)/cellCountX;
    const scalar dy = (maxY-minY)/cellCountY;
    const scalar dz = meshH/cellCountZ;
    const scalar faceCount = cellCount*2+cellCountX*(cellCountY+1)+cellCountY*(cellCountX+1);
    //back+front
    //x-paralell-faces
    //y-paralell-faces


    Info<< "creating Mesh with "
        << cellCount << " cells, "
        << faceCount << " faces and "
        << pointCount << " points" << nl;

    Info<< "x = [" << minX << ";" << maxX << "] (" << cellCountX << ")" << nl
        << "y = [" << minY << ";" << maxY << "] (" << cellCountY << ")" << nl
        << "cellsize = " << dx << "m x " << dy << "m" << endl;


    pointField points(pointCount);
    faceList faces(faceCount);
    cellList cells(cellCount);
    labelListListList pointi(cellCountX+1);
    labelListListList xplanes(cellCountX+1); //x-y-z
    labelListListList yplanes(cellCountY+1); //y-x-z
    labelListListList zplanes(cellCountZ+1); //z-x-y

    label lp = 0;
    for (int i=0; i<cellCountX+1; i++)
    {
        pointi[i] = labelListList(cellCountY+1);
        for (int j=0; j<cellCountY+1; j++)
        {
            pointi[i][j] = labelList(cellCountZ+1);
            for (int k=0; k<cellCountZ+1; k++)
            {
                points[lp] = elevation->X(vector(minX+dx*i, minY+dy*j, dz*k));
                pointi[i][j][k] = lp++;
            }
        }
    }


    label minXboundary, maxXboundary,
          minYboundary, maxYboundary,
          minZboundary, maxZboundary;

    label lf = 0;


    //creating internal x-planes
    for (int i=1; i<cellCountX; i++)
    {
        xplanes[i] = labelListList(cellCountY);
        for (int j=0; j<cellCountY; j++)
        {
            xplanes[i][j] = labelList(cellCountZ);
            for (int k=0; k<cellCountZ; k++)
            {
                face &f = faces[lf];
                f = face(4);
                f[0] = pointi[i][j][k];
                f[1] = pointi[i][j+1][k];
                f[2] = pointi[i][j+1][k+1];
                f[3] = pointi[i][j][k+1];
                xplanes[i][j][k]=lf++;
            }
        }
    }

    //creating internal y-planes
    for (int j=1; j<cellCountY; j++)
    {
        yplanes[j] = labelListList(cellCountX);
        for (int i=0; i<cellCountX; i++)
        {
            yplanes[j][i] = labelList(cellCountZ);
            for (int k=0; k<cellCountZ; k++)
            {
                face &f = faces[lf];
                f = face(4);
                f[0] = pointi[i][j][k];
                f[1] = pointi[i][j][k+1];
                f[2] = pointi[i+1][j][k+1];
                f[3] = pointi[i+1][j][k];
                yplanes[j][i][k]=lf++;

            }
        }
    }
    //creating internal z-plane
    for (int k=1; k<cellCountZ; k++)
    {
        zplanes[k] = labelListList(cellCountX);
        for (int i=0; i<cellCountX; i++)
        {
            zplanes[k][i] = labelList(cellCountY);
            for (int j=0; j<cellCountY; j++)
            {
                face &f = faces[lf];
                f = face(4);
                f[0] = pointi[i][j][k];
                f[1] = pointi[i+1][j][k];
                f[2] = pointi[i+1][j+1][k];
                f[3] = pointi[i][j+1][k];
                zplanes[k][i][j]=lf++;

            }
        }
    }


    minXboundary = lf;

    //creating x-planes
    for (int l=0; l<2; l++)
    {
        int i = l?cellCountX:0;
        xplanes[i] = labelListList(cellCountY);
        for (int j=0; j<cellCountY; j++)
        {
            xplanes[i][j] = labelList(cellCountZ);
            for (int k=0; k<cellCountZ; k++)
            {
                face &f = faces[lf];
                f = face(4);
                f[0] = pointi[i][j][k];
                f[1] = pointi[i][j][k+1];
                f[2] = pointi[i][j+1][k+1];
                f[3] = pointi[i][j+1][k];
                if (i == cellCountX)
                    f = f.reverseFace();
                xplanes[i][j][k]=lf++;
            }
        }
    }

    maxXboundary=lf-cellCountY*cellCountZ;
    minYboundary=lf;

    //creating y-planes
    for (int l=0; l<2; l++)
    {
        int j = l?cellCountY:0;
        yplanes[j] = labelListList(cellCountX);
        for (int i=0; i<cellCountX; i++)
        {
            yplanes[j][i] = labelList(cellCountZ);
            for (int k=0; k<cellCountZ; k++)
            {
                face &f = faces[lf];
                f = face(4);
                f[0] = pointi[i][j][k];
                f[1] = pointi[i+1][j][k];
                f[2] = pointi[i+1][j][k+1];
                f[3] = pointi[i][j][k+1];
                if (j == cellCountY)
                    f = f.reverseFace();
                yplanes[j][i][k]=lf++;

            }
        }
    }
    maxYboundary=lf-cellCountX*cellCountZ;
    minZboundary=lf;

    //creating z-plane
    for (int l=0; l<2; l++)
    {
        int k = l?cellCountZ:0;
        zplanes[k] = labelListList(cellCountX);
        for (int i=0; i<cellCountX; i++)
        {
            zplanes[k][i] = labelList(cellCountY);
            for (int j=0; j<cellCountY; j++)
            {
                face &f = faces[lf];
                f = face(4);
                f[0] = pointi[i][j][k];
                f[1] = pointi[i][j+1][k];
                f[2] = pointi[i+1][j+1][k];
                f[3] = pointi[i+1][j][k];
                if (k == cellCountZ)
                    f = f.reverseFace();
                zplanes[k][i][j]=lf++;

            }
        }
    }
    maxZboundary=lf-cellCountX*cellCountY;


    for (int i=0; i<cellCountX; i++)
    {
        for (int j=0; j<cellCountY; j++)
        {
            cells[i*(cellCountY)+j] = cell(6);
            cells[i*(cellCountY)+j][0] = xplanes[i][j][0];
            cells[i*(cellCountY)+j][1] = xplanes[i+1][j][0];
            cells[i*(cellCountY)+j][2] = yplanes[j][i][0];
            cells[i*(cellCountY)+j][3] = yplanes[j+1][i][0];
            cells[i*(cellCountY)+j][4] = zplanes[0][i][j];
            cells[i*(cellCountY)+j][5] = zplanes[1][i][j];
        }
    }



    word regionName = polyMesh::defaultRegion;

    polyMesh mesh
    (
        IOobject
        (
            regionName,
            runTime.constant(),
            runTime,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        std::move(points),
        std::move(faces),
        std::move(cells)
    );

    label start[6] = {minXboundary, maxXboundary,
                      minYboundary, maxYboundary,
                      minZboundary, maxZboundary};

    label size[6] = {cellCountY*cellCountZ, cellCountY*cellCountZ,
                     cellCountX*cellCountZ, cellCountX*cellCountZ,
                     cellCountX*cellCountY, cellCountX*cellCountY};

    word name[6] = {"minX", "maxX", "minY", "maxY", "slope", "maxZ"};

    List<polyPatch*> patches(6);

    for (int i=0; i<6;i++)
    {
        patches[i] = new polyPatch
        (
            name[i],
            size[i],
            start[i],
            0,
            polyBoundaryMesh
            (
                IOobject
                (
                    regionName,
                    runTime.constant(),
                    runTime
                ),
                mesh
            ),
            "test"
        );
    }

    mesh.addPatches(patches);

    Info<< nl << "Writing polyMesh" << endl;
    mesh.removeFiles();
    if (!mesh.write())
    {
        FatalErrorIn(args.executable())
            << "Failed writing polyMesh."
            << exit(FatalError);
    }
/*
    Info<< "Points of Interest:" << endl;

    const PtrList<entry> pois
    (
        demMeshDict.lookup("POI")
    );

    wordList poiNames(pois.size());

    forAll(poiNames, poiI)
    {
        const entry& poiInfo = pois[poiI];

        if (!poiInfo.isDict())
        {
            FatalIOErrorIn("demMesh.C", demMeshDict)
                << "Entry " << poiInfo << " in POI section is not a"
                << " valid dictionary." << exit(FatalIOError);
        }
        poiNames[poiI] = poiInfo.keyword();

        dictionary poiDict = poiInfo.dict();

        vector localPos = poiDict.get<vector>("localPosition");

        Info<< poiNames[poiI] << "(" << localPos << ")" << " = " << elevation->X(localPos) << endl;
    }*/


    Info<< nl << "End" << endl;

    return 0;
}


// ************************************************************************* //
