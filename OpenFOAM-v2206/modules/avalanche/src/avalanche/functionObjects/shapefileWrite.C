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

#include "shapefileWrite.H"
#include "Time.H"
#include "polyMesh.H"
#include "faMesh.H"
#include "areaFields.H"
#include "ListOps.H"
#include "addToRunTimeSelectionTable.H"
#include "shapefile.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(shapefileWrite, 0);

    addRemovableToRunTimeSelectionTable
    (
        functionObject,
        shapefileWrite,
        dictionary
    );
}
}

const Foam::Enum
<
    Foam::functionObjects::shapefileWrite::writeOption
>
Foam::functionObjects::shapefileWrite::writeOptionNames_
({
    { writeOption::AUTO_WRITE, "autoWrite" },
    { writeOption::NO_WRITE, "noWrite" },
    { writeOption::ANY_WRITE, "anyWrite" },
});


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::shapefileWrite::shapefileWrite
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
    writeOption_(ANY_WRITE),
    objectNames_(),
    prefix_("polys")
{
    read(dict);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

shapefile& Foam::functionObjects::shapefileWrite::generateShapeFileGeometry(const faMesh &mesh)
{
    shp_ = shapefile(shapefile::POLYGONZ);

    for (const face& f : mesh.faces())
    {
        int rI = shp_.addRecord();
        shp_.addPart(rI);
        for(const label& l : f)
        {
            shp_.addPoint(rI, mesh.points()[l].x()-offset_.x(),
                              mesh.points()[l].y()-offset_.y(),
                              mesh.points()[l].z()-offset_.z());
        }
        shp_.addPoint(rI, mesh.points()[f[0]].x()-offset_.x(),
                          mesh.points()[f[0]].y()-offset_.y(),
                          mesh.points()[f[0]].z()-offset_.z());
    }

    shp_.calcBoundingBox();

    return shp_;
}

shapefile& Foam::functionObjects::shapefileWrite::addToShapeFile(const areaScalarField &f, shapefile &shp)
{

    int index = shp.addField(f.name(), 'F', 12, 6);
    int i = 0;
    for (const scalar &val : f)
    {
        shp.setField(i, index, val);
        i++;
    }

    return shp;

}

shapefile& Foam::functionObjects::shapefileWrite::addToShapeFile(const areaVectorField &f, shapefile &shp)
{

    int index = shp.addField(f.name()+"_mag", 'F', 12, 6);
    shp.addField(f.name()+"_x", 'F', 12, 6);
    shp.addField(f.name()+"_y", 'F', 12, 6);
    shp.addField(f.name()+"_z", 'F', 12, 6);

    int i = 0;
    for (const vector &val : f)
    {
        shp_.setField(i, index, mag(val));
        shp_.setField(i, index+1, val.x());
        shp_.setField(i, index+2, val.y());
        shp_.setField(i, index+3, val.z());
        i++;
    }

    return shp;
}

shapefile& Foam::functionObjects::shapefileWrite::addToShapeFile(const areaSphericalTensorField &f, shapefile &shp)
{
    int index = shp.addField(f.name()+"_mag", 'F', 12, 6);

    int i = 0;
    for (const sphericalTensor &val : f)
    {
        shp_.setField(i, index, mag(val));
        i++;
    }

return shp;
}

shapefile& Foam::functionObjects::shapefileWrite::addToShapeFile(const areaSymmTensorField &f, shapefile &shp)
{
    int index = shp.addField(f.name()+"_mag", 'F', 12, 6);
    shp.addField(f.name()+"_xx", 'F', 12, 6);
    shp.addField(f.name()+"_xy", 'F', 12, 6);
    shp.addField(f.name()+"_xz", 'F', 12, 6);
    shp.addField(f.name()+"_yy", 'F', 12, 6);
    shp.addField(f.name()+"_yz", 'F', 12, 6);
    shp.addField(f.name()+"_zz", 'F', 12, 6);

    int i = 0;
    for (const symmTensor &val : f)
    {
        shp_.setField(i, index, mag(val));
        shp_.setField(i, index+1, val.xx());
        shp_.setField(i, index+2, val.xy());
        shp_.setField(i, index+3, val.xz());
        shp_.setField(i, index+4, val.yy());
        shp_.setField(i, index+5, val.yz());
        shp_.setField(i, index+6, val.zz());
        i++;
    }

    return shp;
}

shapefile& Foam::functionObjects::shapefileWrite::addToShapeFile(const areaTensorField &f, shapefile &shp)
{
    int index = shp.addField(f.name()+"_mag", 'F', 12, 6);
    shp.addField(f.name()+"_xx", 'F', 12, 6);
    shp.addField(f.name()+"_xy", 'F', 12, 6);
    shp.addField(f.name()+"_xz", 'F', 12, 6);
    shp.addField(f.name()+"_xy", 'F', 12, 6);
    shp.addField(f.name()+"_yy", 'F', 12, 6);
    shp.addField(f.name()+"_yz", 'F', 12, 6);
    shp.addField(f.name()+"_xz", 'F', 12, 6);
    shp.addField(f.name()+"_yz", 'F', 12, 6);
    shp.addField(f.name()+"_zz", 'F', 12, 6);

    int i = 0;
    for (const tensor &val : f)
    {
        shp_.setField(i, index, mag(val));
        shp_.setField(i, index+1, val.xx());
        shp_.setField(i, index+2, val.xy());
        shp_.setField(i, index+3, val.xz());
        shp_.setField(i, index+4, val.yx());
        shp_.setField(i, index+4, val.yy());
        shp_.setField(i, index+5, val.yz());
        shp_.setField(i, index+6, val.zx());
        shp_.setField(i, index+7, val.zy());
        shp_.setField(i, index+8, val.zz());
        i++;
    }

    return shp;
}

bool Foam::functionObjects::shapefileWrite::writeShapeFile(const shapefile &shp, const fileName &filename)
{
    return shp_.write(filename);
}

bool Foam::functionObjects::shapefileWrite::read(const dictionary& dict)
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

    offset_ = dict.getOrDefault<vector>("offset", Zero);

    writeOption_ = writeOptionNames_.getOrDefault
    (
        "writeOption",
        dict,
        writeOption::ANY_WRITE
    );

    dict.readIfPresent("prefix", prefix_);

    return true;
}


bool Foam::functionObjects::shapefileWrite::execute()
{
    return true;
}


bool Foam::functionObjects::shapefileWrite::write()
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

    if (!selectedNames.empty())
    {
        generateShapeFileGeometry(this->aMesh_);
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
            addToShapeFile(f, shp_);
        }
        else if(obj.type() == "areaVectorField")
        {
            const areaVectorField &f = obr_.lookupObjectRef<areaVectorField>(obj.name());
            addToShapeFile(f, shp_);
        }
        else if(obj.type() == "areaSphericalTensorField")
        {
            const areaSphericalTensorField &f = obr_.lookupObjectRef<areaSphericalTensorField>(obj.name());
            addToShapeFile(f, shp_);
        }
        else if(obj.type() == "areaSymmTensorField")
        {
            const areaSymmTensorField &f = obr_.lookupObjectRef<areaSymmTensorField>(obj.name());
            addToShapeFile(f, shp_);
        }
        else if(obj.type() == "areaTensorField")
        {
            const areaTensorField &f = obr_.lookupObjectRef<areaTensorField>(obj.name());
            addToShapeFile(f, shp_);
        }
    }

    fileName filename(fileName::concat(obr_.time().timePath(), prefix_+obr_.time().timeName()));

    Info << "Writing to " << filename << nl << nl;

    writeShapeFile(shp_, filename);

    return true;
}


// ************************************************************************* //
