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
    Foam::entrainmentModels::Medina

Description
    An entrainment model following the approach of Medina et al. (2008).

    Medina et al. "Application of FLATModel, a 2D finite volume code, to
    debris flows in the northeastern part of the Iberian Peninsula",
    Landslides 5:127-142, 2008, dx.doi.org/10.1007/s10346-007-0102-3

SourceFiles
    Medina.C

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#ifndef Medina_H
#define Medina_H

#include "entrainmentModel.H"
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
namespace entrainmentModels
{

/*---------------------------------------------------------------------------*\
                           Class Erosionenergy Declaration
\*---------------------------------------------------------------------------*/

class Medina
:
    public entrainmentModel
{
    // Private data

        //- Cohesion of the basal material at bottom
        dimensionedScalar tauc_;

        //- Friction coefficient tan(phi) of the basal material
        dimensionedScalar mu_;

        scalar relaxationFactor_;

        //- Reference to the gravitation fields
        const areaVectorField& gs_;

        const areaScalarField& gn_;

public:

    //- Runtime type information
    TypeName("Medina");


    // Constructors

        //- Construct from components
        Medina
        (
            const dictionary& frictionProperties,
            const areaVectorField& Us,
            const areaScalarField& h,
            const areaScalarField& hentrain,
            const areaScalarField& pb,
            const areaVectorField& tau
        );


    //- Destructor
    virtual ~Medina() = default;


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
