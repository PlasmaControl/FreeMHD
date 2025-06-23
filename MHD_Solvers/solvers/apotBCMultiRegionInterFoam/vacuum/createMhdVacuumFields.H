PtrList<volVectorField> vacuumA(vacuumRegions.size()); 
//PtrList<volVectorField> vacuum_a_lm_Re(vacuumRegions.size()); 

forAll(vacuumRegions, i)
{
   Info<< "Initializing magnetic vector potential A for vacuum" << i << endl;
    vacuumA.set
    (
        i,
        new volVectorField
        (
            IOobject
            (
                "A",
                runTime.timeName(),
                vacuumRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            vacuumRegions[i]
        )
    );
    
    Info << "Init vacuum " << i << " finished!" << endl;

        /*
    vacuum_a_lm_Re.set
    (
        i,
        new volVectorField
        (
            IOobject
            (
                "elcond",
                runTime.timeName(),
                vacuumRegions[i],
                IOobject::NO_READ,
                IOobject::AUTO_WRITE // NO_WRITE
            ),
            vacuumRegions[i],
            dimensionedVector(Foam::vector(0,0,0))
        )
    );
    */
}
