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

// Transient external field B0t(t) and dAext/dt; shared with the potE re-solve.
#include "createDAextdt.H"

curlA = fvc::curl(A);
curlA.correctBoundaryConditions();
B     = B0 * Bscale + B0t + curlA;
B.correctBoundaryConditions();

// solve for scalar and vector potentials
label nAEiter(
	runTime.controlDict().get<label>("nAEiter")
);
// A-potE Picard tolerance; 1e-3 is the legacy value, tighten (1e-5) for transients.
const scalar nAEtol(
	runTime.controlDict().getOrDefault<scalar>("nAEtol", 1e-3)
);

// Keep the previous-corrector A for under-relaxation; un-relaxed A swings ~70% at high Rm.
A.storePrevIter();

// Freeze dAdt at the start of the inner Picard loop. Recomputing it each
// iteration with dt~1e-6 amplifies any A change by 1/dt into the potE
// source, which drives the iteration into a stable limit cycle instead of
// converging. Holding dAdt fixed across the loop breaks that feedback.
dAdt  = fvc::ddt(A);
dAdt.correctBoundaryConditions();

for (label iter = 0; iter < nAEiter; ++iter){
	psiub = fvc::interpolate(elcond * ((U ^ B) - dAdt - dAextdt)) & mesh.Sf();
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
		+ (fvc::interpolate(elcond * ((U ^ B) - dAextdt)) & mesh.Sf())
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
	B     = B0 * Bscale + B0t + curlA;
	B.correctBoundaryConditions();

	if (max(potERes, ARes) < nAEtol){
		break;
	}
}

// Under-relax A (relaxationFactors.fields[A]; no-op if unset), then rebuild curlA and B.
A.relax();
A.correctBoundaryConditions();
curlA = fvc::curl(A);
curlA.correctBoundaryConditions();
B     = B0 * Bscale + B0t + curlA;
B.correctBoundaryConditions();

dAdt = fvc::ddt(A);
dAdt.correctBoundaryConditions();

// evaluate current density
elcondF = fvc::interpolate(elcond);
jn      = - elcondF * fvc::snGrad(potE) * mesh.magSf()
		  + (fvc::interpolate(elcond * ((U ^ B) - dAdt - dAextdt)) & mesh.Sf());
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

// Implicit Hartmann-drag split: the -sigma|B|^2 U part of J x B is solved as
// fvm::Sp in mhdUEqn.H; add it back here so phig carries the full J x B.
JxB = JxB + elcond*magSqr(B)*U;

// diagnostic variable evaluation
volScalarField numerator   = mag(fvc::curl(U ^ B));
volScalarField denominator = mag(fvc::laplacian(B) / elcond / magMu);
Rm     = numerator / max(denominator, dimensionedScalar("small", denominator.dimensions(), SMALL));
Rm_ind = numerator;
Rm_dif = denominator;
