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

#include "gridfileWrite.H"
#include "Time.H"
#include "polyMesh.H"
#include "faMesh.H"
#include "fvMesh.H"
#include "areaFields.H"
#include "ListOps.H"
#include "addToRunTimeSelectionTable.H"
#include "gridfile.H"
#include "faCFD.H"
#include "HormannAgathos.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(gridfileWrite, 0);

    addRemovableToRunTimeSelectionTable
    (
        functionObject,
        gridfileWrite,
        dictionary
    );
}
}

const Foam::Enum
<
    Foam::functionObjects::gridfileWrite::writeOption
>
Foam::functionObjects::gridfileWrite::writeOptionNames_
({
    { writeOption::AUTO_WRITE, "autoWrite" },
    { writeOption::NO_WRITE, "noWrite" },
    { writeOption::ANY_WRITE, "anyWrite" },
});


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

bool Foam::functionObjects::gridfileWrite::findNNByCellNeighbors()
{
    bool foundNewValues = false;

    const areaVectorField &n = aMesh_.faceAreaNormals();
    const areaVectorField &c = aMesh_.areaCentres();


    // We search the nearest neighbors by hoppoing along cell neighbors to speed up the process
    // Bulding indexing of cell-to-cell adressing, taken and addapted from primitiveMesh

    labelList ncc(aMesh_.nFaces(), Zero);

    const labelList& own = aMesh_.edgeOwner();
    const labelList& nei = aMesh_.edgeNeighbour();

    forAll(nei, edgei)
    {
        ncc[own[edgei]]++;
        ncc[nei[edgei]]++;
    }

    labelListList faceFaceAddr(ncc.size());


    forAll(faceFaceAddr, facei)
    {
        faceFaceAddr[facei].setSize(ncc[facei]);
    }
    ncc = 0;

    forAll(nei, edgei)
    {
        label ownFacei = own[edgei];
        label neiFacei = nei[edgei];

        faceFaceAddr[ownFacei][ncc[ownFacei]++] = neiFacei;
        faceFaceAddr[neiFacei][ncc[neiFacei]++] = ownFacei;
    }


    label searchIndex = 0;


    for (label i=0; i<this->ncols_; i++)
    {
        for (label j=0; j<this->nrows_; j++)
        {
            label gridIndex = i*nrows_+j;


            if (nearestNeighbor_[gridIndex] > -1)
                continue;


            scalar x(this->xllcenter_+i*this->dx_);
            scalar y(this->yllcenter_+j*this->dy_);

            searchIndex = -1;

             //start at the cell index of neighboring raster cell, assuming the next is close
            if (i > 0)
            {
                if (nearestNeighbor_[(i-1)*nrows_+j] > -1)
                    searchIndex = nearestNeighbor_[(i-1)*nrows_+j];
            }
            else if (j > 0)
            {
                if (nearestNeighbor_[i*nrows_+j-1] > -1)
                    searchIndex = nearestNeighbor_[i*nrows_+j-1];
            }
            if (searchIndex < 0)
                searchIndex = 0;

            label old_SearchIndex = -1;
            scalar dmin = pow(c[searchIndex].x()-x,2)+
                          pow(c[searchIndex].y()-y,2);

            while(searchIndex != old_SearchIndex)
            {
                old_SearchIndex = searchIndex;
                for(int k=0; k<faceFaceAddr[searchIndex].size(); k++)
                {
                    label cand = faceFaceAddr[searchIndex][k];
                    //Info << "candiate is " << cand << endl;
                    scalar d = pow(c[cand].x()-x,2)+
                               pow(c[cand].y()-y,2);
                    if (d <= dmin)
                    {
                        searchIndex = cand;
                        dmin = d;
                    }
                }
            }


            //check if the point is within the cell

            label checkedSI = checkPointInCellandNeighborsIterativly(searchIndex, faceFaceAddr, x, y, 2);
            if (checkedSI > 0)
            {
                nearestNeighbor_[gridIndex] = checkedSI;
                foundNewValues = true;
                scalar z = 0;
                if (n[searchIndex].z() != 0)
                {
                        z += n[searchIndex].x()/n[searchIndex].z()*(c[searchIndex].x()-x);
                        z += n[searchIndex].y()/n[searchIndex].z()*(c[searchIndex].y()-y);
                }
                pointsOnSurface_[gridIndex] = vector(x-c[searchIndex].x(), y-c[searchIndex].y(), z);
            }
        }
    }

    return foundNewValues;
}


Foam::label
Foam::functionObjects::gridfileWrite::checkPointInCellandNeighborsIterativly
(
    label sI,
    const labelListList& faceFaceAddr,
    scalar x, scalar y, label depth
) const
{
    const pointField& pp = aMesh_.points();

    List<point2D> points;
    points.resize(aMesh_.faces()[sI].size());

    for (int vI = 0; vI<aMesh_.faces()[sI].size(); vI++)
    {
        points[vI] = point2D(pp[aMesh_.faces()[sI][vI]].x(), pp[aMesh_.faces()[sI][vI]].y());
    }

    HormannAgathos polygon(points, 0.001);

    if (polygon.evaluate(point2D(x, y)) != HormannAgathos::POINT_OUTSIDE)
    {
        return sI;
    }

    if (depth > 0)
    {
        for (int k=0; k<faceFaceAddr[sI].size(); k++)
        {
            label sI2 = faceFaceAddr[sI][k];

            sI2 = checkPointInCellandNeighborsIterativly(sI2, faceFaceAddr, x, y, depth-1);

            if (sI2 > 0)
            {
                return sI2;
            }
        }
    }

    return -1;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::gridfileWrite::gridfileWrite
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
    secondOrderInterpolations_(false),
    xmin_(0),
    ymin_(0),
    xmax_(0),
    ymax_(0),
    dx_(0),
    dy_(0),
    ncols_(0),
    nrows_(0),
    NODATA_value_(-9999),
    postfix_(".asc")
{
    read(dict);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::functionObjects::gridfileWrite::writeField(const areaScalarField& f, const fileName &fn) const
{
    const areaVectorField gradf(fac::grad(f));

    gridfile raster(xllcenter_-offset_.x(), yllcenter_-offset_.y(), dx_, dy_, ncols_, nrows_);

    for (label i=0; i<this->ncols_; i++)
    {
        for (label j=0; j<this->nrows_; j++)
        {
            if (nearestNeighbor_[i*nrows_+j] > -1)
            {
                raster.vRef()[nrows_-j-1][i] = f[nearestNeighbor_[i*nrows_+j]];
                if (secondOrderInterpolations_)
                {
                    raster.vRef()[nrows_-j-1][i] += gradf[nearestNeighbor_[i*nrows_+j]]&(pointsOnSurface_[i*nrows_+j]);
                }
            }
        }
    }

    raster.write(fn);
}


bool Foam::functionObjects::gridfileWrite::read(const dictionary& dict)
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

    dict.readEntry("secondOrder", secondOrderInterpolations_);

    dict.readIfPresent("postfix", postfix_);

    offset_ = dict.getOrDefault<vector>("offset", Zero);

    dict.readEntry("dx", dx_);
    dict.readEntry("dy", dy_);


    if
    (
        dict.readIfPresent("ncols", ncols_)
     && dict.readIfPresent("nrows", nrows_)
     && (dict.found("xllcenter") || dict.found("xllcorner"))
     && (dict.found("yllcenter") || dict.found("yllcorner"))
    )
    {
        Info << "Found gridfile settings (ncols,nrows,xllcenter/xllcorner,yllcenter/yllcorner)." << endl;

        if (!dict.readIfPresent("xllcenter", xllcenter_))
        {
            dict.readEntry("xllcorner", xllcenter_);
            xllcenter_ -= dx_/2.;
        }
        if (!dict.readIfPresent("yllcenter", yllcenter_))
        {
            dict.readEntry("yllcorner", yllcenter_);
            yllcenter_ -= dy_/2.;
        }
        xmin_ = xllcenter_-dx_/2.;
        ymin_ = yllcenter_-dy_/2.;
        xmax_ = xmin_+dx_*ncols_;
        ymax_ = ymin_+dy_*nrows_;
    }
    else
    {
        Info << "Didn't find gridfile settings." << endl
             << "Searching/Calculating map extends (xmin,ymin,xmax,ymax) to generate gridfile settings." << endl;

        //by bounding box and resolution
        if (!dict.readIfPresent("xmin", xmin_))
        {
            xmin_ = min(aMesh_.points() & vector(1,0,0));
        }
        if (!dict.readIfPresent("xmax", xmax_))
        {
            xmax_ = max(aMesh_.points() & vector(1,0,0));
        }
        if (!dict.readIfPresent("ymin", ymin_))
        {
            ymin_ = min(aMesh_.points() & vector(0,1,0));
        }
        if (!dict.readIfPresent("ymax", ymax_))
        {
            ymax_ = max(aMesh_.points() & vector(0,1,0));
        }

        ncols_ = ceil((xmax_-xmin_)/dx_);
        nrows_ = ceil((ymax_-ymin_)/dy_);
        xllcenter_ = xmin_+dx_/2.;
        yllcenter_ = ymin_+dy_/2.;
    }

    expire();

    return true;
}


bool Foam::functionObjects::gridfileWrite::execute()
{
    return true;
}


bool Foam::functionObjects::gridfileWrite::write()
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
            fileName filename(fileName::concat(obr_.time().timePath(), obj.name()+postfix_));

            this->writeField(f, filename);
        }
        else if (obj.type() == "areaVectorField")
        {
            const areaVectorField &v = obr_.lookupObjectRef<areaVectorField>(obj.name());

            fileName fn(fileName::concat(obr_.time().timePath(), obj.name()+"_mag"+postfix_));
            this->writeField(mag(v), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_x"+postfix_));
            this->writeField(v.component(vector::X), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_y"+postfix_));
            this->writeField(v.component(vector::Y), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_z"+postfix_));
            this->writeField(v.component(vector::Z), fn);
        }
        else if (obj.type() == "areaSphericalTensorField")
        {
            const areaSphericalTensorField &v = obr_.lookupObjectRef<areaSphericalTensorField>(obj.name());

            fileName fn(fileName::concat(obr_.time().timePath(), obj.name()+"_mag"+postfix_));
            this->writeField(mag(v), fn);
        }
        else if (obj.type() == "areaSymmTensorField")
        {
            const areaSymmTensorField &v = obr_.lookupObjectRef<areaSymmTensorField>(obj.name());

            fileName fn(fileName::concat(obr_.time().timePath(), obj.name()+"_mag"+postfix_));
            this->writeField(mag(v), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_xx"+postfix_));
            this->writeField(v.component(tensor::XX), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_xy"+postfix_));
            this->writeField(v.component(tensor::XY), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_xz"+postfix_));
            this->writeField(v.component(tensor::XZ), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_yy"+postfix_));
            this->writeField(v.component(tensor::YY), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_yz"+postfix_));
            this->writeField(v.component(tensor::YZ), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_zz"+postfix_));
            this->writeField(v.component(tensor::ZZ), fn);
        }
        else if (obj.type() == "areaTensorField")
        {
            const areaTensorField &v = obr_.lookupObjectRef<areaTensorField>(obj.name());

            fileName fn(fileName::concat(obr_.time().timePath(), obj.name()+"_mag"+postfix_));
            this->writeField(mag(v), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_xx"+postfix_));
            this->writeField(v.component(tensor::XX), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_xy"+postfix_));
            this->writeField(v.component(tensor::XY), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_xz"+postfix_));
            this->writeField(v.component(tensor::XZ), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_yx"+postfix_));
            this->writeField(v.component(tensor::YX), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_yy"+postfix_));
            this->writeField(v.component(tensor::YY), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_yz"+postfix_));
            this->writeField(v.component(tensor::YZ), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_zx"+postfix_));
            this->writeField(v.component(tensor::ZX), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_zy"+postfix_));
            this->writeField(v.component(tensor::ZY), fn);

            fn = fileName(fileName::concat(obr_.time().timePath(), obj.name()+"_zz"+postfix_));
            this->writeField(v.component(tensor::ZZ), fn);
        }
        else
        {
            continue;
        }
    }

    Info << endl;

    return true;
}


void Foam::functionObjects::gridfileWrite::updateMesh(const mapPolyMesh& mpm)
{
    expire();
}

void Foam::functionObjects::gridfileWrite::movePoints(const polyMesh& mesh)
{
    expire();
}

void Foam::functionObjects::gridfileWrite::readUpdate(const polyMesh::readUpdateState state)
{
    expire();
}

void Foam::functionObjects::gridfileWrite::expire()
{
    Info << "Initializing interpolation " << endl;

    nearestNeighbor_.resize(ncols_*nrows_, -1);
    pointsOnSurface_.resize(ncols_*nrows_, Zero);

    for (label i=0; i<this->ncols_; i++)
    {
        for (label j=0; j<this->nrows_; j++)
        {
            label gridIndex = i*nrows_+j;

            nearestNeighbor_[gridIndex] = -1;
        }
    }

    const areaVectorField &c = aMesh_.areaCentres();
    const pointField &pp = aMesh_.points();
    const areaVectorField &n = aMesh_.faceAreaNormals();


    for(int sI=0; sI<c.size(); sI++)
    {
        label i = round((c[sI].x()-xllcenter_)/dx_);
        label j = round((c[sI].y()-yllcenter_)/dy_);

        if (i < ncols_ && i > 0 && j < nrows_ && j > 0)
        {
            label gridIndex = i*nrows_+j;

            scalar x(this->xllcenter_+i*this->dx_);
            scalar y(this->yllcenter_+j*this->dy_);

            List<point2D> points;

            points.resize(aMesh_.faces()[sI].size());

            for(int vI = 0; vI<aMesh_.faces()[sI].size(); vI++)
            {
                points[vI] = point2D(pp[aMesh_.faces()[sI][vI]].x(), pp[aMesh_.faces()[sI][vI]].y());
            }

            HormannAgathos polygon(points, 0.001);

            if (polygon.evaluate(point2D(x, y)) != HormannAgathos::POINT_OUTSIDE)
            {
                nearestNeighbor_[gridIndex] = sI;
                scalar z = 0;
                if (n[sI].z() != 0)
                {
                        z += n[sI].x()/n[sI].z()*(c[sI].x()-x);
                        z += n[sI].y()/n[sI].z()*(c[sI].y()-y);
                }
                pointsOnSurface_[gridIndex] = vector(x-c[sI].x(), y-c[sI].y(), z);
            }
        }
    }


    findNNByCellNeighbors();


    Info << "Done." << nl << endl;

}


// ************************************************************************* //
