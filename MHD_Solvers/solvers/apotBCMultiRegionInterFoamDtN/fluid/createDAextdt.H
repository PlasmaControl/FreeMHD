// createDAextdt.H
// Transient uniform external field B0t(t) and its analytic dAext/dt, shared by
// mhdAEqn.H and the post-gauge-clean potE re-solve. Needs mesh, runTime in scope.
const vector BtransComponent(
	runTime.controlDict().getOrDefault<vector>("BtransComponent", vector(0,0,0)));
const vector dBtransDt(
	runTime.controlDict().getOrDefault<vector>("dBtransDt", vector(0,0,0)));
const scalar BtransStart(
	runTime.controlDict().getOrDefault<scalar>("BtransStart", 0.0));
const scalar BtransEnd(
	runTime.controlDict().getOrDefault<scalar>("BtransEnd", 0.0));
const vector AextCenter(
	runTime.controlDict().getOrDefault<vector>("AextCenter", vector(0,0,0)));

// B0t(t): frozen outside [BtransStart, BtransEnd], linear inside.
const scalar tNow = runTime.value();
const scalar tc   = max(min(tNow, BtransEnd), BtransStart);
// false: keep only the EMF, drop B0t from the total B (fully-developed cases).
const bool BtransInTotalB(
	runTime.controlDict().getOrDefault<bool>("BtransInTotalB", true));
const dimensionedVector B0t(
	"B0t", dimensionSet(1, 0, -2, 0, 0, -1, 0),
	BtransInTotalB ? (BtransComponent + dBtransDt * (tc - BtransStart)) : vector(0,0,0));
// Soft-start: ramp gamma linearly 0 -> full over BtransRampTime from BtransStart.
const scalar BtransRampTime(
	runTime.controlDict().getOrDefault<scalar>("BtransRampTime", 0.0));
scalar gammaRamp = 1.0;
if (BtransRampTime > SMALL && tNow >= BtransStart)
{
	gammaRamp = max(min((tNow - BtransStart)/BtransRampTime, 1.0), 0.0);
}
const vector dB0tdtVec =
	(tNow >= BtransStart && tNow < BtransEnd) ? (gammaRamp*dBtransDt) : vector(0,0,0);

// dAext/dt = 0.5 * dB0t/dt x (x - x0) [T m/s]; exact for uniform B0t.
const dimensionedVector gammaB(
	"gammaB", dimensionSet(1, 0, -3, 0, 0, -1, 0), dB0tdtVec);
const dimensionedVector x0("x0", dimLength, AextCenter);

// Symmetric gauge by default; AextGaugeExcludeDir = collapsed axis e selects the
// Landau gauge, whose Aext is independent of e (symmetric under-drives it ~2x).
const vector AextExclude(
	runTime.controlDict().getOrDefault<vector>("AextGaugeExcludeDir", vector(0,0,0)));
tmp<volVectorField> tdAextdt;
if (mag(AextExclude) < SMALL)
{
	tdAextdt = 0.5 * (gammaB ^ (mesh.C() - x0));
}
else
{
	const dimensionedVector eHat("eHat", dimless, AextExclude/mag(AextExclude));
	tdAextdt =
		0.5*(gammaB & eHat)*(eHat ^ (mesh.C() - x0))
	  + ((eHat ^ gammaB) & (mesh.C() - x0))*eHat;
}
volVectorField dAextdt("dAextdt", tdAextdt);
