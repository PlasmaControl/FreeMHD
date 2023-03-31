PtrList<volVectorField> fluidB(fluidRegions.size());
PtrList<volVectorField> fluidB0(fluidRegions.size());
PtrList<volVectorField> fluidJ(fluidRegions.size());
PtrList<volVectorField> fluidA(fluidRegions.size());
PtrList<volVectorField> fluidJxB(fluidRegions.size());
PtrList<volScalarField> fluidPotE(fluidRegions.size());

PtrList<dimensionedScalar> fluidElCondValue(fluidRegions.size());
PtrList<volScalarField> fluidElCond(fluidRegions.size());

PtrList<dimensionedScalar> fluidMagMuValue(fluidRegions.size());
PtrList<volScalarField> fluidMagMu(fluidRegions.size());

List<label> potERefCellFluid(fluidRegions.size());
List<scalar> potERefValueFluid(fluidRegions.size());

forAll(fluidRegions, i)
{
     #include "setRegionFluidFields.H" 

    Info<< "Initializing magnetic field B for fluid" << i << endl;
    fluidB.set
    (
        i,
        new volVectorField
        (
            IOobject
            (
                "B",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[i]
        )
    );
    
            fluidB0.set
    (
        i,
        new volVectorField
        (
            IOobject
            (
                "B0",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[i]
        )
    );
    
        Info<< "Initializing magnetic vector potential A for fluid" << i << endl;
        
    fluidA.set
    (
        i,
        new volVectorField
        (
            IOobject
            (
                "A",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[i]
        )
    );
    
        Info<< "Initializing electric potential for fluid" << i << endl;
    fluidPotE.set
    (
        i,
        new volScalarField
        (
            IOobject
            (
                "potE",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[i]
        )
    );
    
    	Info<< "Initializing current density J for fluid" << i << endl;
    fluidJ.set
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


    Info<< "Initializing field JxB for fluid" << i << "\n" << endl;
    fluidJxB.set
    (
        i,
        new volVectorField 
        (
            IOobject
            (
                "JxB",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            mesh,
            dimensionedVector("zero", dimensionSet(1, -2, -2, 0, 0, 0, 0),Foam::vector(0,0,0))
        )
    );

       Info<< "Initializing fluid " << i << " field ElCond\n" << endl;
    fluidElCond.set
    (
        i,
        new volScalarField
        (
            IOobject
            (
                "elcond",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE // NO_WRITE
            ),
            mesh,
            dimensionedScalar("zero", dimensionSet(-1, -3, 3, 0, 0, 2, 0), 0)
        )
    );

    fluidElCondValue.set
    (
        i,
        new dimensionedScalar
        (
            "elcond", 
            dimensionSet(-1, -3, 3, 0, 0, 2, 0), 
            thermoFluid[i]
        )
    );
    
    //read magnetic permeabilities
    fluidMagMuValue.set
    (
        i,
        new dimensionedScalar
        (
            "magMu", 
            dimensionSet(1, 1, -2, 0, 0, -2, 0), 
            thermoFluid[i]
        )
    );
    
        fluidMagMu.set
    (
        i,
        new volScalarField
        (
            IOobject
            (
                "magMu",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE // NO_WRITE
            ),
            mesh,
            dimensionedScalar("zero", dimensionSet(1, 1, -2, 0, 0, -2, 0), 0)
        )
    );

	
	potERefCellFluid[i] = 0;
    potERefValueFluid[i] = 0.0;
    
	setRefCell(fluidPotE[i], mesh.solutionDict().subDict("potE"), potERefCellFluid[i], potERefValueFluid[i]);


//constant fluid properties
fluidElCond[i] = fluidElCondValue[i];
fluidMagMu[i] = fluidMagMuValue[i];

    Info << "Init fluid " << i << " finished!" << endl;
}
