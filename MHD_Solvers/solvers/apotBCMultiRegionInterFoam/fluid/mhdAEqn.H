// multi-phase material property calculation
alpha1f = min(max(alpha1, scalar(0)), scalar(1));
elcond  = alpha1f * elcond1 + (1.0 - alpha1f) * elcond2;
magMu   = alpha1f * magMu1  + (1.0 - alpha1f) * magMu2;

// external magnetic field ramp-up
scalar BtStartTime(
	runTime.controlDict().get<scalar>("BtStartTime")
);
scalar BtDuration(
	runTime.controlDict().get<scalar>("BtDuration")
);
BtDuration = max(BtDuration, 1e-6);
const scalar Bscale = max(min((runTime.value() - BtStartTime) / BtDuration, 1.0), 0.0);
Info << "Current scale multiplied by B0: " << Bscale << endl;
curlA = fvc::curl(A);
curlA.correctBoundaryConditions();
B     = B0 * Bscale + curlA;
B.correctBoundaryConditions();

// solve for scalar and vector potentials
label nAEiter(
	runTime.controlDict().get<label>("nAEiter")
);

// Freeze dAdt at the start of the inner Picard loop. Recomputing it each
// iteration with dt~1e-6 amplifies any A change by 1/dt into the potE
// source, which drives the iteration into a stable limit cycle instead of
// converging. Holding dAdt fixed across the loop breaks that feedback.
dAdt  = fvc::ddt(A);
dAdt.correctBoundaryConditions();

for (label iter = 0; iter < nAEiter; ++iter){
	psiub = fvc::interpolate(elcond * ((U ^ B) - dAdt)) & mesh.Sf();
	fvScalarMatrix PotEEqn(
		fvm::laplacian(elcond, potE)
		==
		fvc::div(psiub)
	);
	PotEEqn.setReference(potERefCell, potERefValue);
	PotEEqn.relax();
	scalar potERes = PotEEqn.solve().initialResidual();
	dpotE = fvc::grad(potE);
	dpotE.correctBoundaryConditions();

	surfaceScalarField sAf
	(
		- elcondF * fvc::snGrad(potE) * mesh.magSf()
		+ (fvc::interpolate(elcond * (U ^ B)) & mesh.Sf())
	);
	volVectorField sA(fvc::reconstruct(sAf));
	fvVectorMatrix AEqn(
		fvm::ddt(elcond, A)
		- fvm::laplacian(1.0 / magMu, A)
		==
		sA
	);
	AEqn.relax();
	scalar ARes = cmptMax(AEqn.solve().initialResidual());

	divA = fvc::div(A);
	divA.correctBoundaryConditions();
	curlA = fvc::curl(A);
	curlA.correctBoundaryConditions();
	B     = B0 * Bscale + curlA;
	B.correctBoundaryConditions();

	if (max(potERes, ARes) < 1e-3){
		break;
	}
}

dAdt = fvc::ddt(A);
dAdt.correctBoundaryConditions();

// evaluate current density
elcondF = fvc::interpolate(elcond);
jn      = - elcondF * fvc::snGrad(potE) * mesh.magSf()
		  + (fvc::interpolate(elcond * ((U ^ B) - dAdt)) & mesh.Sf());
J       = fvc::reconstruct(jn);
J.correctBoundaryConditions();
JB = -fvc::laplacian(1.0 / magMu, A);

// compute JxB force
bool JConservativeForm(
	runTime.controlDict().get<bool>("JConservativeForm")
);
if (JConservativeForm){
	surfaceVectorField bf = fvc::interpolate(B);
	JxB = - fvc::surfaceIntegrate((jn * bf) ^ mesh.Cf())
		  + (fvc::div(jn, B) ^ mesh.C());
} else {
	JxB = J ^ B;
}

// diagnostic variable evaluation
volScalarField numerator   = mag(fvc::curl(U ^ B));
volScalarField denominator = mag(fvc::laplacian(B) / elcond / magMu);
Rm     = numerator / max(denominator, dimensionedScalar("small", denominator.dimensions(), SMALL));
Rm_ind = numerator;
Rm_dif = denominator;
