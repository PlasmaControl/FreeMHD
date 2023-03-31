PtrList<volScalarField> solidPotE(solidRegions.size());
PtrList<volVectorField> solidA(solidRegions.size()); 
PtrList<volVectorField> solidJ(solidRegions.size()); 

PtrList<volScalarField> solidElcond(solidRegions.size()); 
PtrList<volScalarField> solidMagMu(solidRegions.size());

PtrList<dimensionedScalar> solidElcondValue(solidRegions.size());
PtrList<dimensionedScalar> solidMagMuValue(solidRegions.size());

forAll(solidRegions, i)
{
     #include "setRegionSolidFields.H" 

    Info<< "Initializing electric potential for solid" << i << endl;
    solidPotE.set
    (
        i,
        new volScalarField
        (
            IOobject
            (
                "potE",
                runTime.timeName(),
                solidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            solidRegions[i]
        )
    );
   
   Info<< "Initializing magnetic vector potential A for solid" << i << endl;
    solidA.set
    (
        i,
        new volVectorField
        (
            IOobject
            (
                "A",
                runTime.timeName(),
                solidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            solidRegions[i]
        )
    );
    
    solidJ.set
    (
        i,
        new volVectorField 
        (
            IOobject
            (
                "J",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            mesh,
            dimensionedVector("zero", dimensionSet(0, -2, 0, 0, 0, 1, 0),Foam::vector(0,0,0))
        )
    );
    
    Info<< "Initializing solid " << i << " ElCond\n" << endl;
    solidElcond.set
    (
        i,
        new volScalarField
        (
            IOobject
            (
                "elcond",
                runTime.timeName(),
                solidRegions[i],
                IOobject::NO_READ,
                IOobject::NO_WRITE // AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("zero", dimensionSet(-1, -3, 3, 0, 0, 2, 0), 0)
        )
    );
    
        Info<< "Initializing solid " << i << " magMu\n" << endl;
    solidMagMu.set
    (
        i,
        new volScalarField
        (
            IOobject
            (
                "magMu",
                runTime.timeName(),
                solidRegions[i],
                IOobject::NO_READ,
                IOobject::NO_WRITE // AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("zero", dimensionSet(1, 1, -2, 0, 0, -2, 0), 0)
        )
    );
    
    solidElcondValue.set
    (
        i,
        new dimensionedScalar
        (
            "elcond", 
            dimensionSet(-1, -3, 3, 0, 0, 2, 0), 
            thermos[i]
        )
    );
    
    solidMagMuValue.set
    (
        i,
        new dimensionedScalar
        (
            "magMu", 
            dimensionSet(1, 1, -2, 0, 0, -2, 0), 
            thermos[i]
        )
    );
    
    
    
	
	//for now these values are constant but can be set as fields by reading dictionaries 
	solidElcond[i] = solidElcondValue[i];
	solidMagMu[i] = solidMagMuValue[i];
	
    Info << "solid " << i << " solid elcond: " << solidElcondValue[i] << endl;
    Info << "solid " << i << " solid magMu: " << solidMagMuValue[i] << endl;
    Info << "Init solid " << i << " finished!" << endl;
}
