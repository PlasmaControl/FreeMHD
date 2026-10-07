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
// [Dbg] The inner ABC loop below breaks on a LOOSE relative residual (1e-3
// hardwired).  The DtN map then differentiates that solution and amplifies it
// by -(l+1)/R, so a loose vacuum solve can surface as a percent-level error in
// the imposed gradient.  Truncation is immune: a Dirichlet condition does not
// differentiate the solution.  Make the tolerance settable and report the exit.
// [Dbg] PROBE 2: how strongly is the vacuum pinned by its inner interfaces?
// continuousAMixed sets valueFraction w = K_nbr/(K_own + K_nbr), K = delta/mu.
// magMu is uniform here, so w = delta_nbr/(delta_own + delta_nbr).  w -> 1 means
// the vacuum face value is slaved to the neighbour and the OUTER condition
// cannot propagate inward.
{
    static bool bcDiagDone = false;
    if (!bcDiagDone && runTime.controlDict().getOrDefault<bool>("VSH_bcDiag", false))
    {
        bcDiagDone = true;
        Info<< "[Dbg] ---- interface weighting on " << mesh.name() << " ----" << nl;
        // NOTE: iterate ONLY the global patches.  mesh.boundary() also holds
        // processor patches whose COUNT DIFFERS PER RANK, so looping all of
        // them makes the collective gMin/gAverage/distribute calls below run a
        // different number of times on different ranks -> deadlock.
        const label nGlobalPatches = mesh.boundaryMesh().nNonProcessor();
        for (label pI = 0; pI < nGlobalPatches; ++pI)
        {
            const fvPatch& pp = mesh.boundary()[pI];
            const scalarField dOwn(pp.deltaCoeffs());
            if (!isA<mappedPatchBase>(pp.patch()))
            {
                scalar mn = gMin(dOwn), mx = gMax(dOwn), av = gAverage(dOwn);
                Info<< "[Dbg]   " << pp.name() << " (not mapped)"
                    << "  delta_own min/avg/max = "
                    << mn << " / " << av << " / " << mx << nl;
                continue;
            }
            const mappedPatchBase& mpp =
                refCast<const mappedPatchBase>(pp.patch());
            const fvMesh& nbrMesh = refCast<const fvMesh>(mpp.sampleMesh());
            const fvPatch& nbrPatch =
                nbrMesh.boundary()[mpp.samplePolyPatch().index()];
            scalarField dNbr(nbrPatch.deltaCoeffs());
            mpp.distribute(dNbr);
            scalarField w(pp.size(), 0.0);
            forAll(w, i)
            {
                w[i] = dNbr[i]/max(dOwn[i] + dNbr[i], SMALL);
            }
            Info<< "[Dbg]   " << pp.name() << " -> " << nbrMesh.name() << nl
                << "[Dbg]       delta_own min/avg/max = "
                << gMin(dOwn) << " / " << gAverage(dOwn) << " / " << gMax(dOwn) << nl
                << "[Dbg]       delta_nbr min/avg/max = "
                << gMin(dNbr) << " / " << gAverage(dNbr) << " / " << gMax(dNbr) << nl
                << "[Dbg]       valueFraction w min/avg/max = "
                << gMin(w) << " / " << gAverage(w) << " / " << gMax(w)
                << "   (w->1 = slaved to neighbour)" << nl;
        }
    }
}

const scalar abcTol
(
	runTime.controlDict().getOrDefault<scalar>("VSH_abcTol", 1e-3)
);
int abcIters = 0;
scalar abcRes = 0.0;
// [Dbg] VSH_vacuumCurlCurl: solve the vacuum as curl curl A = 0, i.e.
// lap A = grad(div A), instead of lap A = 0.  The two agree only if div A is
// uniform; otherwise lap A = 0 leaves curl B = grad(div A) != 0, so the "vacuum"
// carries current.  Measured: the gradient-family trace at R_v is ~98% inconsistent
// and dominated by even l, i.e. NOT pure gauge.  grad(div A) is lagged and
// re-evaluated every ABC iteration; gradient modes are neutral under this
// iteration (grad div grad chi = lap grad chi), solenoidal modes converge.
const bool vacCurlCurl =
	runTime.controlDict().getOrDefault<bool>("VSH_vacuumCurlCurl", false);
// [Dbg] under-relax the explicit grad(div A) source; at full strength the
// lagged splitting against the implicit Laplacian diverged (FPE after 66 steps).
const scalar runtime_ccRelax =
	runTime.controlDict().getOrDefault<scalar>("VSH_ccRelax", 1.0);
// [Dbg] VSH_vacGaugeIter: vacuum-only Coulomb projection, A -= grad(psi) with
// lap(psi) = div(A).  Differs from globalGaugeClean.H, which uses
// reconstruct(snGrad(psi)*magSf) -- NOT in the discrete kernel of fvc::curl, and
// measured to multiply |curl A|^2 by ~5 per sweep on the Rm=2000 LDC (job G3_probe).
// Here: (i) fvc::grad, whose curl defect is ~1e-3 relative with leastSquares,
// (ii) vacuum only, (iii) applied BEFORE the DtN projection so the operator sees a
// trace with the gradient family already removed.  The injected curl is measured
// and reported every sweep rather than assumed small.
const label nVacGauge =
	runTime.controlDict().getOrDefault<label>("VSH_vacGaugeIter", 0);
for (int j = 0; j < nABCiter; j++)
{
	fvVectorMatrix mvpotEqn
	(
		fvm::laplacian(A)
		==
		dimensionedVector("zero",dimensionSet(1, -1, -2, 0, 0, -1, 0), vector(0,0,0))
	);
	if (vacCurlCurl)
	{
		mvpotEqn -=
			runtime_ccRelax*fvc::grad(fvc::div(A));
	}
	scalar Res = cmptMax(mvpotEqn.solve().initialResidual());
	abcIters = j + 1;
	abcRes = Res;

	/*
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
	*/

	for (label g = 0; g < nVacGauge; ++g)
	{
		fvScalarMatrix PsiEqn
		(
			fvm::laplacian(psi) == fvc::div(A)
		);
		PsiEqn.solve();
		const scalar curlPre = Foam::sqrt(gSum(mesh.V()*magSqr(fvc::curl(A)().primitiveField()))/gSum(mesh.V()));
		const scalar divPre  = Foam::sqrt(gSum(mesh.V()*sqr(fvc::div(A)().primitiveField()))/gSum(mesh.V()));
		A -= fvc::grad(psi);
		A.correctBoundaryConditions();
		const scalar curlPost = Foam::sqrt(gSum(mesh.V()*magSqr(fvc::curl(A)().primitiveField()))/gSum(mesh.V()));
		const scalar divPost  = Foam::sqrt(gSum(mesh.V()*sqr(fvc::div(A)().primitiveField()))/gSum(mesh.V()));
		if (runTime.writeTime() && Pstream::master())
		{
			Info<< "[Dbg] vacGauge sweep " << g
				<< ": rms|curlA| " << curlPre << " -> " << curlPost
				<< " (" << 100.0*(curlPost - curlPre)/(curlPre + SMALL) << "%)"
				<< "  rms divA " << divPre << " -> " << divPost << nl;
		}
	}

	if (mesh.name() == "vacuum_sphere"
	    && runTime.controlDict().getOrDefault<bool>("VSH_enable", false))
	{
		#include "vacuumAExteriorEquation.H"
	}

	if (Res < abcTol){
		break;
	}
}

if (runTime.controlDict().getOrDefault<bool>("VSH_abcReport", false))
{
	Info<< "[Dbg] ABC loop " << mesh.name()
		<< ": iters=" << abcIters << "/" << nABCiter
		<< "  finalRes=" << abcRes
		<< "  tol=" << abcTol
		<< (abcRes < abcTol ? "  (converged)" : "  (HIT ITERATION CAP)") << nl;
}

dAdt = fvc::ddt(A);
dAdt.correctBoundaryConditions();
B = fvc::curl(A);
B.correctBoundaryConditions();

if (finalIter)
{
	mesh.data::remove("finalIteration");
}
