PtrList<volScalarField> solidPotE(solidRegions.size());
PtrList<volScalarField> solidElcond(solidRegions.size());
PtrList<dimensionedScalar> solidElcondValue(solidRegions.size());

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
                IOobject::AUTO_WRITE // NO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("zero", dimensionSet(-1, -3, 3, 0, 0, 2, 0), 0)
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
	
	solidElcond[i] = solidElcondValue[i];
	
    Info << "solid " << i << " solid elcond: " << solidElcondValue[i] << endl;
    Info << "Init solid " << i << " finished!" << endl;
}
