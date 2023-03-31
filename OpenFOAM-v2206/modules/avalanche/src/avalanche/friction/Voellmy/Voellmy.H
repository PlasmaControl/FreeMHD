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
    Foam::frictionModels::Voellmy

Description
    The friction model following Voellmy (1955).

    tau_b = mu*p_b + rho*g/xi*U^2

    Voellmy. Ueber die Zerstoerungskraft von Lawinen (On the destructive forces
    of avalanches). Schweizerische Bauzeitung. 1955, 73 (15), 212-217,
    dx.doi.org/10.5169/seals-61891

    see also:

    Rauter et al. "faSavageHutterFOAM 1.0: depth-integrated simulation of dense
    snow avalanches on natural terrain with OpenFOAM."" Geosci. Model Dev. 11,
    2923-2939. 2018 dx.doi.org/10.5194/gmd-11-2923-2018.

SourceFiles
    Voellmy.C

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#ifndef Voellmy_H
#define Voellmy_H

#include "frictionModel.H"
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
namespace frictionModels
{

/*---------------------------------------------------------------------------*\
                           Class Voellmy Declaration
\*---------------------------------------------------------------------------*/

class Voellmy
:
    public frictionModel
{
    // Private data

        //- Coulomb friction parameter m
        dimensionedScalar mu_;

        //- Voellmy friction parameter xi
        dimensionedScalar xi_;

public:

    //- Runtime type information
    TypeName("Voellmy");


    // Constructors

        //- Construct from components
        Voellmy
        (
            const dictionary& frictionProperties,
            const areaVectorField& Us,
            const areaScalarField& h,
            const areaScalarField& p
        );


    //- Destructor
    virtual ~Voellmy() = default;


    // Member Functions

        //- Return explicit coefficients for the linear system of equations
        //- considering friction
        virtual const areaVectorField& tauSc() const;

        //- Return implicit coefficients for the linear system of equations
        //- considering friction
        virtual const areaScalarField& tauSp() const;


        //- Read the friction properties dictionary
        virtual bool read(const dictionary& frictionProperties);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace frictionModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
