//Computation of current density at cell faces
	surfaceScalarField elcondF = fvc::interpolate(elcond);
	surfaceScalarField jn = -(elcondF * fvc::snGrad(potE) * mesh.magSf());

	//Current density at face center
	surfaceVectorField jnv = jn * mesh.Cf();

	//Interpolation of current density at cell center
	J = fvc::surfaceIntegrate(jnv) - (fvc::surfaceIntegrate(jn) * mesh.C());


	fvVectorMatrix mvpotEqn
			(
				fvm::ddt(A)
				==
				-fvc::grad(potE)
				+1/elcond/magMu*fvm::laplacian(A)
			);

					
	//Solving Poisson equation
	mvpotEqn.solve();
	A.correctBoundaryConditions();

	J = -fvc::laplacian(A)/magMu;
	// assuming curl of B_ext is zero
	J.correctBoundaryConditions();
