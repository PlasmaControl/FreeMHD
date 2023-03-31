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
    Foam::entrainmentModels::Front

Description
    An entrainment model following the simple front entrainment approach.
    All mass will be entrained if the flow height of the avalanche
    exceeds the threshold "htrigger".

SourceFiles
    Front.C

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#ifndef Front_H
#define Front_H

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
                           Class Front Declaration
\*---------------------------------------------------------------------------*/

class Front
:
    public entrainmentModel
{
    // Private data

        //- Height threshold
        dimensionedScalar htrigger_;


public:

    //- Runtime type information
    TypeName("Front");


    // Constructors

        //- construct from components
        Front
        (
            const dictionary& frictionProperties,
            const areaVectorField& Us,
            const areaScalarField& h,
            const areaScalarField& hentrain,
            const areaScalarField& pb,
            const areaVectorField& tau
        );


    //- Destructor
    virtual ~Front() = default;


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
