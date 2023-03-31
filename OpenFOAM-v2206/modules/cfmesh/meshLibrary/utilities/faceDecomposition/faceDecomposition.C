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

#include "faceDecomposition.H"
#include "pointField.H"
#include "boolList.H"
#include "helperFunctions.H"

// #define DEBUG_faceDecomposition

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::label Foam::Module::faceDecomposition::concaveVertex() const
{
    const vector n = f_.unitNormal(points_);

    const edgeList edges = f_.edges();

    label concaveVrt(-1);

    forAll(edges, eI)
    {
        const short next = (eI + 1) % f_.size();

        const vector ev = edges[eI].unitVec(points_);

        const vector evn = edges[next].unitVec(points_);

        const vector prod = (ev ^ evn);

        if ((prod & n) < -SMALL)
        {
            if (concaveVrt != -1)
            {
                FatalErrorInFunction
                    << "Face " << f_ << " has more than one concave vertex."
                    << " Cannot continue ..." << exit(FatalError);
            }

            label vrtIndex = edges[eI].commonVertex(edges[next]);
            /*
            if
            (
                pointTriIndex_[vrtIndex]
            &&  pointTriIndex_[vrtIndex]->size() > 1
            )
            */
            concaveVrt = vrtIndex;
        }
    }

    return concaveVrt;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Module::faceDecomposition::faceDecomposition
(
    const face& f,
    const pointField& pts
)
:
    f_(f),
    points_(pts)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

bool Foam::Module::faceDecomposition::isFaceConvex() const
{
    if (concaveVertex() == -1)
    {
        return true;
    }

    return false;
}



bool Foam::Module::faceDecomposition::isFacePlanar(const scalar tol) const
{
    const vector nref = f_.unitNormal(points_);

    forAll(f_, pI)
    {
        if (mag((points_[f_[pI]] - points_[f_[0]]) & nref) > tol)
        {
            # ifdef DEBUG_faceDecomposition
            Info<< "Face is not planar " << endl;
            # endif
            return false;
        }
    }

    return true;
}


bool Foam::Module::faceDecomposition::isFacePlanar() const
{
    scalar tol(0.0);

    const point c = f_.centre(points_);
    forAll(f_, pI)
    {
        tol = Foam::max(tol, Foam::mag(c - points_[f_[pI]]));
    }

    tol *= 0.05;

    return isFacePlanar(tol);
}


Foam::faceList Foam::Module::faceDecomposition::decomposeFace() const
{
    faceList ff = decomposeFaceIntoTriangles();

    if (ff.size() <= 3)
    {
        // face is decomposed into 2 or 3 triangles. I am happy with that.
        return ff;
    }

    boolList mergedFaces(ff.size(), false);
    face lf = ff[0];
    face rf = ff[ff.size()-1];

    direction il(1), ir(ff.size()-2);
    faceList storage(2);
    direction fI(0);

    while (il < ir)
    {
        // merge on the side of lower labels
        face fl = help::mergeTwoFaces(ff[il], lf);
        if (faceDecomposition(fl, points_).isFaceConvex())
        {
            lf = fl;
            if (il == ir - 1)
            {
                storage.newElmt(fI++) = lf;
            }
        }
        else
        {
            storage.newElmt(fI++) = lf;
            lf = ff[il];
            if (il == ir - 1)
            {
                storage.newElmt(fI++) = lf;
            }
        }

        // merge from the side of higher labels
        face fr = help::mergeTwoFaces(rf, ff[ir]);
        if (faceDecomposition(fr, points_).isFaceConvex())
        {
            rf = fr;
            if (il == ir - 1)
            {
                storage.newElmt(fI++) = rf;
            }
        }
        else
        {
            storage.newElmt(fI++) = rf;
            rf = ff[ir];
            if (il == ir - 1)
            {
                storage.newElmt(fI++) = rf;
            }
        }

        il++;
        ir--;

        // this happens if the face has odd number of edges
        if (il == ir)
        {
            fl = help::mergeTwoFaces(ff[il], lf);
            fr = help::mergeTwoFaces(rf, ff[ir]);
            if (faceDecomposition(fl, points_).isFaceConvex())
            {
                storage.newElmt(fI++) = fl;
                storage.newElmt(fI++) = rf;
            }
            else if (faceDecomposition(fr, points_).isFaceConvex())
            {
                storage.newElmt(fI++) = fr;
                storage.newElmt(fI++) = lf;
            }
            else
            {
                storage.newElmt(fI++) = lf;
                storage.newElmt(fI++) = ff[il];
                storage.newElmt(fI++) = rf;
            }
        }
    }

    if (storage.size() > 2)
    {
        storage.setSize(fI);
    }

    # ifdef DEBUG_faceDecomposition
    Info<< "Original face " << f_ << endl;
    Info<< "Triangles " << ff << endl;
    Info<< "Concave vertex " << concaveVertex(f_) << endl;
    Info<< "Storage " << storage << endl;
    # endif

    return storage;
}


Foam::faceList
Foam::Module::faceDecomposition::decomposeFaceIntoTriangles
(
    const label cv
) const
{
    if (cv != -1)
    {
        short start(0);
        forAll(f_, pI)
        {
            if (cv == f_[pI])
            {
                start = pI;
                break;
            }
        }

        faceList fcs(10);
        short fI(0);

        const edgeList edg = f_.edges();

        for (short eI = 1; eI < edg.size()-1; eI++)
        {
            const short i = (eI + start) % f_.size();
            face add(3);
            add[0] = f_[start];
            add[1] = edg[i].start();
            add[2] = edg[i].end();

            fcs.newElmt(fI++) = add;
        }

        fcs.setSize(fI);

        # ifdef DEBUG_faceDecomposition
        Info<< "face " << faceNo << "  " << f_
            << " is decomposed into " << fcs << endl;
        # endif

        return fcs;
    }

    faceList fcs(1, f_);

    return fcs;
}


Foam::faceList
Foam::Module::faceDecomposition::decomposeFaceIntoTriangles() const
{
    const label cv = concaveVertex();
    return decomposeFaceIntoTriangles(cv);
}


// ************************************************************************* //
