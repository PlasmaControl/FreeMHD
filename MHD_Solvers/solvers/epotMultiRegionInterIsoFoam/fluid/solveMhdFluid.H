if (finalIter)
{
    mesh.data::add("finalIteration", true);
}


if (frozenFlow)
{   
    #include "contErr.H"
    volScalarField divUp("divUp", fvc::div(fvc::absolute(phi, U), p));
    #include "mhdTEqn.H"
    #include "ePotEqn.H"
}
else
{
    #include "alphaControls.H"
    #include "compressibleAlphaEqnSubCycle.H"

    turbulence.correctPhasePhi();

	//solve the electric potential equation for the fluid
	#include "ePotEqn.H"
    #include "mhdUEqn.H"
    volScalarField divUp("divUp", fvc::div(fvc::absolute(phi, U), p));
    #include "mhdTEqn.H" 

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
