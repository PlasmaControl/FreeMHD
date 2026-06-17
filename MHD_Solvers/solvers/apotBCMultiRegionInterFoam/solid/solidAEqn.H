// initialize fields
curlA = fvc::curl(A);
curlA.correctBoundaryConditions();

// solve for scalar and vector potentials
label nAEiter(
	runTime.controlDict().get<label>("nAEiter")
);
dAdt = fvc::ddt(A);
dAdt.correctBoundaryConditions();
for (label iter = 0; iter < nAEiter; ++iter){
	fvScalarMatrix PotEEqn(
		fvm::laplacian(elcond, potE)
		//==
		//fvc::div(fvc::interpolate(-elcond * dAdt) & mesh.Sf())
	);
	PotEEqn.setReference(potERefCell, potERefValue);
	PotEEqn.relax();
	scalar potERes = PotEEqn.solve().initialResidual();

	surfaceScalarField elcondS = fvc::interpolate(elcond);
	surfaceScalarField sAf = - elcondS * fvc::snGrad(potE) * mesh.magSf();
	volVectorField sA(fvc::reconstruct(sAf));
	fvVectorMatrix AEqn(
		fvm::ddt(elcond, A)
		- fvm::laplacian(1.0 / magMu, A)
		==
		sA
	);
	AEqn.relax();
	scalar ARes = cmptMax(AEqn.solve().initialResidual());

	curlA = fvc::curl(A);
	curlA.correctBoundaryConditions();

	if (max(potERes, ARes) < 1e-3){
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
