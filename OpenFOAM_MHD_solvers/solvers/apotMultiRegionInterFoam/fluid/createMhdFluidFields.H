//MHD fields
PtrList<volVectorField> fluidB(fluidRegions.size());
PtrList<volVectorField> fluidB0(fluidRegions.size());
PtrList<volVectorField> fluidJ(fluidRegions.size());
PtrList<volVectorField> fluidA(fluidRegions.size());
PtrList<volVectorField> fluidJxB(fluidRegions.size());
PtrList<volScalarField> fluidPotE(fluidRegions.size());

PtrList<volScalarField>  fluidAlpha1f(fluidRegions.size());

PtrList<dimensionedScalar> fluidElCond1(fluidRegions.size());
PtrList<dimensionedScalar> fluidElCond2(fluidRegions.size());
PtrList<volScalarField> fluidElCond(fluidRegions.size());

PtrList<surfaceScalarField> fluidElcondF(fluidRegions.size());
PtrList<surfaceScalarField> fluidPsiub(fluidRegions.size());
PtrList<surfaceScalarField> fluidJn(fluidRegions.size());

PtrList<dimensionedScalar> fluidMagMu1(fluidRegions.size());
PtrList<dimensionedScalar> fluidMagMu2(fluidRegions.size());
PtrList<volScalarField> fluidMagMu(fluidRegions.size());


List<label> potERefCellFluid(fluidRegions.size());
List<scalar> potERefValueFluid(fluidRegions.size());

forAll(fluidRegions, i)
{
     #include "setRegionFluidFields.H"


    Info<< "Initializing magnetic fields B and B0 for fluid" << i << endl;
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


    Info<< "Initializing fields J and JxB for fluid" << i << "\n" << endl;
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
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[i]
        )
    );


//read electric conductivities
    fluidElCond1.set
    (
        i,
        new dimensionedScalar
        (
            "elcond", 
            dimensionSet(-1, -3, 3, 0, 0, 2, 0), 
            mixture.thermo1()
        )
    );

    Info << "fluid " << i << " mixture.thermo1 elcond: " << fluidElCond1[i] << endl;

    fluidElCond2.set
    (
        i,
        new dimensionedScalar
        (
            "elcond", 
            dimensionSet(-1, -3, 3, 0, 0, 2, 0), 
            mixture.thermo2()
        )
    );

    Info << "fluid " << i << " mixture.thermo2 elcond: " << fluidElCond2[i] << endl;

Info<< "Initializing fluid " << i << " temporary MHD fields elcondF, psiub, jn\n" << endl;
    
    fluidElcondF.set
    (
        i,
        new surfaceScalarField
        (
            "elcondF",
            fvc::interpolate(fluidElCond[i])
        )
    );
	
	fluidPsiub.set
    (
        i,
        new surfaceScalarField
        (
            "psiub",
            fvc::interpolate((fluidElCond[i]* UFluid[i]) ^ fluidB[i]) & fluidRegions[i].Sf()
        )
    );
    
	fluidJn.set
    (
        i,
        new surfaceScalarField
        (
            "jn",
            -(fluidElcondF[i] * fvc::snGrad(fluidPotE[i]) * fluidRegions[i].magSf()) + fluidPsiub[i]
        )
    );

//read magnetic permeabilities
    fluidMagMu1.set
    (
        i,
        new dimensionedScalar
        (
            "magMu", 
            dimensionSet(1, 1, -2, 0, 0, -2, 0), 
            mixture.thermo1()
        )
    );

    Info << "fluid " << i << " mixture.thermo1 magnetic permeability: " << fluidMagMu1[i] << endl;

    fluidMagMu2.set
    (
        i,
        new dimensionedScalar
        (
            "magMu", 
            dimensionSet(1, 1, -2, 0, 0, -2, 0), 
            mixture.thermo2()
        )
    );

    Info << "fluid " << i << " mixture.thermo2 magnetic permeability: " << fluidMagMu2[i] << endl;

    
    
    Info<< "Initializing field alpha1f\n" << endl;
    fluidAlpha1f.set
    (
        i,
        new volScalarField
        (
            IOobject
            (
                "alpha1f",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            alpha1
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
    
        Info<< "Initializing fluid " << i << " field MagMu\n" << endl;
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

    Info << "Max alpha1 init " << gMax(alpha1) << endl;
    Info << "Min alpha1 init " << gMin(alpha1) << endl;

    volScalarField& alpha1f = fluidAlpha1f[i];
    alpha1f = min(max(alpha1, scalar(0)), scalar(1));

    Info << "Max alpha1f init " << gMax(alpha1f) << endl;
    Info << "Min alpha1f init " << gMin(alpha1f) << endl;

    fluidElCond[i] = alpha1f *  fluidElCond1[i] + (1 - alpha1f) * fluidElCond2[i];
    fluidElCond[i] = max(fluidElCond[i], dimensionedScalar("zero",dimensionSet(-1, -3, 3, 0, 0, 2, 0), 0));

    Info << "Max elcond init " << gMax(fluidElCond[i]) << endl;
    Info << "Min elcond init " << gMin(fluidElCond[i]) << endl;
    

    Info << "Init fluid " << i << " finished!" << endl;
}
