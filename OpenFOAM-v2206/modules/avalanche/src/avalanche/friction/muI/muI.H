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
    Foam::frictionModels::MuI

Description
    An incompressible mu(I) friction model. The shear rate at the base follows
    from the Bagnold profile.

    gamma = 5./2.*U/h
    I = gamma*d/sqrt(p/rhop)
    mu = mus + (mu2-mus)/(I0/I+1)
    tau_b = mu*p_b

    Jop et al., "A constitutive law for dense granular flows",
    Nature volume 441, pages727-730 (2006), dx.doi.org/10.1038/nature04801

    Rauter and Tukovic, "A finite area scheme for shallow granular flows on
    three-dimensional surfaces", Computers and Fluids:166, 184-199 (2018),
    dx.doi.org/10.1016/j.compfluid.2018.02.017

SourceFiles
    MuI.C

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#ifndef MuI_H
#define MuI_H

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
                           Class MuI Declaration
\*---------------------------------------------------------------------------*/

class MuI
:
    public frictionModel
{
    // Private data

    //- Particle diameter d
    dimensionedScalar d_;

    //- Particle density rho_p
    dimensionedScalar rhop_;

    //- Friction parameter mu_s
    dimensionedScalar mus_;

    //- Friction parameter mu_2
    dimensionedScalar mu2_;

    //- Friction parameter I_0
    dimensionedScalar I0_;

    //- Effective mu coefficient
    mutable areaScalarField mu_;


public:

    //- Runtime type information
    TypeName("MuI");


    // Constructors

        //- Construct from components
        MuI
        (
            const dictionary& frictionProperties,
            const areaVectorField& Us,
            const areaScalarField& h,
            const areaScalarField& p
        );


    //- Destructor
    virtual ~MuI() = default;


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
