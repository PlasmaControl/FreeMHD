/*---------------------------------------------------------------------------*\
	Coupled mixed BC for the gauge-cleaning scalar potential psi.

	Enforces [psi] = 0, [d psi/dn] = 0 across region interfaces by
	computing Robin coefficients that yield the symmetric interface
	condition for a unit-conductivity Laplacian.

	Must sit on a mappedPatch (or mappedWallPatch) in the mesh.

	When useImplicit() is true (set in every constructor) the
	lduPrimitiveMeshAssembly promotes this patch's faces to interior
	faces of the global assembled matrix.
\*---------------------------------------------------------------------------*/

#ifndef coupledGaugePsiFvPatchScalarField_H
#define coupledGaugePsiFvPatchScalarField_H

#include "mixedFvPatchFields.H"
#include "mappedPatchBase.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
		Class coupledGaugePsiFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class coupledGaugePsiFvPatchScalarField
:
	public mixedFvPatchScalarField
{
	word psiNbrName_;

public:

	TypeName("coupledGaugePsi");

	// Constructors

		coupledGaugePsiFvPatchScalarField
		(
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&
		);

		coupledGaugePsiFvPatchScalarField
		(
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&,
			const dictionary&
		);

		coupledGaugePsiFvPatchScalarField
		(
			const coupledGaugePsiFvPatchScalarField&,
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&,
			const fvPatchFieldMapper&
		);

		coupledGaugePsiFvPatchScalarField
		(
			const coupledGaugePsiFvPatchScalarField&
		);

		coupledGaugePsiFvPatchScalarField
		(
			const coupledGaugePsiFvPatchScalarField&,
			const DimensionedField<scalar, volMesh>&
		);

		virtual tmp<fvPatchScalarField> clone() const
		{
			return tmp<fvPatchScalarField>
			(
				new coupledGaugePsiFvPatchScalarField(*this)
			);
		}

		virtual tmp<fvPatchScalarField> clone
		(
			const DimensionedField<scalar, volMesh>& iF
		) const
		{
			return tmp<fvPatchScalarField>
			(
				new coupledGaugePsiFvPatchScalarField(*this, iF)
			);
		}


	// Member Functions

		virtual void updateCoeffs();

		virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
