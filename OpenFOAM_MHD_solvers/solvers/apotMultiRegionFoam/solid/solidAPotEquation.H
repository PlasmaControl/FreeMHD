//Computation of current density at cell faces
	surfaceScalarField elcondF ("elcondF",fvc::interpolate(elcond));
	surfaceScalarField jn ("jn", -(elcondF * fvc::snGrad(potE) * mesh.magSf()));

	//Current density at face center
	surfaceVectorField jnv ("jnv", jn * mesh.Cf());

	//Interpolation of current density at cell center
	J = fvc::surfaceIntegrate(jnv) - (fvc::surfaceIntegrate(jn) * mesh.C());
	J.correctBoundaryConditions();

	fvVectorMatrix mvpotEqn
			(
				fvm::laplacian(A)
				==
				-magMu*J
			);
					
	//Solving Poisson equation
	mvpotEqn.solve();
