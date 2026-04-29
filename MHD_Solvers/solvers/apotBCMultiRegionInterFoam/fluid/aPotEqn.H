magMu  = alpha1f * magMu1 + (1 - alpha1f) * magMu2;
    
fvVectorMatrix mvpotEqn
			(
				fvm::ddt(A)
				==
				-fvc::grad(potE)
				+(U^B)
				+1/elcond/magMu*fvm::laplacian(A)
			);

	mvpotEqn.solve();
	
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

