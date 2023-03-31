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

#include "polyMeshGenPoints.H"
#include "pointIOField.H"
#include "IOobjectList.H"
#include "pointSet.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::polyMeshGenPoints::polyMeshGenPoints(const Time& runTime)
:
    runTime_(runTime),
    points_
    (
        IOobject
        (
            "points",
            runTime.constant(),
            "polyMesh",
            runTime
        ),
        0
    ),
    pointSubsets_()
{}


Foam::Module::polyMeshGenPoints::polyMeshGenPoints
(
    const Time& runTime,
    const pointField& points
)
:
    runTime_(runTime),
    points_
    (
        IOobject
        (
            "points",
            runTime.constant(),
            "polyMesh",
            runTime
        ),
        points
    ),
    pointSubsets_()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::label Foam::Module::polyMeshGenPoints::addPointSubset
(
    const word& subsetName
)
{
    label id = pointSubsetIndex(subsetName);
    if (id >= 0)
    {
        Warning << "Point subset " << subsetName << " already exists!" << endl;
        return id;
    }

    id = 0;
    forAllConstIters(pointSubsets_, it)
    {
        id = Foam::max(id, it->first + 1);
    }

    pointSubsets_.insert
    (
        std::make_pair
        (
            id,
            meshSubset(subsetName, meshSubset::POINTSUBSET)
        )
    );

    return id;
}


void Foam::Module::polyMeshGenPoints::removePointSubset(const label subsetID)
{
    if (pointSubsets_.find(subsetID) == pointSubsets_.end())
    {
        return;
    }

    pointSubsets_.erase(subsetID);
}


Foam::word Foam::Module::polyMeshGenPoints::pointSubsetName
(
    const label subsetID
) const
{
    std::map<label, meshSubset>::const_iterator it =
        pointSubsets_.find(subsetID);

    if (it == pointSubsets_.end())
    {
        Warning << "Subset " << subsetID << " is not a point subset" << endl;
        return word();
    }

    return it->second.name();
}


Foam::label Foam::Module::polyMeshGenPoints::pointSubsetIndex
(
    const word& subsetName
) const
{
    forAllConstIters(pointSubsets_, it)
    {
        if (it->second.name() == subsetName)
        {
            return it->first;
        }
    }

    return -1;
}


void Foam::Module::polyMeshGenPoints::read()
{
    pointIOField pts
    (
        IOobject
        (
            "points",
            runTime_.constant(),
            "polyMesh",
            runTime_,
            IOobject::MUST_READ
        )
    );
    points_ = pts;

    // read point subsets
    IOobjectList allSets
    (
        runTime_,
        runTime_.constant(),
        "polyMesh/sets"
    );

    wordList setNames = allSets.names("pointSet");
    forAll(setNames, setI)
    {
        IOobject* obj = allSets.lookup(setNames[setI]);

        pointSet pSet(*obj);

        const labelList content = pSet.toc();
        const label id = addPointSubset(setNames[setI]);

        pointSubsets_[id].updateSubset(content);
    }
}


void Foam::Module::polyMeshGenPoints::write() const
{
    points_.write();

    labelLongList containedElements;

    // write point selections
    forAllConstIters(pointSubsets_, setIt)
    {
        pointSet set
        (
            IOobject
            (
                setIt->second.name(),
                runTime_.constant(),
                "polyMesh/sets",
                runTime_,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            )
        );

        setIt->second.containedElements(containedElements);

        forAll(containedElements, i)
        {
            set.insert(containedElements[i]);
        }
        set.write();
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
