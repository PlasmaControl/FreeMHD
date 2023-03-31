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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline void Foam::Module::matrix2D::calculateDeterminant()
{
    if (calculatedDet_)
    {
        return;
    }

    const FixedList<FixedList<scalar, 2>, 2>& mat = *this;
    det_ = mat[0][0]*mat[1][1] - mat[0][1]*mat[1][0];

    calculatedDet_ = true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline Foam::Module::matrix2D::matrix2D()
:
    det_(),
    calculatedDet_(false)
{}


inline Foam::Module::matrix2D::matrix2D(const matrix2D& m)
:
    FixedList<FixedList<scalar, 2>, 2>(m),
    det_(m.det_),
    calculatedDet_(m.calculatedDet_)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline Foam::scalar Foam::Module::matrix2D::determinant()
{
    calculateDeterminant();

    return det_;
}


inline Foam::Module::matrix2D Foam::Module::matrix2D::inverse()
{
    calculateDeterminant();

    const FixedList<FixedList<scalar, 2>, 2>& mat = *this;

    matrix2D imat;
    imat[0][0] = mat[1][1] / det_;
    imat[0][1] = -1.0*mat[0][1] / det_;
    imat[1][0] = -1.0*mat[1][0] / det_;
    imat[1][1] = mat[0][0] / det_;

    return matrix2D(imat);
}


inline Foam::FixedList<Foam::scalar, 2> Foam::Module::matrix2D::solve
(
    const FixedList<scalar, 2>& source
)
{
    FixedList<scalar, 2> result;
    result[0] = solveFirst(source);
    result[1] = solveSecond(source);

    return result;
}


inline Foam::scalar Foam::Module::matrix2D::solveFirst
(
    const FixedList<scalar, 2>& source
)
{
    calculateDeterminant();

    const FixedList<FixedList<scalar, 2>, 2>& mat = *this;

    return
    (
        mat[1][1]*source[0] -
        mat[0][1]*source[1]
    ) / det_;
}


inline Foam::scalar Foam::Module::matrix2D::solveSecond
(
    const FixedList<scalar, 2>& source
)
{
    calculateDeterminant();

    const FixedList<FixedList<scalar, 2>, 2>& mat = *this;

    return
    (
        -1.0*mat[1][0]*source[0] +
        mat[0][0]*source[1]
    ) / det_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
