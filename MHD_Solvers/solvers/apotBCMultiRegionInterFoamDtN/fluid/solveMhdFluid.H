

if (finalIter)
{
	mesh.data::add("finalIteration", true);
}


if (frozenFlow)
{   
	// [Dbg] freeze dAdt once per time step, before the correctors, so the
	// restored potE source cannot feed back through the Picard loop.
	if (runTime.controlDict().getOrDefault<bool>("VSH_potEdAdt", false) && oCorr == 0)
	{
		dAdt = fvc::ddt(A);
		dAdt.correctBoundaryConditions();
	}
	// from compressibleAlphaEqnSubCycle
	#include "contErr.H"
	volScalarField divUp("divUp", fvc::div(fvc::absolute(phi, U), p));
	#include "mhdTEqn.H"
	// [Dbg] VSH_fullAPath: route frozen flow through the VALIDATED induction
	// path (mhdAEqn.H) instead of the separately-written ePotEqn/aPotEqn pair.
	// The two are not equivalent: the frozen pair has no inner Picard loop, no
	// relaxation, omits -sigma dA/dt from the potential source, never refreshes
	// curlA, and -- most importantly -- assembles the A-equation source at CELL
	// CENTRES (-grad(potE) + U^B) where mhdAEqn.H assembles it as a FACE FLUX and
	// reconstructs it. potE exists to cancel the irrotational part of U^B; with
	// mismatched stencils that cancellation is broken discretely, and the
	// remainder is exactly the EMF that drives the dynamo.
	if (runTime.controlDict().getOrDefault<bool>("VSH_fullAPath", false))
	{
		#include "mhdAEqn.H"
	}
	else
	{
		#include "ePotEqn.H"
		#include "aPotEqn.H"
	}
}
else
{
	#include "alphaControls.H" // which version should be used -> gerneal icAlpha!
	#include "compressibleAlphaEqnSubCycle.H"

	turbulence.correctPhasePhi();
	
	//solve the electric potential equation for the fluid
	//#include "ePotEqn.H"
	//#include "aPotEqn.H"
	#include "mhdAEqn.H"
	
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
