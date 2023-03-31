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

Namespace
    Foam::frictionModels

Description
    A namespace for various friction model implementations.

Class
    Foam::frictionModel

Description
    An abstract base class for friction models

SourceFiles
    frictionModel.C
    frictionModelNew.C

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#ifndef frictionModel_H
#define frictionModel_H

#include "IOdictionary.H"
#include "typeInfo.H"
#include "runTimeSelectionTables.H"
#include "dimensionedScalar.H"
#include "tmp.H"
#include "autoPtr.H"
#include "faMatrices.H"
#include "areaFieldsFwd.H"
#include "FieldFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class frictionModel Declaration
\*---------------------------------------------------------------------------*/

class frictionModel
{
protected:

    // Protected data

        dictionary frictionProperties_;

        //- Model coefficients dictionary
        dictionary coeffDict_;

        //- The density
        dimensionedScalar rho_;

        //- Tolerance for velocity
        dimensionedScalar u0_;

        //- Tolerance for flow height
        dimensionedScalar h0_;

        //- Reference to the surface velocity field
        const areaVectorField& Us_;

        //- Reference to the flow height field
        const areaScalarField& h_;

        //- Reference to the bottom pressure field
        const areaScalarField& p_;

        //- Implicit part of friction
        mutable areaScalarField tauSp_;

        //- Explicit part of friction
        mutable areaVectorField tauSc_;


    // Protected Member Functions

        //- Read/update the frictionProperties and coeffDict dictionaries
        void readDict(const word& type, const dictionary& dict);

        //- Set tauSc field to zero
        void resetTauSc() const;

        //- Set tauSp field to zero
        void resetTauSp() const;


        //- Disallow copy construct
        frictionModel(const frictionModel&) = delete;

        //- Disallow default bitwise assignment
        void operator=(const frictionModel&) = delete;


public:

    //- Runtime type information
    TypeName("frictionModel");


    // Declare run-time constructor selection table

#ifndef SWIG
        declareRunTimeSelectionTable
        (
            autoPtr,
            frictionModel,
            dictionary,
            (
                const dictionary& frictionProperties,
                const areaVectorField& Us,
                const areaScalarField& h,
                const areaScalarField& p
            ),
            (frictionProperties, Us, h, p)
        );
#endif


    // Selectors

        //- Return a reference to the selected friction model
        static autoPtr<frictionModel> New
        (
            const dictionary& frictionProperties,
            const areaVectorField& Us,
            const areaScalarField& h,
            const areaScalarField& p
        );


    // Constructors

        //- Construct from components
        frictionModel
        (
            const word& type,
            const dictionary& frictionProperties,
            const areaVectorField& Us,
            const areaScalarField& h,
            const areaScalarField& p
        );


    //- Destructor
    virtual ~frictionModel() = default;


    // Member Functions

        //- Read frictionProperties dictionary
        virtual bool read(const dictionary& frictionProperties) = 0;

        //- Return the friction properties dictionary
        const dictionary& frictionProperties() const
        {
            return frictionProperties_;
        }

        //- Const access to the model coefficients dictionary
        virtual const dictionary& coeffDict() const
        {
            return coeffDict_;
        }


        //- The density
        dimensionedScalar rho() const { return rho_; }

        //- The height limiter
        dimensionedScalar h0() const { return h0_; }

        //- The velocity limiter
        dimensionedScalar u0() const { return u0_; }

        //- Return explicit coefficients for the linear system of equations
        //- considering friction
        virtual const areaVectorField& tauSc() const = 0;

        //- Return implicit coefficients for the linear system of equations
        //- considering friction
        virtual const areaScalarField& tauSp() const = 0;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
