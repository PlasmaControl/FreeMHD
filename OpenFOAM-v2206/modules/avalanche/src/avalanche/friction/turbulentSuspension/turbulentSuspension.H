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

Class
    Foam::frictionModels::turbulentSuspension

Description
    turbulent friction following Parker et al. (1986)

    Definition follows Parker et al. (1986), doi.org/10.1017/S0022112086001404

SourceFiles
    turbulentSuspension.C

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#ifndef turbulentSuspension_H
#define turbulentSuspension_H

#include "suspensionFrictionModel.H"
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
namespace suspensionFrictionModels
{

/*---------------------------------------------------------------------------*\
                     Class turbulentSuspension Declaration
\*---------------------------------------------------------------------------*/

class turbulentSuspension
:
    public suspensionFrictionModel
{
    // Private Data

        dimensionedScalar alpha_;

        dimensionedScalar betaConst_;

        dimensionedScalar cd_;

        dimensionedScalar R_;

        dimensionedScalar Ds_;

        dimensionedScalar nu_;

        dimensionedScalar kmin_;

        mutable areaScalarField k_;

        const edgeScalarField& phi2s_;

        const areaVectorField& tau_;

        const areaScalarField& es_;

        const areaScalarField& ds_;

        const areaScalarField& ew_;

        //- Reference to the gravitation fields
        const areaVectorField& gs_;

        const areaScalarField& gn_;

        areaScalarField vs_;


public:

    //- Runtime type information
    TypeName("turbulentSuspension");


    // Constructors

        //- Construct from components
        turbulentSuspension
        (
            const dictionary& frictionProperties,
            const areaVectorField& Us,
            const areaScalarField& h,
            const areaScalarField& c
        );


    //- Destructor
    virtual ~turbulentSuspension() = default;


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

} // End namespace suspensionFrictionModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
