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

Application
    gridToSTL

Description
    Generate an STL file out of geodata

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "faCFD.H"
#include "HormannAgathos.H"
#include "shapefile.H"
#include "gridfile.H"
#include "triangle.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

typedef triangle<point2D, point2D> triangle2D;
typedef triangle<point, point> triangle3D;
typedef line<point2D, point2D> line2D;
typedef List<point2D> polygon2D;

triangle3D triangle2Dto3D(const triangle2D &t, const scalar &za, const scalar &zb, const scalar &zc)
{
    return triangle3D(point(t.a().x(), t.a().y(), za), point(t.b().x(), t.b().y(), zb), point(t.c().x(), t.c().y(), zc));
}


class triIndex
{
public:
    triIndex(label a_=0, label b_=1, label c_=2):a(a_),b(b_),c(c_){}
    label a, b, c;
};

class bpoly
{
private:
    polygon2D points_;

    point2D offset_;

    List<triIndex> triangles_;

    List<triIndex> isBoundary_;

public:
    bpoly(const polygon2D &points,
          const point2D &offset)
        :
          points_(points),
          offset_(offset)
    {

    }

    label ip(label i) const { return (i+1)<points_.size()?i+1:0;}

    label id(label i) const { return (i-1)>=0?i-1:points_.size()-1;}

    bool crossing(const line2D &l1, const line2D &l2) const
    {
        const point2D &x1 = l1.first();
        const point2D &x2 = l2.first();
        point2D m1 = l1.second()-x1;
        point2D m2 = l2.second()-x2;

        //Info << "crossing (" << x1 << "--" << l1.second() << ") x (" << x2 << "--" << l2.second() << ")";

        //parallel case
        if (mag(m1.y()*m2.x()-m1.x()*m2.y()) < 0.001 || mag(m1.y()*m2.x()+m1.x()*m2.y()) < 0.001)
        {
            //Info << " -> parallel" << nl;
            return false;
        }

        scalar t1 = (m1.y()*x1.x() + m1.x()*x2.y()-m1.y()*x2.x()-m1.x()*x1.y())/(m1.y()*m2.x()-m1.x()*m2.y());
        scalar t2 = (m2.y()*x2.x() + m2.x()*x1.y()-m2.y()*x1.x()-m2.x()*x2.y())/(m2.y()*m1.x()-m2.x()*m1.y());


        if (t1 > 0 and t1 < 1 and t2 > 0 and t2 < 1)
        {
            //Info << " -> crossing" << nl;
            return true;
        }
        else
        {
            //Info << "-> NOT crossing" << nl;
            return false;
        }
    }

    float sign(point2D p1, point2D p2, point2D p3) const
    {
        return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y());
    }

    bool pointInTriangle(const triangle2D &t, const point2D &pt) const
    {

        float d1, d2, d3;
        bool has_neg, has_pos;

        point2D v1 = t.a(), v2 = t.b(), v3 = t.c();

        d1 = sign(pt, v1, v2);
        d2 = sign(pt, v2, v3);
        d3 = sign(pt, v3, v1);

        has_neg = ((d1 < 0.1) || (d2 < 0.1) || (d3 < 0.1));
        has_pos = ((d1 > -0.1) || (d2 > -0.1) || (d3 > -0.1));

        return !(has_neg && has_pos);

    }

    scalar angle(const point2D &line0, const point2D &line1) const
    {
        scalar mag0 = std::hypot(line0.x(), line0.y());
        scalar mag1 = std::hypot(line1.x(), line1.y());
        scalar crossp = line0.x()*line1.y()-line0.y()*line1.x();
        return std::asin(crossp/mag0/mag1);
    }

    scalar quality(const triangle2D &t) const
    {
        point2D line0 = t.b() -t.a();
        point2D line1 = t.c() -t.a();

        point2D line2 = t.b() -t.c();
        point2D line3 = t.a() -t.c();

        point2D line4 = t.c() -t.b();
        point2D line5 = t.a() -t.b();

        scalar a1 = mag(angle(line0, line1));
        scalar a2 = mag(angle(line2, line3));
        scalar a3 = mag(angle(line4, line5));

        if (a1 > M_PI) a1 = 2.*M_PI-a1;
        if (a2 > M_PI) a2 = 2.*M_PI-a2;
        if (a3 > M_PI) a3 = 2.*M_PI-a3;

        return min(a1, min(a2, a3));
    }

    bool checkValidity(const line2D &l) const
    {
        forAll(this->points_, i)
        {
            line2D o = line2D(this->points_[i], this->points_[ip(i)]);
            if(mag(o.start() - l.start()) < 0.001 ||
               mag(o.start() - l.end()) < 0.001 ||
               mag(o.end() - l.start()) < 0.001 ||
               mag(o.end() - l.end()) < 0.001)
            {
                //Info << "lines have a common point" << endl;)
                return true;
            }
            //Info << "checking line " << i << ": (" << i << "-" << ip(i) << ") : "<< o << " x " << l;
            if (crossing(o, l))
            {
                //Info << "crossing" << nl;
                return false;
            }
            else
            {
                //Info << "ok" << nl;
            }
        }
        return true;
    }

    bool checkValidity(const triangle2D &t) const
    {
        bool notcrossing = checkValidity(line2D(t.a(), t.b())) &&
                           checkValidity(line2D(t.b(), t.c())) &&
                           checkValidity(line2D(t.c(), t.a()));

        bool pointInside = false;


        forAll(this->points_, i)
        {
            pointInside = pointInside || pointInTriangle(t, points_[i]);
        }


        return notcrossing && !(pointInside);
    }

    void triangulate()
    {
        label ei1=-1, ei2=-1;

        triIndex t0, t1, t2;
        //find a start triangle at a konkave position
        label i = 0;


        HormannAgathos polygon(this->points_, 0.1);

        while(i < this->points_.size())
        {

            ei1 = id(i);
            ei2 = ip(i);
            t0 = triIndex(ei1, i, ei2);
            triangle2D t(points_[t0.a], points_[t0.b], points_[t0.c]);

            //Info << "************ check " << i << " *******" << nl;

            if (checkValidity(t) && polygon.evaluate(t.centre()) != HormannAgathos::POINT_OUTSIDE)
            {

                this->triangles_.append(t0);

                break;
            }
            i++;
        }

        if (this->triangles_.size() < 1)
        {
            Info << "Failed to find starting triangle " << nl;
            return;
        }
        //Info << "Found starting triangle " << ei1 << ", " << i << ", " << ei2 << nl;


        bool finished = false;

        do
        {
            bool validFound = false;
            label skippoints = 0;


            List<triIndex> skipped1;
            List<triIndex> skipped2;

            bool path1valid = true;
            bool path2valid = true;

            //Info << nl << "Searching for a new triangle next to existing triangle" << nl;

            while (!validFound)
            {
                skippoints ++;

                //Info << "Iteration " << skippoints << nl;


                label ei31 = ei1 - skippoints;
                label ei32 = ei2 + skippoints;

                if (skippoints > 1)
                {
                    //Info << "putting triangle " << ei31+1 << ", " << ei31+2 << ", " << ei31 << " into buffer 1" << nl;
                    triIndex t1(ei31+1, ei31+2, ei31);
                    skipped1.append(t1);
                    triangle2D tri1(points_[t1.a], points_[t1.b], points_[t1.c]);
                    path1valid = path1valid && checkValidity(tri1);

                    //Info << "putting triangle " << ei32-1 << ", " << ei32-2 << ", " << ei32 << " into buffer 2" << nl;
                    triIndex t2(ei32-1, ei32-2, ei32);
                    skipped2.append(t2);
                    triangle2D tri2(points_[t2.a], points_[t2.b], points_[t2.c]);
                    path2valid = path2valid && checkValidity(tri2);
                }

                while (ei31 < 0)
                    ei31 += this->points_.size();
                while (ei31 >= this->points_.size())
                    ei31 -= this->points_.size();

                t1 = triIndex(ei1, ei2, ei31);
                t2 = triIndex(ei1, ei2, ei32);

                triangle2D tri1(points_[t1.a], points_[t1.b], points_[t1.c]);
                triangle2D tri2(points_[t2.a], points_[t2.b], points_[t2.c]);


                if (id(ei1) != ip(ei2))
                {
                    //Info << "Candiate 1 = " << ei1 << ", " << ei2 << ", " << ei31 << nl;
                    bool c1 = checkValidity(tri1) && path1valid;

                    //Info << "Candiate 2 = " << ei1 << ", " << ei2 << ", " << ei32 << nl;
                    bool c2 = checkValidity(tri2) && path2valid;

                    if (c1 && c2)
                    {
                        scalar q1 = quality(tri1);
                        scalar q2 = quality(tri2);
                        //Info << "Checking triangle quality Candidate 1 = " << q1 << ", Candidate 2 = " << q2 << nl;

                        if (q1 > q2)
                        {
                            c2 = false;
                        }
                        else
                        {
                            c1 = false;
                        }
                    }

                    if (c1)
                    {
                        this->triangles_.append(t1);
                        forAll(skipped1, si1)
                        {
                            this->triangles_.append(skipped1[si1]);
                        }
                        ei1 = ei31;


                        validFound = true;
                        //Info << "Picked Candidate 1 = " << ei1 << ", " << ei2 << ", " << ei31 << nl;
                    }
                    else if (c2)
                    {
                        this->triangles_.append(t2);
                        forAll(skipped2, si2)
                        {
                            this->triangles_.append(skipped2[si2]);
                        }
                        ei2 = ei32;

                        validFound = true;
                        //Info << "Picked Candidate 2 = " << ei1 << ", " << ei2 << ", " << ei32 << nl;
                    }
                    else
                    {
                        //Info << "WARNING: Triangulation: Both triangles invalid." << nl;
                    }
                }
                else
                {
                    if (path1valid)
                    {
                        //Info << "last triangle " << ei1 << ", " << ei2 << ", " << ei31 << nl;

                        this->triangles_.append(t1);
                        forAll(skipped1, si1)
                        {
                            this->triangles_.append(skipped1[si1]);
                        }
                    }
                    else
                    {
                        //Info << "last triangle " << ei1 << ", " << ei2 << ", " << ei32 << nl;

                        this->triangles_.append(t2);
                        forAll(skipped2, si2)
                        {
                            this->triangles_.append(skipped2[si2]);
                        }
                    }
                    finished = true;
                    validFound = true;
                }
            }
        }while(!finished);

        forAll(triangles_, i)
        {
            this->isBoundary_.append(triIndex(
                                        ip(this->triangles_[i].a) == this->triangles_[i].b || id(this->triangles_[i].a) == this->triangles_[i].b,
                                        ip(this->triangles_[i].b) == this->triangles_[i].c || id(this->triangles_[i].b) == this->triangles_[i].c,
                                        ip(this->triangles_[i].c) == this->triangles_[i].a || id(this->triangles_[i].c) == this->triangles_[i].a));
        }

        Info << "Found triangulation out of " << this->triangles_.size() << " triangles." << nl;
        //forAll(triangles_, i)
        //{
        //    Info << "(" << this->triangles_[i].a <<", " << this->triangles_[i].b << ", " << this->triangles_[i].c << ")"
        //         << "(" << this->isBoundary_[i].a <<", " << this->isBoundary_[i].b << ", " << this->isBoundary_[i].c << ")" << nl;
        //}
    }

    void addTriangle2D(std::ofstream &output, const triangle2D &t) const
    {
        addTriangle3D(output, t, 0, 0, 0);
    }

    void addTriangle3D(std::ofstream &output, const triangle2D &t, const gridfile &terrain) const
    {
        addTriangle3D(output, t, terrain.interpolate(t.a().x(), t.a().y()), terrain.interpolate(t.b().x(), t.b().y()) , terrain.interpolate(t.c().x(), t.c().y()));
    }

    void addTriangle3D(std::ofstream &output, const triangle2D &t, const double &za, const double &zb, const double &zc) const
    {
        addTriangle3D(output, triangle2Dto3D(t, za, zb, zc));
    }

    void addTriangle3D(std::ofstream &output, const triangle3D &t) const
    {
        output << "    facet normal 0 0 0" << std::endl
               << "        outer loop" << std::endl
               << "        vertex " << t.a().x() + offset_.x() << " " << t.a().y() + offset_.y() << " " << t.a().z() << std::endl
               << "        vertex " << t.b().x() + offset_.x() << " " << t.b().y() + offset_.y() << " " << t.b().z() << std::endl
               << "        vertex " << t.c().x() + offset_.x() << " " << t.c().y() + offset_.y() << " " << t.c().z() << std::endl
               << "        endloop" << std::endl
               << "    endfacet" << std::endl;
    }

    void exportSTLRaw(const fileName &filename)
    {
        std::ofstream output(filename);

        output << "solid triangulation" << std::endl;
        forAll(triangles_, i)
        {
            const triIndex &tt = this->triangles_[i];
            triangle2D t = triangle2D(this->points_[tt.a], this->points_[tt.b], this->points_[tt.c]);
            addTriangle2D(output, t);
        }
        output << "endsolid triangulation" << std::endl;
    }

    void exportSTL(const fileName &filename, const gridfile &terrain, label lineSplits, scalar domainHeight)
    {
        std::ofstream output(filename);


        output << "solid terrain" << std::endl;
        forAll(triangles_, i)
        {
            const triIndex &tt = this->triangles_[i];
            triangle2D tm = triangle2D(this->points_[tt.a], this->points_[tt.b], this->points_[tt.c]);
            point2D l0 = (tm.b() - tm.a())/lineSplits;
            point2D l1 = (tm.c() - tm.a())/lineSplits;

            for (int j=0; j<lineSplits; j++)
            {
                for (int k=0; k<(lineSplits-j); k++)
                {
                    triangle2D t = triangle2D(tm.a()+l1*j+l0*k, tm.a()+l1*(j+1)+l0*k, tm.a()+l1*j+l0*(k+1));
                    addTriangle3D(output, t, terrain);
                }
                for (int k=0; k<(lineSplits-j-1); k++)
                {
                    triangle2D t = triangle2D(tm.a()+l1*(j+1)+l0*(k+1), tm.a()+l1*j+l0*(k+1),tm.a()+l1*(j+1)+l0*k);
                    addTriangle3D(output, t, terrain);
                }
            }
        }
        output << "endsolid terrain" << std::endl;

        output << "solid top" << std::endl;
        forAll(triangles_, i)
        {
            const triIndex &tt = this->triangles_[i];
            triangle2D tm = triangle2D(this->points_[tt.a], this->points_[tt.b], this->points_[tt.c]);
            point2D l0 = (tm.b() - tm.a())/lineSplits;
            point2D l1 = (tm.c() - tm.a())/lineSplits;

            double za0 = terrain.interpolate(tm.a().x(), tm.a().y())+domainHeight;
            double zb0 = terrain.interpolate(tm.b().x(), tm.b().y())+domainHeight;
            double zc0 = terrain.interpolate(tm.c().x(), tm.c().y())+domainHeight;
            double zl0 = (zb0 - za0)/lineSplits;
            double zl1 = (zc0 - za0)/lineSplits;

            for (int j=0; j<lineSplits; j++)
            {
                for (int k=0; k<(lineSplits-j); k++)
                {
                    triangle2D t = triangle2D(tm.a()+l1*j+l0*k, tm.a()+l1*(j+1)+l0*k, tm.a()+l1*j+l0*(k+1));
                    double za = za0+zl1*j+zl0*k;
                    double zb = za0+zl1*(j+1)+zl0*k;
                    double zc = za0+zl1*j+zl0*(k+1);
                    addTriangle3D(output, t, za, zb, zc);
                }
                for (int k=0; k<(lineSplits-j-1); k++)
                {
                    triangle2D t = triangle2D(tm.a()+l1*(j+1)+l0*(k+1), tm.a()+l1*j+l0*(k+1),  tm.a()+l1*(j+1)+l0*k);
                    double za = za0+zl1*(j+1)+zl0*(k+1);
                    double zb = za0+zl1*j+zl0*(k+1);
                    double zc = za0+zl1*(j+1)+zl0*k;
                    addTriangle3D(output, t, za, zb, zc);
                }
            }
        }
        output << "endsolid top" << std::endl;

        forAll(points_, i)
        {
            output << "solid wall" << i << std::endl;

            point2D lb, le;

            if (i < points_.size()-1)
            {
                lb = points_[i];
                le = points_[i+1];
            }
            else
            {
                lb = points_[i];
                le = points_[0];
            }
            scalar za0 = terrain.interpolate(lb.x(), lb.y())+domainHeight;
            scalar zb0 = terrain.interpolate(le.x(), le.y())+domainHeight;
            point2D l0 = (le-lb)/lineSplits;
            scalar zl0 = (zb0 - za0)/lineSplits;

            for (label k=0; k<lineSplits; k++)
            {
                triangle2D t = triangle2D(lb+k*l0, lb+(k+1)*l0, lb+k*l0);
                scalar za = terrain.interpolate(t.a().x(), t.a().y());
                scalar zb = terrain.interpolate(t.b().x(), t.b().y());
                scalar zc = za0+zl0*k;
                addTriangle3D(output, t, za, zb, zc);

                t = triangle2D(lb+k*l0, lb+(k+1)*l0, lb+(k+1)*l0);
                za = za0+zl0*k;
                zc = za0+zl0*(k+1);
                zb = terrain.interpolate(t.c().x(), t.c().y());
                addTriangle3D(output, t, za, zb, zc);
            }
        output << "endsolid wall" << i << std::endl;
        }
    }
};


int main(int argc, char *argv[])
{
    argList::addNote
    (
        "(avalanche)\n"
        "Generate an STL file out of geodata."
    );

    #include "setRootCase.H"
    #include "createTime.H"

    IOdictionary gridToSTLDict
    (
         IOobject
         (
              "gridToSTLDict",
              runTime.system(),
              runTime,
              IOobject::MUST_READ,
              IOobject::NO_WRITE
         )
    );

    const vector offset = gridToSTLDict.getOrDefault<vector>("offset", Zero);
    const label divisions = gridToSTLDict.getOrDefault<label>("divisions", 100);
    const scalar domainHeight =
        gridToSTLDict.getOrDefault<scalar>("domainHeight", 1000.);

    fileName gridName = gridToSTLDict.get<fileName>("gridname");
    word boundary = gridToSTLDict.get<word>("boundary");
    fileName stlName = gridToSTLDict.get<fileName>("stlname");

    polygon2D bPoints;

    Info << "Reading boundary" << endl;

    if (boundary == "fromShape")
    {
        fileName shapeBoundary = gridToSTLDict.get<fileName>("shapeBoundary");
        shapefile shp;

        if (! shp.read(shapeBoundary))
        {
            FatalIOErrorIn("gridToSTL.C", gridToSTLDict)
                << "Unable to read shapefile " << shapeBoundary << "."
                << exit(FatalIOError);
        }

        if (shp.shptype() != shapefile::POLYLINE && shp.shptype() != shapefile::POLYGON)
        {
            FatalIOErrorIn("gridToSTL.C", gridToSTLDict)
                << "Boundary shapefile must be of type POLYLINE or POLYGON."
                << exit(FatalIOError);
        }

        int polyIndex = -1;

        for(unsigned int i=0; i< shp.recordcount(); i++)
        {
            //There are often empty shapes in shapefiles that we want to skip.
            //We select the last record with parts for the boundary
            if (shp.partcount(i) > 0 && shp.pointcount(i) > 0)
                polyIndex = i;
        }

        if (polyIndex < 0)
        {
            FatalIOErrorIn("gridToSTL.C", gridToSTLDict)
                << "Boundary shapefile contains no usable record."
                << exit(FatalIOError);
        }

        bPoints.resize(shp.pointcount(polyIndex));

        forAll(bPoints, i)
        {
            bPoints[i] = point2D(shp.x(polyIndex,i), shp.y(polyIndex,i));
        }
        if (bPoints[0] == bPoints[bPoints.size()-1])
        {
            bPoints.resize(bPoints.size()-1);
        }
    }
    else if (boundary == "fromPoints")
    {
        List<vector> vertices
        (
            gridToSTLDict.get<List<vector>>("boundaryPoints")
        );

        bPoints.resize(vertices.size());
        forAll(vertices, vI)
        {
            bPoints[vI] = point2D(vertices[vI].x(), vertices[vI].y());
        }
    }
    else
    {
        FatalIOErrorIn("gridToSTL.C", gridToSTLDict)
            << "Boundary \"" << boundary
            << "\" not known. Please choose \"fromShape\" or \"fromPoints\""
            << exit(FatalIOError);
    }

    Info << "Using boundary polygon " << nl
         << "(" << nl;
    forAll(bPoints, i)
    {
        Info << "    (" << bPoints[i].x() << ", " << bPoints[i].y() << ")" << nl;
    }
    Info << ")" << nl;

    Info << "Generating base triangulation" << endl;


    bpoly bTri(bPoints, point2D(offset.x(), offset.y()));
    bTri.triangulate();

    Info << "Reading grid file" << endl;

    gridfile grid;
    grid.read(gridName);

    bTri.exportSTL(stlName, grid, divisions, domainHeight);


    Info<< nl << "End" << nl;
    return 0;
}


// ************************************************************************* //
