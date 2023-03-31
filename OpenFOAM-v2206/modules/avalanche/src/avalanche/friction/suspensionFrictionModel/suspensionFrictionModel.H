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

Namespace
    Foam::suspensionFrictionModels

Description
    A namespace for various friction model implementations.

Class
    Foam::suspensionFrictionModel

Description
    An abstract base class for friction models

SourceFiles
    suspensionFrictionModel.C
    suspensionFrictionModelNew.C

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#ifndef suspensionFrictionModel_H
#define suspensionFrictionModel_H

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
                         Class suspensionFrictionModel Declaration
\*---------------------------------------------------------------------------*/

class suspensionFrictionModel
{
protected:

    // Protected data

        dictionary suspensionFrictionProperties_;

        //- Model coefficients dictionary
        dictionary coeffDict_;

        //- Tolerance for velocity
        dimensionedScalar u0_;

        //- Tolerance for flow height
        dimensionedScalar h0_;

        //- Reference to the surface velocity field
        const areaVectorField& Us_;

        //- Reference to the flow height field
        const areaScalarField& h_;

        //- Reference to the sediment concentration field
        const areaScalarField& c_;

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
        suspensionFrictionModel(const suspensionFrictionModel&) = delete;

        //- Disallow default bitwise assignment
        void operator=(const suspensionFrictionModel&) = delete;


public:

    //- Runtime type information
    TypeName("suspensionFrictionModel");


    // Declare run-time constructor selection table

#ifndef SWIG
        declareRunTimeSelectionTable
        (
            autoPtr,
            suspensionFrictionModel,
            dictionary,
            (
                const dictionary& suspensionFrictionProperties,
                const areaVectorField& Us,
                const areaScalarField& h,
                const areaScalarField& c
            ),
            (suspensionFrictionProperties, Us, h, c)
        );
#endif


    // Selectors

        //- Return a reference to the selected friction model
        static autoPtr<suspensionFrictionModel> New
        (
            const dictionary& suspensionFrictionProperties,
            const areaVectorField& Us,
            const areaScalarField& h,
            const areaScalarField& c
        );


    // Constructors

        //- Construct from components
        suspensionFrictionModel
        (
            const word& type,
            const dictionary& suspensionFrictionProperties,
            const areaVectorField& Us,
            const areaScalarField& h,
            const areaScalarField& c
        );


    //- Destructor
    virtual ~suspensionFrictionModel() = default;


    // Member Functions

        //- Read frictionProperties dictionary
        virtual bool read(const dictionary& suspensionFrictionProperties) = 0;

        //- Return the friction properties dictionary
        const dictionary& suspensionFrictionProperties() const
        {
            return suspensionFrictionProperties_;
        }

        //- Const access to the model coefficients dictionary
        virtual const dictionary& coeffDict() const
        {
            return coeffDict_;
        }



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
