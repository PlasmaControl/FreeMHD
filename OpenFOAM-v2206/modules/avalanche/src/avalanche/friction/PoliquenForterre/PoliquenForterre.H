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
    Foam::frictionModels::PoliquenForterre

Description
    A mu(I) friction model, with the parametrisation of
    Pouliquen & Forterre (2002).

    Fr = u/sqrt(h*gn)
    hs = h*beta/Fr;
    mu_start = tan(zeta3) + (tan(zeta2) - tan(zeta1))*1/(1+hs/L)
    mu_stop =  tan(zeta1) + (tan(zeta2) - tan(zeta1))*exp(-hs/L_)
    mu = mu_start + (Fr/beta)^gamma * (mu_stop-mu_start)
    tau_b = mu*p_b

    Pouliquen & Forterre. "Friction law for dense granular flows: application
    to the motion of a mass down a rough inclined plane." J. Fluid Mech.
    453, 133-151 (2002), dx.doi.org/10.1017/S0022112001006796

    Johnson & Gray. "Granular jets and hydraulic jumps on an inclined plane"
    J. Fluid Mech. 675, 87-116 (2011), dx.doi.org/10.1017/jfm.2011.2

SourceFiles
    PoliquenForterre.C

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#ifndef PoliquenForterre_H
#define PoliquenForterre_H

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
#include "uniformDimensionedFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace frictionModels
{

/*---------------------------------------------------------------------------*\
                      Class PoliquenForterre Declaration
\*---------------------------------------------------------------------------*/

class PoliquenForterre
:
    public frictionModel
{
    // Private data

    //- PoliquenForterre friction parameter L
    dimensionedScalar L_;

    //- PoliquenForterre friction parameter beta
    dimensionedScalar beta_;

    //- PoliquenForterre friction parameter zeta1
    dimensionedScalar zeta1_;

    //- PoliquenForterre friction parameter zeta2
    dimensionedScalar zeta2_;

    //- PoliquenForterre friction parameter zeta3
    dimensionedScalar zeta3_;

    //- PoliquenForterre friction parameter zeta3
    dimensionedScalar gamma_;

    //- mu effective
    mutable areaScalarField mu_;

    //- gravity constant
    uniformDimensionedVectorField g_;


public:

    //- Runtime type information
    TypeName("PoliquenForterre");


    // Constructors

        //- Construct from components
        PoliquenForterre
        (
            const dictionary& frictionProperties,
            const areaVectorField& Us,
            const areaScalarField& h,
            const areaScalarField& p
        );


    //- Destructor
    virtual ~PoliquenForterre() = default;


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
