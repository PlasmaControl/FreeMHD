  fvScalarMatrix PotEEqn
	(
		fvm::laplacian(potE) 
		== 
		dimensionedScalar("zero",dimensionSet(1, 0, -3, 0, 0, -1, 0), 0)
	);
			
	//Solving Poisson equation
	PotEEqn.solve(); 
