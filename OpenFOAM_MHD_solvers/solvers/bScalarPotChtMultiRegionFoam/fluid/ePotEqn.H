
	surfaceScalarField psiub = fvc::interpolate(U ^ B) & mesh.Sf();

		fvScalarMatrix PotEEqn
			(
				fvm::laplacian(potE) 
				== 
				fvc::div(psiub)
			);
			
	//Solving Poisson equation
	PotEEqn.setReference(potERefCell, potERefValue);
	PotEEqn.solve();
	
	//Computation of current density at cell faces
	surfaceScalarField jn = -(elcond * fvc::snGrad(potE) * mesh.magSf()) + elcond*psiub;

	//Current density at face center
	surfaceVectorField jnv = jn * mesh.Cf();

	//Interpolation of current density at cell center
	volVectorField jfinal = fvc::surfaceIntegrate(jnv) - (fvc::surfaceIntegrate(jn) * mesh.C());

	//Update current density distribution and boundary condition
	jfinal.correctBoundaryConditions();
	
	//Lorentz force computation
	JxB = (jfinal ^ B);
