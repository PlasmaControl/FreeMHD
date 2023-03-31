 /*---------------------------------------------------------------------------*\
   =========                 |
   \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
    \\    /   O peration     | 
     \\  /    A nd           | Copyright (C) 2020 Ibai Mugica
      \\/     M anipulation  |
 -------------------------------------------------------------------------------
 License
     This file is derivative work of OpenFOAM.
 
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
     Foam::psiCoupleConstMur
 
 Description
     Boundary condition that links two magnetic regions with a constant
     magnetic permeability. This is achieved by conserving magnetic fluxes
     through the boundary (B=Bnbr).
 
 SourceFiles
     psiCoupleConstMur.H
     psiCoupleConstMur.C
 
 \*---------------------------------------------------------------------------*/

#ifndef psiCoupleConstMur_H
#define psiCoupleConstMur_H

#include "mixedFvPatchFields.H"
#include "scalarField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
   Class psiCoupleConstMur Declaration
\*---------------------------------------------------------------------------*/

class psiCoupleConstMur
:
    public mixedFvPatchScalarField
{
    // Private data

        //- Name of field on the neighbour region
        const word psinbrName_;

public:

    //- Runtime type information
    TypeName("psiCoupleConstMur");


    // Constructors

        //- Construct from patch and internal field
        psiCoupleConstMur
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        psiCoupleConstMur
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  turbulentTemperatureCoupledBaffleMixedFvPatchScalarField onto a
        //  new patch
        psiCoupleConstMur
        (
            const psiCoupleConstMur&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new psiCoupleConstMur
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        psiCoupleConstMur
        (
            const psiCoupleConstMur&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchScalarField> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return tmp<fvPatchScalarField>
            (
                new psiCoupleConstMur
                (
                    *this,
                    iF
                )
            );
        }


    // Member functions

        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
