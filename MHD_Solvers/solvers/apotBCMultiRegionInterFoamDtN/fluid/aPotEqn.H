magMu  = alpha1f * magMu1 + (1 - alpha1f) * magMu2;
	
// [Dbg] VSH_conductorDtN: apply the spherical-harmonic exterior operator directly
// on a conductor patch named vacuumExterior (no meshed vacuum).  The operator
// fixes dA/dn from the patch values, so it is iterated with the A solve exactly
// as in the vacuum region (nABCiter, VSH_abcTol).  Off by default.
{
	const bool condDtN =
		runTime.controlDict().getOrDefault<bool>("VSH_conductorDtN", false)
	 && mesh.boundary().findPatchID("vacuumExterior") >= 0;
	const label nDtN =
		condDtN ? runTime.controlDict().getOrDefault<label>("nABCiter", 10) : 1;
	const scalar dtnTol =
		runTime.controlDict().getOrDefault<scalar>("VSH_abcTol", 1e-3);
	for (label kDtN = 0; kDtN < nDtN; ++kDtN)
	{
		fvVectorMatrix mvpotEqn
			(
				fvm::ddt(A)
				==
				-fvc::grad(potE)
				+(U^B)
				+1/elcond/magMu*fvm::laplacian(A)
			);

		const scalar resDtN = cmptMax(mvpotEqn.solve().initialResidual());
		if (!condDtN) { break; }
		{
			#include "vacuumAExteriorEquation.H"
		}
		if (resDtN < dtnTol) { break; }
	}
}
	
	B= B0*scale + fvc::curl(A);
	B.correctBoundaryConditions();
	A.correctBoundaryConditions();
	J = fvc::curl(B)/magMu;

	volScalarField numerator = mag(fvc::curl(U^B));
	volScalarField denominator = mag(fvc::laplacian(B)/elcond/magMu);
	dimensionedScalar epsilon("epsilon", denominator.dimensions(), 1e-10);
	Rm = (numerator) / (denominator + epsilon);
	Rm_ind = numerator;
	Rm_dif = denominator;

	//Update current density distribution and boundary condition
	J.correctBoundaryConditions();
	
	//Lorentz force computation using the conservative form of JxB
	if (JConservativeForm)
	{
		// interpolate B to faces
		surfaceVectorField bf = fvc::interpolate(B);
		
		//Lorentz force 
		JxB=(-fvc::surfaceIntegrate(jn*bf^mesh.Cf()) + ( fvc::div( jn,B) ^ mesh.C() )); 
	}
	
	//Alternatively, just use previously computed cell-centered J from the conservative interoplation scheme
	else
	{
		//Lorentz force 
		JxB = (J ^ B);
	}

