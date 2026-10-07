/*---------------------------------------------------------------------------*\
	Coupled mixed BC for global gauge-cleaning scalar potential psi.

	Symmetric Robin coefficients for a unit-conductivity Laplacian:
		valueFraction = delta_nbr / (delta_nbr + delta_my)
		refValue      = psi_nbr internal cell
		refGrad       = 0
		source        = 0

	Setting useImplicit(true) instructs lduPrimitiveMeshAssembly to
	promote this patch's faces to interior faces when assembling the
	global multi-region matrix.
\*---------------------------------------------------------------------------*/

#include "coupledGaugePsiFvPatchScalarField.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "mappedPatchBase.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private helpers * * * * * * * * * * * * * * * * //

static void initMixed(mixedFvPatchScalarField& f)
{
	f.refValue()      = scalar(0);
	f.refGrad()       = scalar(0);
	f.valueFraction() = scalar(0.5);
	f.source()        = scalar(0);
}


// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

coupledGaugePsiFvPatchScalarField::coupledGaugePsiFvPatchScalarField
(
	const fvPatch& p,
	const DimensionedField<scalar, volMesh>& iF
)
:
	mixedFvPatchScalarField(p, iF),
	psiNbrName_("psi")
{
	initMixed(*this);
	this->useImplicit(true);
}


coupledGaugePsiFvPatchScalarField::coupledGaugePsiFvPatchScalarField
(
	const fvPatch& p,
	const DimensionedField<scalar, volMesh>& iF,
	const dictionary& dict
)
:
	mixedFvPatchScalarField(p, iF),
	psiNbrName_(dict.getOrDefault<word>("psiNbr", "psi"))
{
	if (!isA<mappedPatchBase>(p.patch()))
	{
		FatalErrorInFunction
			<< "Patch " << p.name() << " is not a mappedPatch."
			<< " coupledGaugePsi requires a mapped(Wall)Patch."
			<< exit(FatalError);
	}

	initMixed(*this);
	this->useImplicit(true);

	if (dict.found("value"))
	{
		fvPatchScalarField::operator=
		(
			scalarField("value", dict, p.size())
		);
	}
	else
	{
		fvPatchScalarField::operator=(refValue());
	}
}


coupledGaugePsiFvPatchScalarField::coupledGaugePsiFvPatchScalarField
(
	const coupledGaugePsiFvPatchScalarField& ptf,
	const fvPatch& p,
	const DimensionedField<scalar, volMesh>& iF,
	const fvPatchFieldMapper& mapper
)
:
	mixedFvPatchScalarField(ptf, p, iF, mapper),
	psiNbrName_(ptf.psiNbrName_)
{
	this->useImplicit(true);
}


coupledGaugePsiFvPatchScalarField::coupledGaugePsiFvPatchScalarField
(
	const coupledGaugePsiFvPatchScalarField& ptf
)
:
	mixedFvPatchScalarField(ptf),
	psiNbrName_(ptf.psiNbrName_)
{
	this->useImplicit(true);
}


coupledGaugePsiFvPatchScalarField::coupledGaugePsiFvPatchScalarField
(
	const coupledGaugePsiFvPatchScalarField& ptf,
	const DimensionedField<scalar, volMesh>& iF
)
:
	mixedFvPatchScalarField(ptf, iF),
	psiNbrName_(ptf.psiNbrName_)
{
	this->useImplicit(true);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void coupledGaugePsiFvPatchScalarField::updateCoeffs()
{
	if (this->updated())
	{
		return;
	}

	// Fetch the mappedPatchBase to reach the neighbour region/patch
	const mappedPatchBase& mpp =
		refCast<const mappedPatchBase>(patch().patch());

	const fvMesh& nbrMesh =
		refCast<const fvMesh>(mpp.sampleMesh());

	const label nbrPatchI = mpp.samplePolyPatch().index();

	const fvPatch& nbrPatch = nbrMesh.boundary()[nbrPatchI];

	// Internal-cell psi on the neighbour side
	const scalarField& psiNbrInternal =
		nbrMesh.lookupObject<volScalarField>(psiNbrName_).primitiveField();

	// Face-cell labels on neighbour patch
	const labelUList& nbrFaceCells = nbrPatch.faceCells();

	scalarField psiNbrFace(nbrFaceCells.size());
	forAll(nbrFaceCells, facei)
	{
		psiNbrFace[facei] = psiNbrInternal[nbrFaceCells[facei]];
	}

	// Distribute from neighbour to owner layout
	mpp.distribute(psiNbrFace);

	// Inverse-distance weights (unit conductivity on both sides)
	const scalarField& myDelta   = patch().deltaCoeffs();   // 1/d_my
	const scalarField& nbrDeltaC = nbrPatch.deltaCoeffs();  // 1/d_nbr before distribute

	// We need neighbour delta in the owner face layout as well
	scalarField nbrDelta(nbrDeltaC);
	mpp.distribute(nbrDelta);

	// Robin: weight by inverse distance so continuity of flux is exact
	// valueFraction = delta_nbr / (delta_my + delta_nbr)
	valueFraction() = nbrDelta / (myDelta + nbrDelta);
	refValue()      = psiNbrFace;
	refGrad()       = scalar(0);
	source()        = scalar(0);

	mixedFvPatchScalarField::updateCoeffs();
}


void coupledGaugePsiFvPatchScalarField::write(Ostream& os) const
{
	mixedFvPatchScalarField::write(os);
	os.writeEntryIfDifferent<word>("psiNbr", "psi", psiNbrName_);
	writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField
(
	fvPatchScalarField,
	coupledGaugePsiFvPatchScalarField
);

} // End namespace Foam

// ************************************************************************* //
