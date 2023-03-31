

if (finalIter)
{
    mesh.data::add("finalIteration", true);
}


if (frozenFlow)
{   
    // from compressibleAlphaEqnSubCycle
    #include "contErr.H"
    volScalarField divUp("divUp", fvc::div(fvc::absolute(phi, U), p));
    #include "mhdTEqn.H"
	#include "ePotEqn.H"
	#include "aPotEqn.H"
}
else
{
    #include "alphaControls.H" // which version should be used -> gerneal icAlpha!
    #include "compressibleAlphaEqnSubCycle.H"

    turbulence.correctPhasePhi();
	
	//solve the electric potential equation for the fluid
	#include "ePotEqn.H"
	#include "aPotEqn.H"
	
    #include "mhdUEqn.H"
    volScalarField divUp("divUp", fvc::div(fvc::absolute(phi, U), p));
    #include "mhdTEqn.H" // EEqn would be needed to include radiation
	
	
    // --- Pressure corrector loop
    // --- PISO loop
    for (int corr=0; corr<nCorr; corr++)
    {
        #include "pEqn.H"
    }

    if((turbOnFinalIterOnly && finalIter) || !turbOnFinalIterOnly)
    {
        turbulence.correct();
    }
}

if (finalIter)
{
    mesh.data::remove("finalIteration");
}
