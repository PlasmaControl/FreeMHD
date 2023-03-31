PtrList<volScalarField> fluidPsi(fluidRegions.size());
PtrList<volVectorField> fluidB(fluidRegions.size());
PtrList<volVectorField> fluidH(fluidRegions.size());

PtrList<surfaceVectorField> fluidMrf(fluidRegions.size());
PtrList<dimensionedVector> fluidMrfValue(fluidRegions.size());

PtrList<surfaceScalarField> fluidMurf(fluidRegions.size());
PtrList<dimensionedScalar> fluidMurfValue(fluidRegions.size());


forAll(fluidRegions, i)
{
     #include "setRegionFluidFields.H" 


    Info<< "Initializing magnetic scalar potential for fluid" << i << endl;
    fluidPsi.set
    (
        i,
        new volScalarField
        (
            IOobject
            (
                "Psi",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[i]
        )
    );
    
    

	Info<< "Initializing fluid " << i << " Mrf\n" << endl;
    fluidMrf.set
    (
        i,
        new surfaceVectorField
        (
            IOobject
            (
                "Mrf",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::NO_READ,
                IOobject::AUTO_WRITE // NO_WRITE
            ),
            fluidRegions[i],
            dimensionedVector("zero",dimensionSet(0, -1, 0, 0, 0, 1, 0), vector(0, 0, 0))
        )
    );
    
    fluidMrfValue.set
    (
        i,
        new dimensionedVector
        (
            "Mrf", 
            dimensionSet(0, -1, 0, 0, 0, 1, 0), 
            thermoFluid[i]
        )
    );
    
    Info<< "Initializing fluid " << i << " Murf\n" << endl;
    fluidMurf.set
    (
        i,
        new surfaceScalarField
        (
            IOobject
            (
                "murf",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::NO_READ,
                IOobject::AUTO_WRITE // NO_WRITE
            ),
            fluidRegions[i],
             dimensionedScalar("one", dimless, 1.0)
        )
    );
    
    fluidMurfValue.set
    (
        i,
        new dimensionedScalar
        (
            "murf", 
             dimless, 
            thermoFluid[i]
        )
    );
    
	fluidMrf[i] = fluidMrfValue[i];
	fluidMurf[i] = fluidMurfValue[i];
    
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
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            constant::electromagnetic::mu0
           *fvc::reconstruct(fluidMurf[i]*fvc::snGrad(fluidPsi[i])*fluidRegions[i].magSf() + fluidMurf[i]*(fluidMrf[i]& fluidRegions[i].Sf()))

        )
    );
        Info<< "Initializing magnetic field H for fluid" << i << endl;
    fluidH.set
    (
        i,
        new volVectorField
        (
            IOobject
            (
                "H",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
             fvc::reconstruct(fvc::snGrad(fluidPsi[i])*fluidRegions[i].magSf())

        )
    );
    
	
	
    Info << "fluid " << i << " fluid Mrf: " << fluidMrfValue[i] << endl;
    Info << "fluid " << i << " fluid Murf: " << fluidMurfValue[i] << endl;
    Info << "Init fluid " << i << " finished!" << endl;
}
