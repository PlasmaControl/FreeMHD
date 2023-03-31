solve(

			fvm::laplacian(murf, Psi) 
			+ 
			fvc::div(murf* (Mrf & mesh.Sf()))
		);
		
H = fvc::reconstruct(fvc::snGrad(Psi)*mesh.magSf());
B = constant::electromagnetic::mu0*fvc::reconstruct(murf*fvc::snGrad(Psi)*mesh.magSf() + (murf* (Mrf & mesh.Sf())));

