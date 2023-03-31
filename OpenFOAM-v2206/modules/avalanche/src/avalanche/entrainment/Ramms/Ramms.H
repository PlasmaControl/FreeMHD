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

Class
    Foam::entrainmentModels::Ramms

Description
    An entrainment model following the RAMMS approach.

    Christen et al. "RAMMS: Numerical simulation of dense snow avalanches
    in three-dimensional terrain", Cold Regions Science and Technology, 63,
    2010, dx.doi.org/10.1016/j.coldregions.2010.04.005

SourceFiles
    Ramms.C

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#ifndef Ramms_H
#define Ramms_H

#include "entrainmentModel.H"
#include "dimensionedScalar.H"
#include "volFields.H"
#include "IOdictionary.H"
#include "typeInfo.H"
#include "runTimeSelectionTables.H"
#include "dimensionedScalar.H"
#include "tmp.H"
#include "autoPtr.H"
#include "faMatrices.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace entrainmentModels
{

/*---------------------------------------------------------------------------*\
                           Class Erosionenergy Declaration
\*---------------------------------------------------------------------------*/

class Ramms
:
    public entrainmentModel
{
    // Private data

        //- Erosion rate kappa
        dimensionedScalar kappa_;

public:

    //- Runtime type information
    TypeName("Ramms");


    // Constructors

        //- Construct from components
        Ramms
        (
            const dictionary& frictionProperties,
            const areaVectorField& Us,
            const areaScalarField& h,
            const areaScalarField& hentrain,
            const areaScalarField& pb,
            const areaVectorField& tau
        );


    //- Destructor
    virtual ~Ramms() = default;


    // Member Functions

        //- Return the Source by entrainment
        virtual const areaScalarField& Sm() const;

        //- Read entrainmentProperties dictionary
        virtual bool read(const dictionary& entrainmentProperties);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace frictionModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
