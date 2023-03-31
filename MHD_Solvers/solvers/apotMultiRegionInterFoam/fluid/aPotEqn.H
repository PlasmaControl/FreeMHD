magMu  = alpha1f * magMu1 + (1 - alpha1f) * magMu2;
    
fvVectorMatrix mvpotEqn
			(
				fvm::laplacian(A)
				==
				-magMu*J
			);

	mvpotEqn.solve();
	

