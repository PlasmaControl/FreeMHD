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
    Foam::suspensionEntrainmentModels::suspensionParkerFukushimaEntrainment

Description
    Sediment entrainment model following Parker et al. (1986)

    Definition follows Parker et al. (1986), doi.org/10.1017/S0022112086001404


SourceFiles
    suspensionParkerFukushimaEntrainment.C

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#ifndef suspensionParkerFukushimaEntrainment_H
#define suspensionParkerFukushimaEntrainment_H

#include "suspensionEntrainmentModel.H"
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
namespace suspensionEntrainmentModels
{

/*---------------------------------------------------------------------------*\
                           Class Erosionenergy Declaration
\*---------------------------------------------------------------------------*/

class suspensionParkerFukushimaEntrainment
:
    public suspensionEntrainmentModel
{
    // Private data

        //- Density ratio R = rhos/rhow
        dimensionedScalar R_;

        //- Sediment diameter
        dimensionedScalar Ds_;

        //- Water viscosity
        dimensionedScalar nu_;

        //- Entrainment parameter
        dimensionedScalar Zm_;

        //- Entrainment parameter
        dimensionedScalar Zc_;

        //- Reference to the gravitation fields
        const areaVectorField& gs_;

        const areaScalarField& gn_;

        areaScalarField Rp_;

        areaScalarField vs_;

public:

    //- Runtime type information
    TypeName("ParkerFukushimaEntrainment");


    // Constructors

        //- Construct from components
        suspensionParkerFukushimaEntrainment
        (
            const dictionary& frictionProperties,
            const areaVectorField& Us,
            const areaScalarField& h,
            const areaScalarField& hentrain,
            const areaScalarField& c,
            const areaVectorField& tau
        );


    //- Destructor
    virtual ~suspensionParkerFukushimaEntrainment() = default;


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
