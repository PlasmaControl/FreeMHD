    alpha1f = min(max(alpha1, scalar(0)), scalar(1));
    elcond = alpha1f * elcond1 + (1 - alpha1f) * elcond2;

    B= B0 + fvc::curl(A);
    B.correctBoundaryConditions();
    
	psiub = fvc::interpolate((elcond* U) ^ B) & mesh.Sf();
	
	fvScalarMatrix PotEEqn
		(
			fvm::laplacian(elcond,potE) 
			== 
			fvc::div(psiub)
		);
			
	//Solving Poisson equation
	PotEEqn.setReference(potERefCell, potERefValue);
	PotEEqn.solve();
	
	//Computation of current density at cell faces
	elcondF = fvc::interpolate(elcond);
	jn = -(elcondF * fvc::snGrad(potE) * mesh.magSf()) + psiub;

	//Interpolation of current density at cell centers
	J= fvc::surfaceIntegrate((jn * mesh.Cf())) - (fvc::surfaceIntegrate(jn) * mesh.C());

	//Update current density distribution and boundary condition
	J.correctBoundaryConditions();
	
	bool JConservativeForm
	(
		runTime.controlDict().get<bool>("JConservativeForm")
	);
	
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

