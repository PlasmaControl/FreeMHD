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
    releaseAreaMapping

Description
    Initialization of finite area fields,
    used to create release areas for avalanche simulations.

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "faCFD.H"
#include "HormannAgathos.H"
#include "shapefile.H"
#include "gridfile.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "(avalanche)\n"
        "Initialization of finite area fields,"
        " used to create release areas for avalanche simulations."
    );

    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "createFaMesh.H"

    areaVectorField c = aMesh.areaCentres();
    areaVectorField n = aMesh.faceAreaNormals();


    IOdictionary releaseArea
    (
         IOobject
         (
              "releaseArea",
              runTime.constant(),
              mesh,
              IOobject::MUST_READ,
              IOobject::NO_WRITE
         )
    );

    const dictionary fields = releaseArea.subDict("fields");

    wordList fieldNames = fields.toc();

    forAll(fieldNames, fieldsI)
    {
        dictionary fieldsDict = fields.subDict(fieldNames[fieldsI]);

        Info<< "Reading field " << fieldNames[fieldsI] << nl;

        areaScalarField f
        (
            IOobject
            (
                fieldNames[fieldsI],
                runTime.timeName(),
                mesh,
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            aMesh
        );

        dimensionedScalar fdefault = fieldsDict.getOrDefault<dimensionedScalar>("default", dimensionedScalar("default", f.dimensions(), -1e100));

        if (fdefault.value() > -1e99)
        {
            Info<< "Setting field " << fieldNames[fieldsI]
                << " to default value" << nl;
            f.ref() = fdefault;
        }
        string str = fieldNames[fieldsI];
        str[0] = toupper(str[0]);

        Info<< "Creating field " << str << nl;

        volScalarField F
        (
            IOobject
            (
                str,
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            mesh,
            dimensionedScalar("0", f.dimensions(), 0)
        );

        // Create volume-to surface mapping object
        volSurfaceMapping vsm(aMesh);


        Info<< "Reading regions" << nl;

        const PtrList<entry> regions
        (
            fieldsDict.lookup("regions")
        );

        wordList areaNames(regions.size());

        forAll(regions, areaI)
        {
            const entry& regionInfo = regions[areaI];

            Info<< "processing region " << regions[areaI].keyword() << nl;
            if (!regionInfo.isDict())
            {
                FatalIOErrorIn("releaseAreaMapping.C", releaseArea)
                    << "Entry " << regionInfo << " in boundary section is not a"
                    << " valid dictionary." << exit(FatalIOError);
            }
            areaNames[areaI] = regionInfo.keyword();

            dictionary areaDict = regionInfo.dict();

            vector offset;
            List<point2D> points;

            word type = areaDict.get<word>("type");

            if (type == "polygon")
            {
                scalar finit = 0, x0 = 0, y0 = 0, z0 = 0, fx = 0, fy = 0, fz = 0;

                word filltype = areaDict.get<word>("filltype");
                offset = areaDict.getOrDefault<vector>("offset", Zero);

                List<vector> vertices = areaDict.get<List<vector>>("vertices");
                const bool projectNormal =
                    areaDict.getOrDefault("projectToNormal", false);

                Switch filltypeConst = filltype == "constant";
                Switch filltypeLinear = filltype == "linear";

                areaScalarField projection
                (
                    IOobject
                    (
                        "projection",
                        runTime.timeName(),
                        mesh,
                        IOobject::NO_READ,
                        IOobject::NO_WRITE
                    ),
                    aMesh,
                    dimensionedScalar("0", dimless, 1)
                );

                if (projectNormal)
                {
                    projection = n&vector(0, 0, -1);
                }

                points.resize(vertices.size());

                if (filltypeConst)
                {
                    areaDict.readEntry("value", finit);

                    Info<< "constant value with" << nl
                        << "     f = " << finit << nl;
                }
                else if (filltypeLinear)
                {
                    areaDict.readEntry("valueAtZero", finit);
                    x0 = areaDict.getOrDefault<scalar>("x0", 0);
                    y0 = areaDict.getOrDefault<scalar>("y0", 0);
                    z0 = areaDict.getOrDefault<scalar>("z0", 0);
                    fx = areaDict.getOrDefault<scalar>("dfdx", 0);
                    fy = areaDict.getOrDefault<scalar>("dfdy", 0);
                    fz = areaDict.getOrDefault<scalar>("dfdz", 0);

                    Info<< "linear function with" << nl
                        << "     f0 = " << finit << nl
                        << "     x0 = " << x0 << ", dfdx = " << fx << nl
                        << "     y0 = " << y0 << ", dfdy = " << fy << nl
                        << "     z0 = " << z0 << ", dfdz = " << fz << nl;
                }
                else
                {
                    Info << "Unknown filltype \"" << filltype << "\". Skipping entry." << nl;
                    continue;
                }

                forAll(vertices, vI)
                {
                    points[vI] = point2D(vertices[vI].x()+offset.x(), vertices[vI].y()+offset.y());
                }
                HormannAgathos polygon(points, 0.001);

                forAll(c.internalField(), vi)
                {
                    if (polygon.evaluate(point2D(c[vi].x(), c[vi].y())) != HormannAgathos::POINT_OUTSIDE)
                    {
                        f[vi] = (finit + fx*(c[vi].x()-x0) + fy*(c[vi].y()-y0) + fz*(c[vi].z()-z0))*projection[vi];
                    }
                }
            }
            else if (type == "sphere")
            {
                vector center = areaDict.get<vector>("center");
                scalar rad = areaDict.get<scalar>("r");
                scalar scale = areaDict.getOrDefault<scalar>("scale", 1);

                forAll(c.internalField(), i)
                {
                    vector c_cs = c[i]-center;
                    scalar c_csmag = Foam::mag(c_cs);
                    if (c_csmag < rad)
                    {
                        scalar cy = -n[i] & c_cs;
                        scalar cx = Foam::sqrt(Foam::sqr(Foam::mag(c_cs))-Foam::sqr(cy));
                        f[i] = scale*(Foam::sqrt(Foam::sqr(rad)-Foam::sqr(cx))-cy);
                    }
                }
            }
            else if (type == "shapefile")
            {
                shapefile shp;
                fileName filename = areaDict.get<fileName>("filename");
                shp.read(filename);

                if (shp.shptype() != shapefile::POLYGON &&
                    shp.shptype() != shapefile::POLYGONM &&
                    shp.shptype() != shapefile::POLYGONZ &&
                    shp.shptype() != shapefile::POLYLINE &&
                    shp.shptype() != shapefile::POLYLINEM &&
                    shp.shptype() != shapefile::POLYLINEZ &&
                    shp.shptype() != shapefile::MULTIPATCH)
                {
                    Info << "Shapefile is neiter a polygon(MZ), polyline(MZ) nor multipatch. Skipping entry." << nl;
                    continue;
                }


                scalar finit = 0, x0 = 0, y0 = 0, z0 = 0, fx = 0, fy = 0, fz = 0;
                word fieldname;
                label fieldIndex = -1;

                word filltype = areaDict.get<word>("filltype");
                offset = areaDict.getOrDefault<vector>("offset", Zero);

                const bool projectNormal =
                    areaDict.getOrDefault("projectToNormal", false);

                Switch filltypeConst = filltype == "constant";
                Switch filltypeLinear = filltype == "linear";
                Switch filltypeShapefile = filltype == "shapefile";

                areaScalarField projection
                (
                    IOobject
                    (
                        "projection",
                        runTime.timeName(),
                        mesh,
                        IOobject::NO_READ,
                        IOobject::NO_WRITE
                    ),
                    aMesh,
                    dimensionedScalar("0", dimless, 1)
                );

                if (projectNormal)
                {
                    projection = n&vector(0, 0, -1);
                }

                if (filltypeConst)
                {
                    areaDict.readEntry("value", finit);

                    Info<< "constant value with" << nl
                        << "     f = " << finit << nl;
                }
                else if (filltypeLinear)
                {
                    areaDict.readEntry("valueAtZero", finit);
                    x0 = areaDict.getOrDefault<scalar>("x0", 0);
                    y0 = areaDict.getOrDefault<scalar>("y0", 0);
                    z0 = areaDict.getOrDefault<scalar>("z0", 0);
                    fx = areaDict.getOrDefault<scalar>("dfdx", 0);
                    fy = areaDict.getOrDefault<scalar>("dfdy", 0);
                    fz = areaDict.getOrDefault<scalar>("dfdz", 0);

                    Info<< "linear function with" << nl
                        << "     f0 = " << finit << nl
                        << "     x0 = " << x0 << ", dfdx = " << fx << nl
                        << "     y0 = " << y0 << ", dfdy = " << fy << nl
                        << "     z0 = " << z0 << ", dfdz = " << fz << nl;
                }
                else if (filltypeShapefile)
                {
                    areaDict.readEntry("fieldname", fieldname);

                    for (unsigned int i=0; i<shp.fieldcount(); i++)
                    {
                        if (shp.fieldname(i) == fieldname)
                        {
                            if (shp.isnumeric(i))
                            {
                                fieldIndex = i;
                            }
                            break;
                        }
                    }
                    if (fieldIndex > 0)
                    {
                        Info<< "Reading initial values from file/field \"" << filename << "/" << fieldname <<"\"." << nl;
                    }
                    else
                    {
                        Info<< "Cannot find file/field \"" << filename << "/" << fieldname <<"\". Skipping entry." << nl;
                    }
                }
                else
                {
                    Info << "Unknown filltype \"" << filltype << "\". Skipping entry." << nl;
                    continue;
                }

                if (shp.shptype() == shapefile::MULTIPATCH)
                {
                    for (unsigned int i=0; i<shp.recordcount(); i++)
                    {
                        Info << "Mapping record " << i << nl;

                        areaScalarField fold = f;

                        Switch leadingFirstRing = false;

                        for (int j=0; j<shp.partcount(i); j++)
                        {
                            int numpoints = shp.pointcount(i,j);

                            if (shp.parttype(i,j) == shapefile::TRIANGLE_STRIP ||
                                shp.parttype(i,j) == shapefile::TRIANGLE_FAN)
                            {
                                leadingFirstRing = false;

                                points.resize(3);

                                for(int vI=0; vI < numpoints-2; vI++)
                                {
                                    if (shp.parttype(i,j) == shapefile::TRIANGLE_STRIP)
                                    {
                                        points[0] = point2D(shp.x(i, vI+0+shp.partStart(i,j)), shp.y(i, vI+0+shp.partStart(i,j)));
                                        points[1] = point2D(shp.x(i, vI+1+shp.partStart(i,j)), shp.y(i, vI+1+shp.partStart(i,j)));
                                        points[2] = point2D(shp.x(i, vI+2+shp.partStart(i,j)), shp.y(i, vI+2+shp.partStart(i,j)));
                                    }
                                    else //if (shp.pt[i][j] == shapefile::TRIANGLE_FAN)
                                    {
                                        points[0] = point2D(shp.x(i, 0+shp.partStart(i,j)), shp.y(i,0+shp.partStart(i,j)));
                                        points[1] = point2D(shp.x(i, vI+1+shp.partStart(i,j)), shp.y(i, vI+1+shp.partStart(i,j)));
                                        points[2] = point2D(shp.x(i, vI+2+shp.partStart(i,j)), shp.y(i, vI+2+shp.partStart(i,j)));
                                    }
                                    HormannAgathos polygon(points, 0.001);

                                    forAll(c.internalField(), vi)
                                    {
                                        if (polygon.evaluate(point2D(c[vi].x()-offset.x(), c[vi].y()-offset.y())) != HormannAgathos::POINT_OUTSIDE)
                                        {
                                            if (filltypeConst)
                                            {
                                                f[vi] = finit*projection[vi];
                                            }
                                            else if (filltypeLinear)
                                            {
                                                f[vi] = (finit + fx*(c[vi].x()-x0) + fy*(c[vi].y()-y0) + fz*(c[vi].z()-z0))*projection[vi];
                                            }
                                            else if (filltypeShapefile)
                                            {
                                                f[vi] = shp.numericfield(i,fieldIndex)*projection[vi];
                                            }
                                        }
                                    }
                                }
                            }
                            else if (shp.parttype(i,j) == shapefile::OUTER_RING ||
                                     shp.parttype(i,j) == shapefile::INNER_RING ||
                                     shp.parttype(i,j) == shapefile::FIRST_RING ||
                                     shp.parttype(i,j) == shapefile::RING)
                            {
                                if (shp.parttype(i,j) == shapefile::FIRST_RING)
                                {
                                    leadingFirstRing = true;
                                }
                                if (shp.parttype(i,j) == shapefile::INNER_RING || shp.parttype(i,j) == shapefile::OUTER_RING)
                                {
                                    leadingFirstRing = false;
                                }

                                int numpoints = shp.pointcount(i,j);
                                points.resize(numpoints);

                                for(int vI=0; vI < numpoints; vI++)
                                {
                                    points[vI] = point2D(shp.x(i,vI+shp.partStart(i,j)), shp.y(i,vI+shp.partStart(i,j)));
                                }
                                HormannAgathos polygon(points, 0.001);

                                Switch fillPolygon = false;



                                if (shp.parttype(i,j) == shapefile::OUTER_RING ||
                                    shp.parttype(i,j) == shapefile::FIRST_RING)
                                    fillPolygon = true;
                                if (shp.parttype(i,j) == shapefile::RING && leadingFirstRing)
                                    fillPolygon = true;
                                //if (shp.parttype(i,j) == shapefile::RING && !leadingFirstRing)
                                //    fillPolygon = false;
                                //if (shp.parttype(i,j) == shapefile::INNER_RING)
                                //    fillPolygon = false;

                                forAll(c.internalField(), vi)
                                {
                                    if (polygon.evaluate(point2D(c[vi].x()-offset.x(), c[vi].y()-offset.y())) != HormannAgathos::POINT_OUTSIDE)
                                    {
                                        if (!fillPolygon)
                                        {
                                            f[vi] = fold[vi];
                                        }
                                        else if (filltypeConst)
                                        {
                                            f[vi] = finit*projection[vi];
                                        }
                                        else if (filltypeLinear)
                                        {
                                            f[vi] = (finit + fx*(c[vi].x()-x0) + fy*(c[vi].y()-y0) + fz*(c[vi].z()-z0))*projection[vi];
                                        }
                                        else if (filltypeShapefile)
                                        {
                                            f[vi] = shp.numericfield(i,fieldIndex)*projection[vi];
                                        }
                                    }
                                }
                            }
                            else
                            {
                                Info << "Unknown Part Type " << shp.parttype(i,j) << ", skipping part." << nl;
                            }
                        }
                    }
                }
                else
                {
                    for (unsigned int i=0; i<shp.recordcount(); i++)
                    {
                        Info << "Mapping record " << i << nl;

                        areaScalarField fold = f;

                        for (int j=0; j<shp.partcount(i); j++)
                        {
                            int numpoints = shp.pointcount(i,j);

                            points.resize(numpoints);

                            for(int vI=0; vI < numpoints; vI++)
                            {
                                points[vI] = point2D(shp.x(i,vI+shp.partStart(i,j)), shp.y(i,vI+shp.partStart(i,j)));
                            }

                            scalar sumofangles = 0;
                            for(int vI=0; vI < numpoints-2; vI++)
                            {
                                point2D line0 = points[vI+1]-points[vI];
                                point2D line1 = points[vI+2]-points[vI+1];
                                scalar mag0 = std::hypot(line0.x(), line0.y());
                                scalar mag1 = std::hypot(line1.x(), line1.y());
                                scalar crossp = line0.x()*line1.y()-line0.y()*line1.x();
                                scalar angle = std::asin(crossp/mag0/mag1);
                                //Info << line0 << ", " << line1 << ", " << angle*180./3.1415 << endl;
                                sumofangles += angle;
                            }

                            Switch clockwise =sumofangles > 0;

                            //Info << "sum of angles = " << sumofangles << nl;
                            //Info << "part is clockwise " << clockwise << nl;

                            HormannAgathos polygon(points, 0.001);

                            forAll(c.internalField(), vi)
                            {
                                if (polygon.evaluate(point2D(c[vi].x()-offset.x(), c[vi].y()-offset.y())) != HormannAgathos::POINT_OUTSIDE)
                                {
                                    if (clockwise && shp.partcount(i) > 1)
                                    {
                                        f[vi] = fold[vi];
                                    }
                                    else if (filltypeConst)
                                    {
                                        f[vi] = finit*projection[vi];
                                    }
                                    else if (filltypeLinear)
                                    {
                                        f[vi] = (finit + fx*(c[vi].x()-x0) + fy*(c[vi].y()-y0) + fz*(c[vi].z()-z0))*projection[vi];
                                    }
                                    else if (filltypeShapefile)
                                    {
                                        f[vi] = shp.numericfield(i,fieldIndex)*projection[vi];
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if (type == "rasterfile")
            {
                gridfile grid;
                fileName filename = areaDict.get<fileName>("filename");
                grid.read(filename);

                word interpolation = areaDict.get<word>("interpolation");
                offset = areaDict.getOrDefault<vector>("offset", Zero);

                Switch linInterpol = interpolation == "linear";
                Switch nnInterpol = interpolation == "nearestneighbor";


                if (linInterpol)
                {
                    forAll(c.internalField(), vi)
                    {
                        f[vi] = grid.interpolate(c[vi].x(), c[vi].y());
                    }
                }
                else if (nnInterpol)
                {
                    forAll(c.internalField(), vi)
                    {
                        f[vi] = grid.interpolateNN(c[vi].x(), c[vi].y());
                    }
                }
                else
                {
                    Info << "Unknown interpolation type \"" << interpolation << "\". Skipping entry." << nl;
                    continue;
                }
            }
            else
            {
                Info << "Unknown type \"" << type << "\". Skipping entry." << nl;
            }
        }

        Info<< "Writing fields" << nl;

        vsm.mapToVolume(f, F.boundaryFieldRef());

        f.write();
        F.write();
    }

    Info<< nl << "End" << nl;
    return 0;
}


// ************************************************************************* //
