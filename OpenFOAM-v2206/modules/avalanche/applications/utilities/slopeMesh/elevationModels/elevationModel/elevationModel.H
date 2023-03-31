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
    Foam::elevationModels

Description
    A namespace for various elevationModel implementations.

Class
    Foam::elevationModel

Description
    An abstract base class for elevationModels

SourceFiles
    elevationModel.C
    elevationModelNew.C

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#ifndef elevationModel_H
#define elevationModel_H

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
                         Class elevationModel Declaration
\*---------------------------------------------------------------------------*/

class elevationModel
{
protected:

    // Protected data

        word name_;

        dictionary elevationProperties_;


    // Private Member Functions

        //- Disallow copy construct
        elevationModel(const elevationModel&) = delete;

        //- Disallow default bitwise assignment
        void operator=(const elevationModel&) = delete;


public:

    //- Runtime type information
    TypeName("elevationModel");


    // Declare run-time constructor selection table

#ifndef SWIG
        declareRunTimeSelectionTable
        (
            autoPtr,
            elevationModel,
            dictionary,
            (
                const word& name,
                const dictionary& elevationProperties
            ),
            (name, elevationProperties)
        );
#endif


    // Selectors

        //- Return a reference to the selected elevation model
        static autoPtr<elevationModel> New
        (
            const word& name,
            const dictionary& elevationProperties
        );


    // Constructors

        //- Construct from components
        elevationModel
        (
            const word& name,
            const dictionary& elevationProperties
        );


    //- Destructor
    virtual ~elevationModel() = default;


    // Member Functions

        //- Return the global point X for the local point x
        virtual vector X(const vector& x) const = 0;

        //- Return the elevation properties dictionary
        const dictionary& elevationProperties() const
        {
            return elevationProperties_;
        }

        //- Read elevationProperties dictionary
        virtual bool read(const dictionary& elevationProperties) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
