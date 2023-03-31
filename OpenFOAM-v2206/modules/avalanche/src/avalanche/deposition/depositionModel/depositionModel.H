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
    Foam::depositionModels

Description
    A namespace for various deposition model implementations.

Class
    Foam::depositionModel

Description
    An abstract base class for deposition models

SourceFiles
    depositionModel.C
    depositionModelNew.C

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#ifndef depositionModel_H
#define depositionModel_H

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
                       Class depositionModel Declaration
\*---------------------------------------------------------------------------*/

class depositionModel
{
protected:

    // Protected data

        dictionary depositionProperties_;

        //- Model coefficients dictionary
        dictionary coeffDict_;

        //- The density
        dimensionedScalar rho_;

        //- Reference to the surface velocity field
        const areaVectorField& Us_;

        //- Reference to the flow height field
        const areaScalarField& h_;

        //- Reference to the cover height field
        const areaScalarField& hentrain_;

        //- Reference to the basal pressure field
        const areaScalarField& pb_;

        //- Reference to the bottom stress field
        const areaVectorField& tau_;

        //- Sink term
        mutable areaScalarField Sd_;


    // Protected Member Functions

        //- Read/update the depositionProperties and coeffDict dictionaries
        void readDict(const word& type, const dictionary& dict);


        //- Disallow copy construct
        depositionModel(const depositionModel&) = delete;

        //- Disallow default bitwise assignment
        void operator=(const depositionModel&) = delete;


public:

    //- Runtime type information
    TypeName("depositionModel");


    // Declare run-time constructor selection table

#ifndef SWIG
        declareRunTimeSelectionTable
        (
            autoPtr,
            depositionModel,
            dictionary,
            (
                const dictionary& depositionProperties,
                const areaVectorField& Us,
                const areaScalarField& h,
                const areaScalarField& hentrain,
                const areaScalarField& pb,
                const areaVectorField& tau
            ),
            (depositionProperties, Us, h, hentrain, pb, tau)
        );
#endif


    // Selectors

        //- Return a reference to the selected deposition model
        static autoPtr<depositionModel> New
        (
            const dictionary& depositionProperties,
            const areaVectorField& Us,
            const areaScalarField& h,
            const areaScalarField& hentrain,
            const areaScalarField& pb,
            const areaVectorField& tau
        );


    // Constructors

        //- Construct from components
        depositionModel
        (
            const word& type,
            const dictionary& depositionProperties,
            const areaVectorField& Us,
            const areaScalarField& h,
            const areaScalarField& hentrain,
            const areaScalarField& pb,
            const areaVectorField& tau
        );


    //- Destructor
    virtual ~depositionModel() = default;


    // Member Functions

        //- Read depositionProperties dictionary
        virtual bool read(const dictionary& depositionProperties) = 0;

        //- Return the friction properties dictionary
        const dictionary& depositionProperties() const
        {
            return depositionProperties_;
        }

        //- Const access to the model coefficients dictionary
        virtual const dictionary& coeffDict() const
        {
            return coeffDict_;
        }


        //- Return Sink by deposition
        virtual const areaScalarField& Sd() const = 0;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
