/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | avalanche module
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2020 Matthias Rauter
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

\*---------------------------------------------------------------------------*/

#include "autoAreaToVolumeMapping.H"
#include "Time.H"
#include "faCFD.H"
#include "fvCFD.H"
#include "polyMesh.H"
#include "faMesh.H"
#include "areaFields.H"
#include "ListOps.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(autoAreaToVolumeMapping, 0);

    addRemovableToRunTimeSelectionTable
    (
        functionObject,
        autoAreaToVolumeMapping,
        dictionary
    );
}
}

const Foam::Enum
<
    Foam::functionObjects::autoAreaToVolumeMapping::writeOption
>
Foam::functionObjects::autoAreaToVolumeMapping::writeOptionNames_
({
    { writeOption::AUTO_WRITE, "autoWrite" },
    { writeOption::NO_WRITE, "noWrite" },
    { writeOption::ANY_WRITE, "anyWrite" },
});


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::autoAreaToVolumeMapping::autoAreaToVolumeMapping
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    regionFunctionObject(name, runTime, dict),
    aMesh_
    (
        obr_.lookupObject<faMesh>("faMesh")
    ),
    mesh_
    (
        obr_.lookupObject<fvMesh>("data")
    ),
    writeOption_(ANY_WRITE),
    objectNames_(),
    prefix_("fa_"),
    vsm_(aMesh_)
{
    read(dict);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::autoAreaToVolumeMapping::read(const dictionary& dict)
{
    regionFunctionObject::read(dict);

    if (dict.found("field"))
    {
        objectNames_.resize(1);
        dict.readEntry("field", objectNames_.first());
    }
    else if (dict.found("fields"))
    {
        dict.readEntry("fields", objectNames_);
    }
    else
    {
        dict.readEntry("objects", objectNames_);
    }

    writeOption_ = writeOptionNames_.getOrDefault
    (
        "writeOption",
        dict,
        writeOption::ANY_WRITE
    );

    dict.readIfPresent("prefix", prefix_);

    return true;
}


bool Foam::functionObjects::autoAreaToVolumeMapping::execute()
{
    return true;
}


bool Foam::functionObjects::autoAreaToVolumeMapping::write()
{
    Info << type() << " " << name() << " write:" << nl;

    if (!obr_.time().writeTime())
    {
        return false;
    }

    // Get selection
    const wordList selectedNames(obr_.sortedNames<regIOobject>(objectNames_));

    // Warning if anything was missed
    bitSet missed(objectNames_.size());

    label index = 0;
    for (const wordRe& select : objectNames_)
    {
        if (!ListOps::found(selectedNames, select))
        {
            missed.set(index);
        }
        ++index;
    }

    if (missed.any())
    {
        WarningInFunction
            << "No corresponding selection for "
            << flatOutput(subset(missed, objectNames_)) << nl
            << "Available objects in database:"
            << nl << obr_.sortedToc()
            << endl;
    }

    for (const word& objName : selectedNames)
    {
        regIOobject& obj = obr_.lookupObjectRef<regIOobject>(objName);

        if (obj.type() != "areaScalarField" &&
            obj.type() != "areaVectorField" &&
            obj.type() != "areaSphericalTensorField" &&
            obj.type() != "areaSymmTensorField" &&
            obj.type() != "areaTensorField")
        {
            continue;
        }

        switch (writeOption_)
        {
            case AUTO_WRITE:
            {
                if (obj.writeOpt() != IOobject::AUTO_WRITE)
                {
                    continue;
                }

                break;
            }
            case NO_WRITE:
            {
                if (obj.writeOpt() != IOobject::NO_WRITE)
                {
                    continue;
                }

                break;
            }
            case ANY_WRITE:
            {
                break;
            }
            default:
            {
                FatalErrorInFunction
                    << "Unknown writeOption "
                    << writeOptionNames_[writeOption_]
                    << ". Valid writeOption types are "
                    << writeOptionNames_
                    << exit(FatalError);

                continue;
                break;
            }
        }
        if (obj.name().ends_with("_0"))
        {
            Info << "    ignoring old " << obj.type() <<" " << obj.name() << endl;
            continue;
        }
        else if
        (
            obj.writeOpt() == IOobject::AUTO_WRITE
         && obr_.time().writeTime()
        )
        {
            Info << "    automatically writing " << obj.type() <<" " << obj.name() << endl;
        }
        else
        {
            Info << "    writing " << obj.type() <<" " << obj.name() << endl;
        }

        if (obj.type() == "areaScalarField")
        {
            const areaScalarField &f = obr_.lookupObjectRef<areaScalarField>(obj.name());
            volScalarField F
            (
                IOobject
                (
                    prefix_+f.name(),
                    obr_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh_,
                dimensionedScalar(f.dimensions())
            );

            vsm_.mapToVolume(f, F.boundaryFieldRef());
            F.write();
        }
        else if(obj.type() == "areaVectorField")
        {
            const areaVectorField &f = obr_.lookupObjectRef<areaVectorField>(obj.name());
            volVectorField F
            (
                IOobject
                (
                    prefix_+f.name(),
                    obr_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh_,
                dimensionedVector(f.dimensions())
            );

            vsm_.mapToVolume(f, F.boundaryFieldRef());
            F.write();
        }
        else if(obj.type() == "areaSphericalTensorField")
        {
            const areaSphericalTensorField &f = obr_.lookupObjectRef<areaSphericalTensorField>(obj.name());
            volSphericalTensorField F
            (
                IOobject
                (
                    prefix_+f.name(),
                    obr_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh_,
                dimensionedSphericalTensor(f.dimensions())
            );

            vsm_.mapToVolume(f, F.boundaryFieldRef());
            F.write();
        }
        else if(obj.type() == "areaSymmTensorField")
        {
            const areaSymmTensorField &f = obr_.lookupObjectRef<areaSymmTensorField>(obj.name());
            volSymmTensorField F
            (
                IOobject
                (
                    prefix_+f.name(),
                    obr_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh_,
                dimensionedSymmTensor(f.dimensions())
            );

            vsm_.mapToVolume(f, F.boundaryFieldRef());
            F.write();
        }
        else if(obj.type() == "areaTensorField")
        {
            const areaTensorField &f = obr_.lookupObjectRef<areaTensorField>(obj.name());
            volTensorField F
            (
                IOobject
                (
                    prefix_+f.name(),
                    obr_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh_,
                dimensionedTensor(f.dimensions())
            );

            vsm_.mapToVolume(f, F.boundaryFieldRef());
            F.write();
        }
    }

    Info << endl;

    return true;
}


// ************************************************************************* //
