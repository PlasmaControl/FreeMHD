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
    Foam::entrainmentModels

Description
    A namespace for various entrainment model implementations.

Class
    Foam::entrainmentModel

Description
    An abstract base class for entrainment models

SourceFiles
    entrainmentModel.C
    entrainmentModelNew.C

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#ifndef entrainmentModel_H
#define entrainmentModel_H

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
                         Class entrainmentModel Declaration
\*---------------------------------------------------------------------------*/

class entrainmentModel
{
protected:

    // Protected data

        dictionary entrainmentProperties_;

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

        //- Source term
        mutable areaScalarField Sm_;


    // Protected Member Functions

        //- Read/update the entrainmentProperties and coeffDict dictionaries
        void readDict(const word& type, const dictionary& dict);


        //- Disallow copy construct
        entrainmentModel(const entrainmentModel&) = delete;

        //- Disallow default bitwise assignment
        void operator=(const entrainmentModel&) = delete;


public:

    //- Runtime type information
    TypeName("entrainmentModel");


    // Declare run-time constructor selection table

#ifndef SWIG
        declareRunTimeSelectionTable
        (
            autoPtr,
            entrainmentModel,
            dictionary,
            (
                const dictionary& entrainmentProperties,
                const areaVectorField& Us,
                const areaScalarField& h,
                const areaScalarField& hentrain,
                const areaScalarField& pb,
                const areaVectorField& tau
            ),
            (entrainmentProperties, Us, h, hentrain, pb, tau)
        );
#endif


    // Selectors

        //- Return a reference to the selected entrainment model
        static autoPtr<entrainmentModel> New
        (
            const dictionary& entrainmentProperties,
            const areaVectorField& Us,
            const areaScalarField& h,
            const areaScalarField& hentrain,
            const areaScalarField& pb,
            const areaVectorField& tau
        );


    // Constructors

        //- Construct from components
        entrainmentModel
        (
            const word& type,
            const dictionary& entrainmentProperties,
            const areaVectorField& Us,
            const areaScalarField& h,
            const areaScalarField& hentrain,
            const areaScalarField& pb,
            const areaVectorField& tau
        );


    //- Destructor
    virtual ~entrainmentModel() = default;


    // Member Functions

        //- Read entrainmentProperties dictionary
        virtual bool read(const dictionary& entrainmentProperties) = 0;

        //- Return the entrainment properties dictionary
        const dictionary& entrainmentProperties() const
        {
            return entrainmentProperties_;
        }

        //- Const access to the model coefficients dictionary
        virtual const dictionary& coeffDict() const
        {
            return coeffDict_;
        }

        //- Return the Source by entrainment
        virtual const areaScalarField& Sm() const = 0;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
