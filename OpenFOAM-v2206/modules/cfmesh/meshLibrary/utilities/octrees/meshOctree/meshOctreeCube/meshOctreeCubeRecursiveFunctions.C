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

#include "meshOctreeCube.H"
#include "demandDrivenData.H"
#include "Ostream.H"
#include "meshOctree.H"

//#define DEBUGSearch

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Module::meshOctreeCube::leavesInBox
(
    const boundBox& rootBox,
    const boundBox& searchingBox,
    DynList<const meshOctreeCube*, 256>& leaves
) const
{
    boundBox cubeBox;
    this->cubeBox(rootBox, cubeBox.min(), cubeBox.max());

    if (cubeBox.overlaps(searchingBox))
    {
        if (this->isLeaf())
        {
            leaves.append(this);
        }
        else
        {
            for (label scI = 0; scI < 8; ++scI)
            {
                meshOctreeCube* scPtr = subCubesPtr_[scI];

                if (scPtr)
                {
                    scPtr->leavesInBox
                    (
                        rootBox,
                        searchingBox,
                        leaves
                    );
                }
                else if (Pstream::parRun())
                {
                    meshOctreeCubeCoordinates cc = refineForPosition(scI);

                    boundBox bb;
                    cc.cubeBox(rootBox, bb.min(), bb.max());

                    if (bb.overlaps(searchingBox))
                        leaves.append(this);
                }
            }
        }
    }
}


void Foam::Module::meshOctreeCube::leavesInSphere
(
    const boundBox& rootBox,
    const point& c,
    const scalar r,
    DynList<label>& containedLeaves
) const
{
    const point cubeCentre = this->centre(rootBox);
    const scalar size = 1.732*this->size(rootBox);

    if (magSqr(cubeCentre-c) < sqr(r + size))
    {
        if (this->isLeaf())
        {
            containedLeaves.append(this->cubeLabel());
        }
        else
        {
            for (label scI = 0; scI < 8; ++scI)
            {
                meshOctreeCube* scPtr = subCubesPtr_[scI];

                if (scPtr)
                {
                    scPtr->leavesInSphere
                    (
                        rootBox,
                        c,
                        r,
                        containedLeaves
                    );
                }
                else if (Pstream::parRun())
                {
                    meshOctreeCubeCoordinates cc = refineForPosition(scI);
                    const point sc = cc.centre(rootBox);

                    if (magSqr(sc - c) < sqr(r + size))
                        containedLeaves.append(meshOctreeCube::OTHERPROC);
                }
            }
        }
    }
}


void Foam::Module::meshOctreeCube::markLeavesInSphere
(
    const boundBox& rootBox,
    const point& c,
    const scalar r,
    labelList& markedLeaves,
    bool& atProcessorBnd
) const
{
    const point cubeCentre = this->centre(rootBox);
    const scalar size = 1.732*this->size(rootBox);

    if (magSqr(cubeCentre-c) < sqr(r + size))
    {
        if (this->isLeaf())
        {
            markedLeaves[this->cubeLabel()] |= 2;
        }
        else
        {
            for (label scI = 0; scI < 8; ++scI)
            {
                meshOctreeCube* scPtr = subCubesPtr_[scI];

                if (scPtr)
                {
                    scPtr->markLeavesInSphere
                    (
                        rootBox,
                        c,
                        r,
                        markedLeaves,
                        atProcessorBnd
                    );
                }
                else if (Pstream::parRun())
                {
                    meshOctreeCubeCoordinates cc = refineForPosition(scI);
                    const point sc = cc.centre(rootBox);

                    if (magSqr(sc - c) < sqr(r + size))
                    {
                        atProcessorBnd = true;
                    }
                }
            }
        }
    }
}


void Foam::Module::meshOctreeCube::findLeaves
(
    LongList<meshOctreeCube*>& leaves
) const
{
    if (this->isLeaf())
    {
        meshOctreeCube* oc = const_cast<meshOctreeCube*>(this);
        cubeLabel_ = leaves.size();
        leaves.append(oc);
    }
    else
    {
        cubeLabel_ = -1;

        for (label scI = 0; scI < 8; ++scI)
        {
            const meshOctreeCube* scPtr = subCubesPtr_[scI];

            if (scPtr)
                scPtr->findLeaves(leaves);
        }
    }
}


void Foam::Module::meshOctreeCube::findCoordinatesOfMissingCubes
(
    LongList<meshOctreeCubeCoordinates>& coordinates
) const
{
    if (this->isLeaf())
        return;

    for (label scI = 0; scI < 8; ++scI)
    {
        meshOctreeCube* scPtr = subCubesPtr_[scI];

        if (scPtr)
        {
            scPtr->findCoordinatesOfMissingCubes(coordinates);
        }
        else
        {
            coordinates.append(this->refineForPosition(scI));
        }
    }
}


void Foam::Module::meshOctreeCube::countChildCubes(label& counter) const
{
    ++counter;

    if (!this->isLeaf())
    {
        for (label scI = 0; scI < 8; ++scI)
        {
            meshOctreeCube* scPtr = subCubesPtr_[scI];

            if (scPtr)
            {
                scPtr->countChildCubes(counter);
            }
        }
    }
}


bool Foam::Module::meshOctreeCube::purgeProcessorCubes(const short procNo)
{
    if (this->procNo() == ALLPROCS)
    {
        this->setProcNo(procNo);
    }

    if (this->isLeaf())
    {
        if (this->procNo() != procNo)
        {
            return true;
        }

        return false;
    }
    else
    {
        label mergedSubcubes = 0;
        for (label scI = 0; scI < 8; ++scI)
        {
            if (!subCubesPtr_[scI])
            {
                mergedSubcubes |= 1 << scI;
                continue;
            }

            if (subCubesPtr_[scI]->purgeProcessorCubes(procNo))
            {
                subCubesPtr_[scI] = nullptr;
                mergedSubcubes |= 1 << scI;
            }
        }

        if (mergedSubcubes == 255)
        {
            subCubesPtr_ = nullptr;

            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}


// ************************************************************************* //
