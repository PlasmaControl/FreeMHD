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

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#include "SimpleSlope.H"
#include "unitConversion.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace elevationModels
{
    defineTypeNameAndDebug(SimpleSlope, 0);
    addToRunTimeSelectionTable(elevationModel, SimpleSlope, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::elevationModels::SimpleSlope::SimpleSlope
(
    const word& name,
    const dictionary& elevationProperties
)
:
    elevationModel(name, elevationProperties),
    alpha_(elevationProperties_.subDict("SimpleSlopeCoeffs").getOrDefault<scalar>("alpha", 35.)),
    beta_(elevationProperties_.subDict("SimpleSlopeCoeffs").getOrDefault<scalar>("alpha2", 15.)),
    a_(elevationProperties_.subDict("SimpleSlopeCoeffs").getOrDefault<scalar>("a", 17.5)),
    b_(elevationProperties_.subDict("SimpleSlopeCoeffs").getOrDefault<scalar>("b", 21.5))

{
    alpha_ = degToRad(alpha_);
    beta_ = degToRad(beta_);
    ab_ = b_-a_;

    Info<< "Creating simple slope, transfer between "
        << globalZ(a_) << " and " << globalZ(b_) << endl;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

Foam::vector Foam::elevationModels::SimpleSlope::X(const vector &x) const
{
    return vector(globalX(x[0]),x[1], globalZ(x[0])+x[2]);
}


bool Foam::elevationModels::SimpleSlope::read
(
    const dictionary& elevationProperties
)
{
    //elevationModel::read(elevationProperties);

    return true;
}


Foam::scalar Foam::elevationModels::SimpleSlope::globalX(scalar x) const
{
    if (x < a_)
        return cos(alpha_)*x;
    else if (x < b_)
        return cos(alpha_)*a_
               -ab_/(alpha_-beta_)*sin(alpha_*(a_+ab_-x)/ab_
                                       -beta_*(a_-x)/ab_)
               +ab_/(alpha_-beta_)*sin(alpha_);
    else
        return cos(alpha_)*a_
               -ab_/(alpha_-beta_)*sin(alpha_*(a_+ab_-b_)/ab_
                                       -beta_*(a_-b_)/ab_)
               +ab_/(alpha_-beta_)*sin(alpha_)
               +cos(beta_)*(x-b_);
}


Foam::scalar Foam::elevationModels::SimpleSlope::globalZ(scalar x) const
{
    if (x < a_)
        return -sin(alpha_)*x;
    else if (x < b_)
        return -sin(alpha_)*a_
               -ab_/(alpha_-beta_)*cos(alpha_*(a_+ab_-x)/ab_
                                       -beta_*(a_-x)/ab_)
               +ab_/(alpha_-beta_)*cos(alpha_);
    else
        return -sin(alpha_)*a_
               -ab_/(alpha_-beta_)*cos(alpha_*(a_+ab_-b_)/ab_
                                       -beta_*(a_-b_)/ab_)
               +ab_/(alpha_-beta_)*cos(alpha_)
               -sin(beta_)*(x-b_);
}


// ************************************************************************* //
