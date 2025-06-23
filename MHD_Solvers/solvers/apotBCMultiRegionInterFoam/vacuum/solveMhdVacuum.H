fvMesh& mesh = vacuumRegions[i];
if (finalIter)
{
    mesh.data::add("finalIteration", true);
}

if (mesh.name() == "vacuum_sphere")
{
for (int j = 0; j < 10; j++)
{
    // solve the apot equation in the vacuum

	fvVectorMatrix mvpotEqn
			(
				fvm::laplacian(A)
				==
				dimensionedVector("zero",dimensionSet(1, -1, -2, 0, 0, -1, 0), vector(0,0,0))
			);
					
	//Solving Poisson equation
	mvpotEqn.solve();

	#include "vacuumAExteriorEquation.H"

}
}
else 
{
	fvVectorMatrix mvpotEqn
			(
				fvm::laplacian(A)
				==
				dimensionedVector("zero",dimensionSet(1, -1, -2, 0, 0, -1, 0), vector(0,0,0))
			);
	//Solving Poisson equation
	mvpotEqn.solve();

}

if (finalIter)
{
    mesh.data::remove("finalIteration");
}
