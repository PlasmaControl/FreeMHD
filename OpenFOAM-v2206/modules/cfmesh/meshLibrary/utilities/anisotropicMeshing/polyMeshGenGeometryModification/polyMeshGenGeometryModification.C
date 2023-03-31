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

#include "polyMeshGenGeometryModification.H"
#include "dictionary.H"
#include "demandDrivenData.H"

// * * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * //

void Foam::Module::polyMeshGenGeometryModification::checkModification()
{
    if (meshDict_.found("anisotropicSources"))
    {
        modificationActive_ = true;

        const dictionary& anisotropicDict =
            meshDict_.subDict("anisotropicSources");

        coordinateModifierPtr_ = new coordinateModifier(anisotropicDict);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::polyMeshGenGeometryModification::polyMeshGenGeometryModification
(
    polyMeshGen& mesh,
    const dictionary& meshDict
)
:
    mesh_(mesh),
    meshDict_(meshDict),
    coordinateModifierPtr_(nullptr),
    modificationActive_(false)
{
    checkModification();
}


Foam::Module::polyMeshGenGeometryModification::
~polyMeshGenGeometryModification()
{
    deleteDemandDrivenData(coordinateModifierPtr_);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

bool Foam::Module::polyMeshGenGeometryModification::activeModification() const
{
    return modificationActive_;
}


void Foam::Module::polyMeshGenGeometryModification::modifyGeometry()
{
    if (!modificationActive_)
    {
        WarningInFunction
            << "Modification is not active" << endl;

        return;
    }

    pointFieldPMG& pts = mesh_.points();

    # ifdef USE_OMP
    # pragma omp parallel for schedule(dynamic, 50)
    # endif
    forAll(pts, pointI)
    {
        pts[pointI] = coordinateModifierPtr_->modifiedPoint(pts[pointI]);
    }
}


void Foam::Module::polyMeshGenGeometryModification::revertGeometryModification()
{
    if (!modificationActive_)
    {
        WarningInFunction
            << "Modification is not active" << endl;

        return;
    }

    pointFieldPMG& pts = mesh_.points();

    # ifdef USE_OMP
    # pragma omp parallel for schedule(dynamic, 50)
    # endif
    forAll(pts, pointI)
    {
        pts[pointI] =
            coordinateModifierPtr_->backwardModifiedPoint(pts[pointI]);
    }
}


// ************************************************************************* //
