
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
	
	//update B
	B= B0 + fvc::curl(A);
	B.correctBoundaryConditions();
	
	
	//Computation of current density at cell faces
	surfaceScalarField jn = -(elcond * fvc::snGrad(potE) * mesh.magSf()) + elcond*psiub;

	//Current density at face center
	surfaceVectorField jnv = jn * mesh.Cf();

	//Interpolation of current density at cell center
	J = fvc::surfaceIntegrate(jnv) - (fvc::surfaceIntegrate(jn) * mesh.C());

	//Update current density distribution and boundary condition
	J.correctBoundaryConditions();
	
	bool JConservativeForm
	(
		runTime.controlDict().get<bool>("JConservativeForm")
	);
	
	//Lorentz force computation using the conservative form of JxB
	if(JConservativeForm)
	{
		// interpolate B to faces
		surfaceVectorField bf = fvc::interpolate(B);
		
		//Lorentz force 
		JxB=(-fvc::surfaceIntegrate(jn*bf^mesh.Cf()) + ( fvc::div( jn,B) ^ mesh.C() )); 
		
	}
	
	//Alternatively, just use previously computed cell-centered J from the conservative interoplation scheme
	else
	{
		//Lorentz force computation
		JxB = (J ^ B);
	}
