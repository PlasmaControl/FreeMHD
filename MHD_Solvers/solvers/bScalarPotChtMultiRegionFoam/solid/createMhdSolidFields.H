PtrList<volScalarField> solidPsi(solidRegions.size());
PtrList<volVectorField> solidB(solidRegions.size());
PtrList<volVectorField> solidH(solidRegions.size());

PtrList<surfaceVectorField> solidMrf(solidRegions.size());
PtrList<dimensionedVector> solidMrfValue(solidRegions.size());

PtrList<surfaceScalarField> solidMurf(solidRegions.size());
PtrList<dimensionedScalar> solidMurfValue(solidRegions.size());


forAll(solidRegions, i)
{
     #include "setRegionSolidFields.H" // some things prob missing

    Info<< "Initializing magnetic scalar potential for solid" << i << endl;
    solidPsi.set
    (
        i,
        new volScalarField
        (
            IOobject
            (
                "Psi",
                runTime.timeName(),
                solidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            solidRegions[i]
        )
    );
    
    

	Info<< "Initializing solid " << i << " Mrf\n" << endl;
    solidMrf.set
    (
        i,
        new surfaceVectorField
        (
            IOobject
            (
                "Mrf",
                runTime.timeName(),
                solidRegions[i],
                IOobject::NO_READ,
                IOobject::AUTO_WRITE // NO_WRITE
            ),
            solidRegions[i],
            dimensionedVector("zero",dimensionSet(0, -1, 0, 0, 0, 1, 0), vector(0, 0, 0))
        )
    );
    
    solidMrfValue.set
    (
        i,
        new dimensionedVector
        (
            "Mrf", 
            dimensionSet(0, -1, 0, 0, 0, 1, 0), 
            thermos[i]
        )
    );
    
    Info<< "Initializing solid " << i << " murf\n" << endl;
    solidMurf.set
    (
        i,
        new surfaceScalarField
        (
            IOobject
            (
                "murf",
                runTime.timeName(),
                solidRegions[i],
                IOobject::NO_READ,
                IOobject::AUTO_WRITE // NO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("one", dimless, 1.0)
        )
    );
    
    solidMurfValue.set
    (
        i,
        new dimensionedScalar
        (
            "murf", 
             dimless, 
            thermos[i]
        )
    );
	solidMrf[i] = solidMrfValue[i];
	solidMurf[i] = solidMurfValue[i];
    
            Info<< "Initializing magnetic field B for solid" << i << endl;
    solidB.set
    (
        i,
        new volVectorField
        (
            IOobject
            (
                "B",
                runTime.timeName(),
                solidRegions[i],
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            constant::electromagnetic::mu0
           *fvc::reconstruct(solidMurf[i]*fvc::snGrad(solidPsi[i])*solidRegions[i].magSf() + solidMurf[i]*(solidMrf[i]& solidRegions[i].Sf()))

        )
    );
        Info<< "Initializing magnetic field H for solid" << i << endl;
    solidH.set
    (
        i,
        new volVectorField
        (
            IOobject
            (
                "H",
                runTime.timeName(),
                solidRegions[i],
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
             fvc::reconstruct(fvc::snGrad(solidPsi[i])*solidRegions[i].magSf())

        )
    );
    
	
	
    Info << "solid " << i << " solid Mrf: " << solidMrfValue[i] << endl;
    Info << "solid " << i << " solid Murf: " << solidMurfValue[i] << endl;
    Info << "Init solid " << i << " finished!" << endl;
}
