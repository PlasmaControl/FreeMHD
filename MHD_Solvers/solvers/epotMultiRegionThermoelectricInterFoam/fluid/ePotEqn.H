    alpha1f = min(max(alpha1, scalar(0)), scalar(1));
    elcond = alpha1f * elcond1 + (1 - alpha1f) * elcond2;
	
	scalar BtStartTime
	(
		runTime.controlDict().get<scalar>("BtStartTime")
	);

	scalar BtDuration
	(
		runTime.controlDict().get<scalar>("BtDuration")
	);
	
	scalar scale = max(min((runTime.value() - BtStartTime)/(BtDuration+1e-6), 1.0), 0.0);
	Info<<"Current scale multiplied by B0: "<<scale<<endl;
	B=B0*scale;
    
	psiub = fvc::interpolate((elcond* U) ^ B) & mesh.Sf();
	
	fvScalarMatrix PotEEqn
		(
			fvm::laplacian(elcond,potE) 
			== 
			fvc::div(psiub)
		);
	bool includeThermoelectricEffect
		(
		runTime.controlDict().get<bool>("includeThermoelectricEffect")
		);
		
	if (includeThermoelectricEffect)
	{
		seebcoeff = alpha1f * seebcoeff1 + (1 - alpha1f) * seebcoeff2 ;
		psid2t = ( fvc :: interpolate(-seebcoeff * elcond * fvc::grad(T) )) & mesh.Sf();
		
		fvScalarMatrix PotEEqn
		(
			fvm::laplacian(elcond,potE)
			==
			fvc::div(psiub)
			+ fvc :: div(psid2t)
		);
	}

	//Solving Poisson equation
	PotEEqn.setReference(potERefCell, potERefValue);
	PotEEqn.solve();
	
	//Computation of current density at cell faces
	elcondF = fvc::interpolate(elcond);
	
	if (includeThermoelectricEffect)
	{
		jn = -(elcondF * fvc::snGrad(potE) * mesh.magSf()) + psiub + psid2t;
	}
	else
	{
		jn = -(elcondF * fvc::snGrad(potE) * mesh.magSf()) + psiub;
	}

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
