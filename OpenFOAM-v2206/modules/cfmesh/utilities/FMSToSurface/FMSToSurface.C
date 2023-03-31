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
    Convert a FMS file to another surface format

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "triSurf.H"
#include "triSurfaceCopyParts.H"
#include "demandDrivenData.H"
#include "OFstream.H"

using namespace Foam;
using namespace Foam::Module;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void exportFeatureEdges
(
    const triSurf& origSurf,
    const fileName& edgeFileName
)
{
    OFstream file(edgeFileName);

    const pointField& points = origSurf.points();
    labelList newPointLabel(points.size(), -1);
    label nPoints(0);

    const edgeLongList& featureEdges = origSurf.featureEdges();
    forAll(featureEdges, feI)
    {
        const edge& e = featureEdges[feI];

        if (newPointLabel[e[0]] == -1)
            newPointLabel[e[0]] = nPoints++;
        if (newPointLabel[e[1]] == -1)
            newPointLabel[e[1]] = nPoints++;
    }

    pointField pCopy(nPoints);
    forAll(newPointLabel, pI)
    {
        if (newPointLabel[pI] < 0)
            continue;

        pCopy[newPointLabel[pI]] = points[pI];
    }

    // write the header
    file << "# vtk DataFile Version 3.0\n";
    file << "vtk output\n";
    file << "ASCII\n";
    file << "DATASET POLYDATA\n";

    // write points
    file << "POINTS " << pCopy.size() << " float\n";
    forAll(pCopy, pI)
    {
        const point& p = pCopy[pI];
        file << p.x() << ' ' << p.y() << ' ' << p.z() << '\n';
    }

    file << "\nLINES " << featureEdges.size()
         << ' ' << 3*featureEdges.size() << nl;
    forAll(featureEdges, edgeI)
    {
        const edge& e = featureEdges[edgeI];
        file << "2 " << newPointLabel[e[0]]
             << token::SPACE << newPointLabel[e[1]] << nl;
    }
    file << nl;

    if (!file)
        FatalErrorInFunction
            << "Writting of feature edges failed!" << exit(FatalError);
}


int main(int argc, char *argv[])
{
    argList::addNote
    (
        "(cfmesh)\n"
        "Convert a FMS file to another surface format."
    );

    argList::noParallel();
    argList::validArgs.clear();

    argList::validArgs.append("input surface file");
    argList::validArgs.append("output surface file");
    argList::addBoolOption("exportSubsets");
    argList::addBoolOption("exportFeatureEdges");
    argList args(argc, argv);

    const fileName inFileName(args[1]);
    const fileName outFileName(args[2]);

    fileName outFileNoExt = outFileName.lessExt();
    fileName outExtension = outFileName.ext();

    Info<< "Out file no ext " << outFileNoExt << endl;
    Info<< "Extension " << outExtension << endl;

    // read the inout surface
    triSurf origSurf(inFileName);

    // write the surface in the requated format
    origSurf.writeSurface(outFileName);

    // export surface subsets as separate surface meshes
    if (args.found("exportSubsets"))
    {
        DynList<label> subsetIDs;
        origSurf.facetSubsetIndices(subsetIDs);

        triSurfaceCopyParts copyParts(origSurf);

        forAll(subsetIDs, subsetI)
        {
            // get the name of the subset
            triSurf copySurf;
            wordList subsetName(1);
            subsetName[0] = origSurf.facetSubsetName(subsetIDs[subsetI]);

            // create a surface mesh corresponding to the subset
            copyParts.copySurface(subsetName, copySurf);

            // write the mesh on disk
            fileName fName = outFileNoExt+"_facetSubset_"+subsetName[0];
            fName += '.'+outExtension;

            copySurf.writeSurface(fName);
        }
    }

    if (args.found("exportFeatureEdges"))
    {
        fileName fName = outFileNoExt+"_featureEdges";
        fName += ".vtk";
        exportFeatureEdges(origSurf, fName);
    }

    Info<< "End\n" << endl;
    return 0;
}


// ************************************************************************* //
