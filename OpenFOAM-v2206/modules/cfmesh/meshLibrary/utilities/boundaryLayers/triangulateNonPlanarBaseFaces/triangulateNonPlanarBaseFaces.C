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

\*---------------------------------------------------------------------------*/

#include "triangulateNonPlanarBaseFaces.H"
#include "dictionary.H"
#include "demandDrivenData.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::triangulateNonPlanarBaseFaces::triangulateNonPlanarBaseFaces
(
    polyMeshGen& mesh
)
:
    mesh_(mesh),
    invertedCell_(mesh_.cells().size(), false),
    decomposeFace_(mesh_.faces().size(), false),
    tol_(0.5)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::triangulateNonPlanarBaseFaces::setRelativeTolerance
(
    const scalar tol
)
{
    tol_ = tol;
}


void Foam::Module::triangulateNonPlanarBaseFaces::triangulateLayers()
{
    if (findNonPlanarBoundaryFaces())
    {
        Info<< "Decomposing twisted boundary faces" << endl;

        decomposeBoundaryFaces();

        decomposeCellsIntoPyramids();
    }
    else
    {
        Info<< "All boundary faces are flat" << endl;
    }
}


void Foam::Module::triangulateNonPlanarBaseFaces::readSettings
(
    const dictionary& meshDict,
    triangulateNonPlanarBaseFaces& triangulator
)
{
    if (meshDict.found("boundaryLayers"))
    {
        const dictionary& layersDict = meshDict.subDict("boundaryLayers");

        if (layersDict.found("optimisationParameters"))
        {
            const dictionary& optLayerDict =
                layersDict.subDict("optimisationParameters");

            scalar relTol;
            if (optLayerDict.readIfPresent("relFlatnessTol", relTol))
            {
                triangulator.setRelativeTolerance(relTol);
            }
        }
    }
}


// ************************************************************************* //
