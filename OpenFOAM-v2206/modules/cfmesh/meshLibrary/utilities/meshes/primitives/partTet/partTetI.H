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

#include "triangle.H"
#include "tetrahedron.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline Foam::Module::partTet::partTet
(
    const label a,
    const label b,
    const label c,
    const label d
)
{
    data_[0] = a;
    data_[1] = b;
    data_[2] = c;
    data_[3] = d;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline Foam::label Foam::Module::partTet::a() const
{
    return data_[0];
}


inline Foam::label Foam::Module::partTet::b() const
{
    return data_[1];
}


inline Foam::label Foam::Module::partTet::c() const
{
    return data_[2];
}


inline Foam::label Foam::Module::partTet::d() const
{
    return data_[3];
}


inline Foam::label Foam::Module::partTet::size() const
{
    return 4;
}


inline Foam::label
Foam::Module::partTet::whichPosition(const label pointI) const
{
    for (label i = 0; i < 4; ++i)
    {
        if (data_[i] == pointI)
        {
            return i;
        }
    }

    return -1;
}


template<class PointField>
inline Foam::vector Foam::Module::partTet::Sa(const PointField& points) const
{
    triangle<point, point> tria
    (
        points[data_[1]],
        points[data_[2]],
        points[data_[3]]
    );

    return tria.areaNormal();
    //return triangle<point, point>(b_, c_, d_).normal();
}


template<class PointField>
inline Foam::vector Foam::Module::partTet::Sb(const PointField& points) const
{
    triangle<point, point> tria
    (
        points[data_[0]],
        points[data_[3]],
        points[data_[2]]
    );

    return tria.areaNormal();
    //return triangle<point, point>(a_, d_, c_).normal();
}


template<class PointField>
inline Foam::vector Foam::Module::partTet::Sc(const PointField& points) const
{
    triangle<point, point> tria
    (
        points[data_[0]],
        points[data_[1]],
        points[data_[3]]
    );

    return tria.areaNormal();
    //return triangle<point, point>(a_, b_, d_).normal();
}


template<class PointField>
inline Foam::vector Foam::Module::partTet::Sd(const PointField& points) const
{
    triangle<point, point> tria
    (
        points[data_[0]],
        points[data_[2]],
        points[data_[1]]
    );

    return tria.areaNormal();
    //return triangle<point, point>(a_, c_, b_).normal();
}


template<class PointField>
inline Foam::scalar Foam::Module::partTet::mag(const PointField& points) const
{
    tetrahedron<point, point> tet
    (
        points[data_[0]],
        points[data_[1]],
        points[data_[2]],
        points[data_[3]]
    );

    return tet.mag();
    //return (1.0/6.0)*(((b_ - a_) ^ (c_ - a_)) & (d_ - a_));
}


template<class PointField>
inline Foam::point
Foam::Module::partTet::crcmCentre(const PointField& points) const
{
    tetrahedron<point, point> tet
    (
        points[data_[0]],
        points[data_[1]],
        points[data_[2]],
        points[data_[3]]
    );

    return tet.circumCentre();
}


template<class PointField>
inline Foam::scalar
Foam::Module::partTet::crcmRadius(const PointField& points) const
{
    tetrahedron<point, point> tet
    (
        points[data_[0]],
        points[data_[1]],
        points[data_[2]],
        points[data_[3]]
    );

    return tet.circumRadius();
}


template<class PointField>
inline Foam::point
Foam::Module::partTet::centroid(const PointField& points) const
{
    point p = points[data_[0]];
    for (label i = 1; i < 4; ++i)
    {
        p += points[data_[i]];
    }

    p /= 4;
    return p;
}


inline Foam::FixedList<Foam::edge, 6> Foam::Module::partTet::edges() const
{
    FixedList<edge, 6> edges;
    edges[0] = edge(data_[0], data_[1]);
    edges[1] = edge(data_[0], data_[2]);
    edges[2] = edge(data_[0], data_[3]);
    edges[3] = edge(data_[3], data_[1]);
    edges[4] = edge(data_[1], data_[2]);
    edges[5] = edge(data_[3], data_[2]);

    return edges;
}


inline Foam::edge Foam::Module::partTet::getEdge(const label eI) const
{
    switch(eI)
    {
        case 0:
        {
            return edge(data_[0], data_[1]);
        } break;
        case 1:
        {
            return edge(data_[0], data_[2]);
        } break;
        case 2:
        {
            return edge(data_[0], data_[3]);
        } break;
        case 3:
        {
            return edge(data_[3], data_[1]);
        } break;
        case 4:
        {
            return edge(data_[1], data_[2]);
        } break;
        case 5:
        {
            return edge(data_[3], data_[2]);
        } break;
    }

    FatalErrorInFunction
        << "Invalid edge index given " << eI << abort(FatalError);

    return edge();
}


inline Foam::label Foam::Module::partTet::operator[](const label i) const
{
    return data_[i];
}


inline void Foam::Module::partTet::operator=(const partTet& tet)
{
    for (label i = 0; i < 4; ++i)
    {
        data_[i] = tet.data_[i];
    }
}


inline bool Foam::Module::partTet::operator==(const partTet& tet) const
{
    if (this->a() != tet.a())
    {
        return false;
    }
    if (this->b() != tet.b())
    {
        return false;
    }
    if (this->c() != tet.c())
    {
        return false;
    }
    if (this->d() != tet.d())
    {
        return false;
    }

    return true;
}


inline bool Foam::Module::partTet::operator!=(const partTet& tet) const
{
    return !this->operator==(tet);
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

inline Foam::Ostream& Foam::Module::operator<<
(
    Ostream& os,
    const Foam::Module::partTet& t
)
{
    os  << nl
        << token::BEGIN_LIST
        << t.a() << token::SPACE << t.b()
        << token::SPACE << t.c() << token::SPACE << t.d()
        << token::END_LIST;

    return os;
}


// ************************************************************************* //
