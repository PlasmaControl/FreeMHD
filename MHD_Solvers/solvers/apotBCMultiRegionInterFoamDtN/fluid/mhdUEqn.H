
	MRF.correctBoundaryVelocity(U); 

	 tmp<fvVectorMatrix> tUEqn
	(
		fvm::ddt(rho, U) + fvm::div(rhoPhi, U)
	  - fvm::Sp(contErr, U)
	  + MRF.DDt(rho, U)
	  + turbulence.divDevRhoReff(U)
	  + fvm::Sp(elcond*magSqr(B), U)   // implicit Hartmann drag (Ohm's-law sigma|B|^2 U);
	                                   // compensated by +sigma|B|^2 U added to JxB in mhdAEqn.H
	 ==
		fvOptions(rho, U)
		// J x B enters as a Rhie-Chow face flux in pEqn.H (phig), not as a source here.
	);
	fvVectorMatrix& UEqn = tUEqn.ref();
	
	UEqn.relax();

	fvOptions.constrain(UEqn);

	if (momentumPredictor)
	{
		solve
		(
			UEqn
		 ==
			fvc::reconstruct
			(
				(
					mixture.surfaceTensionForce()
				  - ghf*fvc::snGrad(rho)
				  - fvc::snGrad(p_rgh)
				) * mesh.magSf()
			  + fvc::flux(JxB)   // consistent Lorentz face flux (matches pEqn.H phig)
			)
		);

		fvOptions.correct(U);

		K = 0.5*magSqr(U);
	}
