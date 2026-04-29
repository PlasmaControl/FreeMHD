// initialize fields
curlA = fvc::curl(A);
curlA.correctBoundaryConditions();

// solve for scalar and vector potentials
label nAEiter(
	runTime.controlDict().get<label>("nAEiter")
);
label nGaugeIter(
	runTime.controlDict().get<label>("nGaugeIter")
);
for (label iter = 0; iter < nAEiter; ++iter){
	dAdt = fvc::ddt(A);
	dAdt.correctBoundaryConditions();
	fvScalarMatrix PotEEqn(
		fvm::laplacian(elcond, potE)
		==
		fvc::div(fvc::interpolate(-elcond * dAdt) & mesh.Sf())
	);
	PotEEqn.setReference(potERefCell, potERefValue);
	PotEEqn.relax();
	scalar potERes = PotEEqn.solve().initialResidual();
	//potE -= dimensionedScalar("potEAvg", potE.dimensions(),gAverage(potE.primitiveField()));
	//potE.correctBoundaryConditions();

	divA  = fvc::div(A);
	divA.correctBoundaryConditions();
	fvVectorMatrix AEqn(
		fvm::ddt(elcond, A)
		- fvm::laplacian(1.0 / magMu, A)
		==
		- elcond * fvc::grad(potE)
		//- fvc::grad(divA) / magMu
	);
	AEqn.relax();
	scalar ARes = cmptMax(AEqn.solve().initialResidual());

	for (label gIter = 0; gIter < nGaugeIter; ++gIter){
		fvScalarMatrix PsiEqn
		(
			fvm::laplacian(psi)
			==
			fvc::div(A)
		);
		//PsiEqn.setReference(potERefCell, 0.0);
		PsiEqn.solve();
		//A -= fvc::grad(psi);
		A.correctBoundaryConditions();
	}

	curlA = fvc::curl(A);
	curlA.correctBoundaryConditions();

	if (max(potERes, ARes) < 1e-5){
		break;
	}
}

dAdt = fvc::ddt(A);
dAdt.correctBoundaryConditions();
divA  = fvc::div(A);
divA.correctBoundaryConditions();

surfaceScalarField elcondF = fvc::interpolate(elcond);
surfaceScalarField jn      = - elcondF * fvc::snGrad(potE) * mesh.magSf()
                             - (fvc::interpolate(elcond * dAdt) & mesh.Sf());
J = fvc::reconstruct(jn);
J.correctBoundaryConditions();
