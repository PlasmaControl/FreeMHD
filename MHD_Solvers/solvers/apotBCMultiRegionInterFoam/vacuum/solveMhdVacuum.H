fvMesh& mesh = vacuumRegions[i];
if (finalIter)
{
    mesh.data::add("finalIteration", true);
}

label nGaugeIter(
	runTime.controlDict().get<label>("nGaugeIter")
);
const int nABCiter(
	runTime.controlDict().getOrDefault("nABCiter", 10)
);
for (int j = 0; j < nABCiter; j++)
{
	fvVectorMatrix mvpotEqn
	(
		fvm::laplacian(A)
		==
		//fvc::grad(fvc::div(A))
		dimensionedVector("zero",dimensionSet(1, -1, -2, 0, 0, -1, 0), vector(0,0,0))
	);
	scalar Res = cmptMax(mvpotEqn.solve().initialResidual());

	for (label gIter = 0; gIter < nGaugeIter; ++gIter){
		fvScalarMatrix PsiEqn
		(
			fvm::laplacian(psi)
			==
			fvc::div(A)
		);
		PsiEqn.setReference(0, 0.0);
		PsiEqn.solve();
		//A -= fvc::grad(psi);
		A.correctBoundaryConditions();
	}

	if (mesh.name() == "vacuum_sphere")
	{
		#include "vacuumAExteriorEquation.H"
	}

	if (Res < 1e-5){
		break;
	}
}

B = fvc::curl(A);
B.correctBoundaryConditions();

if (finalIter)
{
    mesh.data::remove("finalIteration");
}
