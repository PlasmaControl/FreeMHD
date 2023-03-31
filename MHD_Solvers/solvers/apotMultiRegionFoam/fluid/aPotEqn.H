
fvVectorMatrix mvpotEqn
			(
				fvm::laplacian(A)
				==
				-magMu*J
			);
			
	//Solving Poisson equation
	mvpotEqn.solve();
	

