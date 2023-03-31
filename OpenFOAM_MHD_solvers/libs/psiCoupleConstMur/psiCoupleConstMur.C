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
 
 \*---------------------------------------------------------------------------*/

#include "psiCoupleConstMur.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "mappedPatchBase.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

psiCoupleConstMur::
psiCoupleConstMur
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(p, iF),
    psinbrName_("undefined-psinbr")
{
    this->refValue() = 0.0;
    this->refGrad() = 0.0;
    this->valueFraction() = 1.0;
}


psiCoupleConstMur::
psiCoupleConstMur
(
    const psiCoupleConstMur& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchScalarField(ptf, p, iF, mapper),
    psinbrName_(ptf.psinbrName_)
{}


psiCoupleConstMur::
psiCoupleConstMur
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchScalarField(p, iF),
    psinbrName_(dict.lookup("psinbr"))
{
    if (!isA<mappedPatchBase>(this->patch().patch()))
    {
        FatalErrorInFunction
            << "' not type '" << mappedPatchBase::typeName << "'"
            << "\n    for patch " << p.name()
            << " of field " << internalField().name()
            << " in file " << internalField().objectPath()
            << exit(FatalError);
    }


    fvPatchScalarField::operator=(scalarField("value", dict, p.size()));

    if (dict.found("refValue"))
    {
        // Full restart
        refValue() = scalarField("refValue", dict, p.size());
        refGrad() = scalarField("refGradient", dict, p.size());
        valueFraction() = scalarField("valueFraction", dict, p.size());
    }
    else
    {
        // Start from user entered data. Assume fixedValue.
        refValue() = *this;
        refGrad() = 0.0;
        valueFraction() = 1.0;
    }
}


psiCoupleConstMur::
psiCoupleConstMur
(
    const psiCoupleConstMur& wtcsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(wtcsf, iF),
    psinbrName_(wtcsf.psinbrName_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void psiCoupleConstMur::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    // Since we're inside initEvaluate/evaluate there might be processor
    // comms underway. Change the tag we use.
    int oldTag = UPstream::msgType();
    UPstream::msgType() = oldTag+1;

    // Get the coupling information from the mappedPatchBase
    const mappedPatchBase& mpp =
        refCast<const mappedPatchBase>(patch().patch());
    const polyMesh& nbrMesh = mpp.sampleMesh();
    const label samplePatchi = mpp.samplePolyPatch().index();
    const fvPatch& nbrPatch =
        refCast<const fvMesh>(nbrMesh).boundary()[samplePatchi];


    const psiCoupleConstMur& nbrField =
    refCast
    <
        const psiCoupleConstMur
    >
    (
        nbrPatch.lookupPatchField<volScalarField, scalar>
        (
            psinbrName_
        )
    );


    tmp<scalarField> nbrIntFld(new scalarField(nbrField.size(), 0.0));
    tmp<scalarField> nbrDelta(new scalarField(nbrField.size(), 0.0));

    nbrIntFld.ref() = nbrField.patchInternalField();
    nbrDelta.ref() =  nbrPatch.deltaCoeffs();

    mpp.distribute(nbrIntFld.ref());
    mpp.distribute(nbrDelta.ref());


// mur
    //const IOdictionary& transDict=nbrMesh.lookupObject<IOdictionary>("magneticProperties");
    //const dimensionedScalar& nbrMur=readScalar(transDict.lookup("mur"));
    //const scalar nbrMur_=nbrMur.value();
    
    //neighbor as scalarField
    //const volScalarField& Mur_nbrmesh = nbrMesh.lookupObject<volScalarField>("murf");  
    //scalarField nbrMurField = Mur_nbrmesh.boundaryField().boundaryInternalField()[samplePatchi];
    
	//neighbor as scalar
	const IOdictionary& nbrTransDict=nbrMesh.lookupObject<IOdictionary>("thermophysicalProperties");
	const scalar nbrMur_=readScalar(nbrTransDict.lookup("murf"));
	
	//self as scalarField
	//const fvMesh& mesh = patch().boundaryMesh().mesh();
    //const label id = patch().index();
	//const volScalarField& Amesh = mesh.lookupObject<volScalarField>("murf");
	//const scalarField& murField = Amesh.boundaryField().boundaryInternalField()[id];

	//self as scalar
	const IOdictionary& transDict=patch().boundaryMesh().mesh().lookupObject<IOdictionary>("thermophysicalProperties");
	const scalar mur_=readScalar(transDict.lookup("murf"));
	
    //const IOdictionary& nbrTransDict=patch().boundaryMesh().mesh().lookupObject<IOdictionary>("magneticProperties");
    //const dimensionedScalar& mur=readScalar(nbrTransDict.lookup("mur"));
    //const scalar mur_=mur.value();
    
    

//// Mrem

    //const vector nbrMrem=transDict.lookup("Mrem");
    //const vector nbrMrem_=nbrMrem;
    
	//neighbor (a little confusing since he mixes up neighbor and self, but we will stick to normal naming here
	
	//const IOdictionary& nbrTransDict=nbrMesh.lookupObject<IOdictionary>("thermophysicalProperties");
    const vector nbrMrem=nbrTransDict.lookup("Mrf");
	const vector nbrMrem_=nbrMrem;
	
	//self
	//const IOdictionary& transDict=patch().boundaryMesh().mesh().lookupObject<IOdictionary>("thermophysicalProperties");
	const vector Mrem=transDict.lookup("Mrf");
	const vector Mrem_=Mrem;
    //const vector Mrem=nbrTransDict.lookup("Mrem");
    //const vector Mrem_=Mrem;


	//Info<<"stage 1"<<nl;
    tmp<scalarField> murField(new scalarField(this->size(), mur_));
    tmp<scalarField> nbrMurField(new scalarField(nbrField.size(), nbrMur_));

    mpp.distribute(nbrMurField.ref());


    //this->valueFraction() = (nbrMurField*nbrDelta())/(murField*patch().deltaCoeffs()+nbrMurField*nbrDelta());
    this->valueFraction() = (nbrMurField()*nbrDelta())/(murField()*patch().deltaCoeffs()+nbrMurField()*nbrDelta());

    this->refValue() = nbrIntFld-((nbrMrem_-Mrem_) & patch().nf())  /(nbrMurField*nbrDelta);

    this->refGrad() = 0.0;

    mixedFvPatchScalarField::updateCoeffs();

    // Restore tag
    UPstream::msgType() = oldTag;

}


void psiCoupleConstMur::write
(
    Ostream& os
) const
{
    mixedFvPatchScalarField::write(os);
    os.writeKeyword("psinbr")<< psinbrName_
        << token::END_STATEMENT << nl;

}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField
(
    fvPatchScalarField,
    psiCoupleConstMur
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam


// ************************************************************************* //
