//const scalar L = 20;
const scalar L = runTime.controlDict().getOrDefault("VSH_L", 1);
// Boundary-gradient under-relaxation; default 0.1 is the original 0.9/0.1 blend.
const scalar VSHalpha = runTime.controlDict().getOrDefault<scalar>("VSH_alpha", 0.1);
// [Dbg] VSH_gaugeDecay: give the gradient (gauge) family its own decaying condition from
// the MEASURED c_lm, instead of slaving it to the radial family as beta = b/R.  On a real
// dynamo field the measured beta is ~98% inconsistent with b/R (l = 2 dominated), and the
// unmatched content otherwise receives NO imposed derivative -- a reflecting wall.  A pure
// decaying gauge harmonic grad(C r^-(l+1) Y) has every component falling as -(l+2)/r.
const bool gaugeDecay = runTime.controlDict().getOrDefault<bool>("VSH_gaugeDecay", false);

// ---- [Dbg] VSH_mms: manufactured-solution test of the operator in isolation.
// Overwrite the patch values of A with an EXACT decaying exterior toroidal mode
//     A = r^-(l+1) * (e_r x grad_s Y_l0) = r^-(l+1) * (dY_l0/dtheta) e_phi
// whose exact radial derivative is  dA/dr = -(l+1)/r * A.
// Then compare the operator's reconstructed dA_dr_cart against that exact value.
// A correct DtN map must reproduce it to discretisation error.
const label VSHmms = runTime.controlDict().getOrDefault<label>("VSH_mms", -1);
// [Dbg] VSH_cartDtN: the EXACT exterior operator for the equation the vacuum actually
// solves, componentwise lap(A) = 0 with no constraint on div A.  Its decaying exterior has
// THREE VSH families per (l,m): toroidal r^-(l+1) Phi, gradient r^-(l+2) (Psi - (l+1) Y e_r),
// and a third, r^-l (Psi + l Y e_r), with div A != 0 and curl A != 0.  The two-family map
// below has no slot for the third family, which carries B, and imposes a wrong derivative
// on it.  Here each Cartesian component is expanded in scalar Y_lm and given its own
// decaying derivative, dA_i/dr = sum -(l+1)/R s_i,lm Y_lm, l = 0 included.  On a
// Coulomb-admissible trace this coincides with the two-family map.  A VSH degree below L
// needs scalar degree up to L, so the default (exclusive) bound is L + 1.
const bool cartDtN = runTime.controlDict().getOrDefault<bool>("VSH_cartDtN", false);
const label Lc = runTime.controlDict().getOrDefault<label>("VSH_cartL", label(L) + 1);
vectorField s_lm_Re(cartDtN ? Lc*Lc : 0, Zero);
vectorField s_lm_Im(cartDtN ? Lc*Lc : 0, Zero);
if (cartDtN)
{
    static bool cartMsg = false;
    if (!cartMsg)
    {
        cartMsg = true;
        Info<< "[Dbg] VSH_cartDtN ON: componentwise scalar DtN, scalar degree < "
            << Lc << nl;
    }
}
// [Dbg] operator CPU time (this rank), reported at write times.
const scalar vshCpu0 = runTime.elapsedCpuTime();
static scalar vshCpuSum = 0.0;
static label vshCalls = 0;
// [Dbg] VSH_cacheY: the boundary faces are static, so every scalar harmonic the componentwise
// operator evaluates on them (at the face centre and at the four finite-difference points of the
// tangential derivatives) is tabulated once per region and reused.  The tabulated values are the
// same EvalSHSlow calls, consumed in the same order, so the arithmetic of the operator is unchanged.
const bool cacheY = cartDtN && runTime.controlDict().getOrDefault<bool>("VSH_cacheY", true);
// [Dbg] With cartDtN the two-family (VSH) projection is read only by the spectrum report (write
// times) and the MMS leak report, and the two-family reconstruction by nothing at all.
const bool needTwoFamily =
    !cartDtN
 || (VSHmms >= 1)
 || (runTime.controlDict().getOrDefault<bool>("VSH_spectrum", false) && runTime.writeTime())
 || !runTime.controlDict().getOrDefault<bool>("VSH_skipTwoFamily", true);
static HashTable<scalarList> vshYtab;
static HashTable<label> vshYtabLc;
const label nKc = Lc*Lc;
const scalarList* vshTab = nullptr;
if (cacheY)
{
    const word vshKey = mesh.name();
    const label pID = mesh.boundary().findPatchID("vacuumExterior");
    const label nF = (pID >= 0) ? mesh.boundary()[pID].size() : 0;
    if
    (
        !vshYtab.found(vshKey)
     || vshYtabLc[vshKey] != Lc
     || vshYtab[vshKey].size() != 10*nF*nKc
    )
    {
        scalarList tab(10*nF*nKc, 0.0);
        for (label f = 0; f < nF; ++f)
        {
            // angles exactly as in the projection and reconstruction loops below
            const vector& Cf = mesh.boundary()[pID].Cf()[f];
            const scalar x = Cf.x();
            const scalar y = Cf.y();
            const scalar z = Cf.z();
            const scalar R = Foam::sqrt(x*x + y*y + z*z);
            const scalar mu = Foam::max(-1.0, Foam::min(1.0, z/R));
            const scalar theta = Foam::acos(mu);
            const scalar phi = Foam::atan2(y, x);
            const scalar thetaP = Foam::min(constant::mathematical::pi - 1e-8, theta + 1e-6);
            const scalar thetaM = Foam::max(1e-8, theta - 1e-6);
            const scalar thE[5] = {theta, thetaP, thetaM, theta, theta};
            const scalar phE[5] = {phi, phi, phi, phi + 1e-6, phi - 1e-6};
            for (label p = 0; p < 5; ++p)
            {
                const label o = (f*5 + p)*2*nKc;
                for (int l = 0; l < Lc; ++l)
                {
                    for (int m = -l; m <= l; ++m)
                    {
                        const label kc = l*l + l + m;
                        tab[o + kc]       = EvalSHSlow_Re(l, m, phE[p], thE[p]);
                        tab[o + nKc + kc] = EvalSHSlow_Im(l, m, phE[p], thE[p]);
                    }
                }
            }
        }
        vshYtab.set(vshKey, tab);
        vshYtabLc.set(vshKey, Lc);
        Info<< "[Dbg] VSH_cacheY: tabulated " << nKc << " harmonics x 5 points on "
            << returnReduce(nF, sumOp<label>()) << " faces of " << vshKey << nl;
    }
    vshTab = &vshYtab[vshKey];
}

// [Dbg] VSH_mmsFamily: which exact decaying exterior mode VSH_mms prescribes.
//   0 toroidal  r^-(l+1) Phi                 dA/dr = -(l+1)/r A   (default)
//   1 gradient  r^-(l+2) (Psi - (l+1) Y e_r) dA/dr = -(l+2)/r A
//   2 third     r^-l     (Psi + l Y e_r)     dA/dr = -l/r A
const label VSHmmsFam = runTime.controlDict().getOrDefault<label>("VSH_mmsFamily", 0);
// [Dbg] VSH_mmsM: azimuthal order of the MMS mode, built from the real harmonic Re Y_lm.
const label VSHmmsM = runTime.controlDict().getOrDefault<label>("VSH_mmsM", 0);
vshDThetaFix = runTime.controlDict().getOrDefault<bool>("VSH_dThetaFix", false);
{
    static bool fixMsg = false;
    if (!fixMsg && vshDThetaFix)
    {
        fixMsg = true;
        Info<< "[Dbg] VSH_dThetaFix ON: EvalSHSlow_dTheta returns +dY/dtheta" << nl;
    }
}
auto mmsTrace = [&](const scalar Rm, const scalar th, const scalar ph) -> vector
{
    // f = Re Y_lm.  The theta derivative is a central difference of EvalSHSlow_Re, NOT
    // EvalSHSlow_dTheta, so the manufactured trace is independent of the routine under test.
    const scalar hT  = 1e-6;
    const scalar Ym  = EvalSHSlow_Re(VSHmms, VSHmmsM, ph, th);
    const scalar dYm =
        (EvalSHSlow_Re(VSHmms, VSHmmsM, ph, th + hT) - EvalSHSlow_Re(VSHmms, VSHmmsM, ph, th - hT))
       /(2.0*hT);
    const scalar pYm =
        -VSHmmsM*EvalSHSlow_Im(VSHmms, VSHmmsM, ph, th)/Foam::max(Foam::sin(th), 1e-12);
    const scalar sT = Foam::sin(th), cT = Foam::cos(th);
    const scalar sP = Foam::sin(ph), cP = Foam::cos(ph);
    const vector eR(sT*cP, sT*sP, cT), eT(cT*cP, cT*sP, -sT), eP(-sP, cP, 0.0);
    const vector Psi = dYm*eT + pYm*eP;     // r grad_s f
    const vector Phi = dYm*eP - pYm*eT;     // e_r x Psi
    const scalar lm = VSHmms;
    if (VSHmmsFam == 1) { return Foam::pow(Rm, -(lm + 2.0))*(Psi - (lm + 1.0)*Ym*eR); }
    if (VSHmmsFam == 2) { return Foam::pow(Rm, -lm)*(Psi + lm*Ym*eR); }
    return Foam::pow(Rm, -(lm + 1.0))*Phi;
};
const scalar mmsK =
    (VSHmmsFam == 1) ? -(VSHmms + 2.0)
  : (VSHmmsFam == 2) ? -scalar(VSHmms)
  : -(VSHmms + 1.0);

scalar mmsErr2 = 0.0, mmsRef2 = 0.0, mmsErrInf = 0.0, mmsRefInf = 0.0;
scalarField a_lm_Re(2*L*L, 0.0); scalarField a_lm_Im(2*L*L, 0.0);
scalarField b_lm_Re(2*L*L, 0.0); scalarField b_lm_Im(2*L*L, 0.0);
// [Dbg] c_lm = projection of A onto T = grad_s Y (the "beta" component).
// The operator NEVER measures this; it asserts beta = b/R.  Measure the truth.
scalarField c_lm_Re(2*L*L, 0.0); scalarField c_lm_Im(2*L*L, 0.0);
scalar RSPHERE = 0.0;

for(int j=0; j<2*L*L; j++) {
	a_lm_Re[j] = 0.; a_lm_Im[j] = 0.;
	b_lm_Re[j] = 0.; b_lm_Im[j] = 0.;
}
forAll(mesh.boundary(), patch) {
	if (patch != mesh.boundary().findPatchID("vacuumExterior")) { continue; }
	forAll(mesh.boundary()[patch], face) {
		const vector& Cf = mesh.boundary()[patch].Cf()[face];
		const scalar x = Cf.x();
		const scalar y = Cf.y();
		const scalar z = Cf.z();
		const scalar R = Foam::sqrt(x*x + y*y + z*z);
		const scalar mu = Foam::max(-1.0, Foam::min(1.0, z/R));
		const scalar theta = Foam::acos(mu);
		const scalar phi = Foam::atan2(y, x);
		const scalar weight = mesh.boundary()[patch].magSf()[face] / (R * R);
		RSPHERE = R;
		fvPatchField<vector> &Av = A.boundaryFieldRef()[patch];
		if (VSHmms >= 1)
{
			Av[face] = mmsTrace(R, theta, phi);
		}
		scalar Ax = Av[face].x(); scalar Ay = Av[face].y(); scalar Az = Av[face].z();
		scalar Ar = Ax * Foam::sin(theta) * Foam::cos(phi) + Ay * Foam::sin(theta) * Foam::sin(phi) + Az * Foam::cos(theta);
		scalar At = Ax * Foam::cos(theta) * Foam::cos(phi) + Ay * Foam::cos(theta) * Foam::sin(phi) - Az * Foam::sin(theta);
		scalar Ap = -Ax * Foam::sin(phi) + Ay * Foam::cos(phi);
		for(int l = 0; l < (needTwoFamily ? label(L) : 0); l++) {
			scalar coeff_a = 0;
			if (l > 0) {coeff_a = 1.0 / (l * (l + 1.0));}
			scalar coeff_b = -1. / (l + 1.);
			for(int m=-l; m<=l; m++) {
				scalar Y_lm_Re = EvalSHSlow_Re(l, m, phi, theta);
				scalar Y_lm_Im = EvalSHSlow_Im(l, m, phi, theta);
				scalar P_lm_r_Re = 0.; scalar P_lm_r_Im = 0.;
				scalar P_lm_t_Re = 0.;
				scalar P_lm_t_Im = 0.;
				//if (abs(Foam::sin(theta)) > 1e-10) {
					P_lm_t_Re = m / Foam::sin(theta) * Y_lm_Im;
					P_lm_t_Im = -m / Foam::sin(theta) * Y_lm_Re;
				//}
				scalar P_lm_p_Re = EvalSHSlow_dTheta_Re(l, m, phi, theta);
				scalar P_lm_p_Im = EvalSHSlow_dTheta_Im(l, m, phi, theta);
				scalar A_dot_P_Re = At * P_lm_t_Re + Ap * P_lm_p_Re;
				scalar A_dot_P_Im = At * P_lm_t_Im + Ap * P_lm_p_Im;
				a_lm_Re[2*L*l+m+L] += A_dot_P_Re * weight * coeff_a;
				a_lm_Im[2*L*l+m+L] -= A_dot_P_Im * weight * coeff_a;
				b_lm_Re[2*L*l+m+L] += Ar * Y_lm_Re * weight * coeff_b * R;
				b_lm_Im[2*L*l+m+L] -= Ar * Y_lm_Im * weight * coeff_b * R;
				// [Dbg] A . T*, T = grad_s Y : T_t = dY/dtheta, T_p = (1/sin) dY/dphi
				{
					const scalar T_t_Re =  P_lm_p_Re;
					const scalar T_t_Im =  P_lm_p_Im;
					const scalar T_p_Re = -P_lm_t_Re;
					const scalar T_p_Im = -P_lm_t_Im;
					const scalar AdotT_Re = At*T_t_Re + Ap*T_p_Re;
					const scalar AdotT_Im = At*T_t_Im + Ap*T_p_Im;
					c_lm_Re[2*L*l+m+L] += AdotT_Re * weight * coeff_a;
					c_lm_Im[2*L*l+m+L] -= AdotT_Im * weight * coeff_a;
				}
			}
		}
		if (cartDtN)
		{
			const vector Af = Av[face];
			const label o0 = face*5*2*nKc;
			for (int l = 0; l < Lc; ++l)
			{
				for (int m = -l; m <= l; ++m)
				{
					const label kc = l*l + l + m;
					const scalar yRe = vshTab ? (*vshTab)[o0 + kc] : EvalSHSlow_Re(l, m, phi, theta);
					const scalar yIm = vshTab ? (*vshTab)[o0 + nKc + kc] : EvalSHSlow_Im(l, m, phi, theta);
					s_lm_Re[kc] += Af*(yRe*weight);
					s_lm_Im[kc] -= Af*(yIm*weight);
				}
			}
		}
	}
}
reduce(a_lm_Re, sumOp<scalarField>()); reduce(a_lm_Im, sumOp<scalarField>());
reduce(b_lm_Re, sumOp<scalarField>()); reduce(b_lm_Im, sumOp<scalarField>());
reduce(c_lm_Re, sumOp<scalarField>()); reduce(c_lm_Im, sumOp<scalarField>());
reduce(RSPHERE, maxOp<scalar>());
if (cartDtN)
{
    reduce(s_lm_Re, sumOp<vectorField>());
    reduce(s_lm_Im, sumOp<vectorField>());
}

// ---- [Dbg] VSH_gainA: deliberately scale the recovered toroidal amplitude by
// (1 + g) to MEASURE the sensitivity dE_B/dg of the solution to a systematic
// amplitude error in the DtN map.  The manufactured-solution test found the
// shipped operator over-recovers |a| by +0.24%..+1.14% (midpoint quadrature).
// If the measured sensitivity is large, that bias explains the +3.4..3.9%
// solution error; if it is O(1), something else remains.
{
    const scalar gA = runTime.controlDict().getOrDefault<scalar>("VSH_gainA", 0.0);
    if (mag(gA) > SMALL)
    {
        a_lm_Re *= (1.0 + gA);
        a_lm_Im *= (1.0 + gA);
    }
}

// ---- [Dbg] VSH_betaFit: determine the gauge amplitude by least squares over
// BOTH measured components, instead of from the radial component alone.
//
// Boundary data per (l,m):  A = alpha Y e_r + beta grad_s Y + gamma (e_r x grad_s Y)
// The gauge family contributes (alpha, beta) = (b/R) * (-(l+1), 1).
// Minimising ||(alpha + (l+1)x) Y e_r + (beta - x) grad_s Y||^2 with
// ||Y e_r||^2 = 1 and ||grad_s Y||^2 = l(l+1) gives
//
//     x = b/R = (l*beta - alpha) / (2l + 1)
//
// The shipped operator instead uses x = -alpha/(l+1), which is the same thing
// ONLY if beta == -alpha/(l+1).  Measurement says that is violated by 79-99%.
//
// In stored variables: alpha = -(l+1) b_lm / R and beta = c_lm, so
//     b_new = ( R*l*c_lm + (l+1)*b_lm ) / (2l + 1)
if (runTime.controlDict().getOrDefault<bool>("VSH_betaFit", false))
{
    for (int l = 1; l < L; ++l)
    {
        const scalar den = 2.0*l + 1.0;
        for (int m = -l; m <= l; ++m)
        {
            const label idx = 2*L*l + m + L;
            const scalar nRe =
                (RSPHERE*l*c_lm_Re[idx] + (l + 1.0)*b_lm_Re[idx])/den;
            const scalar nIm =
                (RSPHERE*l*c_lm_Im[idx] + (l + 1.0)*b_lm_Im[idx])/den;
            b_lm_Re[idx] = nRe;
            b_lm_Im[idx] = nIm;
        }
    }
}



// ---- [Dbg] VSH_ortho: discrete orthogonality of the harmonics ----------
// Both projections assume  int Y_lm conj(Y_l'm') dOmega = delta.  On a faceted
// sphere the face-centre midpoint rule only approximates that.  Measure the
// actual Gram matrix over the real patch faces, once, at the first call.
{
    static bool orthoDone = false;
    if
    (
        !orthoDone
     && runTime.controlDict().getOrDefault<bool>("VSH_ortho", false)
    )
    {
        orthoDone = true;
        const label nlm = label(L*L);
        scalarField gRe(nlm*nlm, 0.0), gIm(nlm*nlm, 0.0);
        scalar area = 0.0;

        forAll(mesh.boundary(), patchO)
        {
            if (patchO != mesh.boundary().findPatchID("vacuumExterior"))
            {
                continue;
            }
            forAll(mesh.boundary()[patchO], faceO)
            {
                const vector& Cf = mesh.boundary()[patchO].Cf()[faceO];
                const scalar Rf =
                    Foam::sqrt(Cf.x()*Cf.x() + Cf.y()*Cf.y() + Cf.z()*Cf.z());
                const scalar muf =
                    Foam::max(-1.0, Foam::min(1.0, Cf.z()/Rf));
                const scalar th = Foam::acos(muf);
                const scalar ph = Foam::atan2(Cf.y(), Cf.x());
                const scalar w =
                    mesh.boundary()[patchO].magSf()[faceO]/(Rf*Rf);
                area += w;

                label i = 0;
                for (int l = 0; l < L; ++l)
                {
                    for (int m = -l; m <= l; ++m, ++i)
                    {
                        const scalar yRe = EvalSHSlow_Re(l, m, ph, th);
                        const scalar yIm = EvalSHSlow_Im(l, m, ph, th);
                        label j = 0;
                        for (int l2 = 0; l2 < L; ++l2)
                        {
                            for (int m2 = -l2; m2 <= l2; ++m2, ++j)
                            {
                                const scalar zRe = EvalSHSlow_Re(l2, m2, ph, th);
                                const scalar zIm = EvalSHSlow_Im(l2, m2, ph, th);
                                // Y_lm * conj(Y_l2m2)
                                gRe[i*nlm + j] += w*(yRe*zRe + yIm*zIm);
                                gIm[i*nlm + j] += w*(yIm*zRe - yRe*zIm);
                            }
                        }
                    }
                }
            }
        }
        reduce(gRe, sumOp<scalarField>());
        reduce(gIm, sumOp<scalarField>());
        reduce(area, sumOp<scalar>());

        scalar maxDiag = 0.0, maxOff = 0.0;
        label iD = -1, iO = -1, jO = -1;
        for (label i = 0; i < nlm; ++i)
        {
            for (label j = 0; j < nlm; ++j)
            {
                const scalar mag2 =
                    Foam::sqrt
                    (
                        sqr(gRe[i*nlm + j]) + sqr(gIm[i*nlm + j])
                    );
                if (i == j)
                {
                    if (Foam::mag(mag2 - 1.0) > maxDiag)
                    {
                        maxDiag = Foam::mag(mag2 - 1.0);
                        iD = i;
                    }
                }
                else if (mag2 > maxOff)
                {
                    maxOff = mag2;
                    iO = i;
                    jO = j;
                }
            }
        }
        Info<< "[Dbg] VSH discrete orthogonality on " << mesh.name()
            << "  (L=" << label(L) << ", " << nlm << " modes)" << nl
            << "[Dbg]   patch solid angle   = " << area
            << "   (exact 4*pi = " << 4.0*constant::mathematical::pi << ")" << nl
            << "[Dbg]   max |diag - 1|      = " << maxDiag
            << "   at flat index " << iD << nl
            << "[Dbg]   max |off-diagonal|  = " << maxOff
            << "   at (" << iO << "," << jO << ")" << nl;
    }
}

// ---- [Dbg] EXPERIMENT / DIAGNOSTIC SWITCHES ---------------------------
// The "b" family is
//     A_b = (b/r^{l+2}) [ -(l+1) Y e_r + grad_s Y ]  ==  grad( b r^{-(l+1)} Y )
// i.e. a pure GAUGE mode: curl(A_b) == 0, it carries no magnetic field.
// The "a" (toroidal) family carries all the physical exterior B.
//
//   VSH_dropB    true -> zero the gauge family, so the DtN map constrains
//                        only the physical content of A.
//   VSH_spectrum true -> report per-l power in each family at write times.
{
    if (runTime.controlDict().getOrDefault<bool>("VSH_dropB", false))
    {
        b_lm_Re = 0.0;
        b_lm_Im = 0.0;
    }

    if
    (
        runTime.controlDict().getOrDefault<bool>("VSH_spectrum", false)
     && runTime.writeTime()
    )
    {
        scalar sumA = 0.0, sumB = 0.0;
        Info<< "[Dbg] VSH spectrum on " << mesh.name()
            << "  (A_a ~ a, A_b ~ b/R)" << nl;
        for (int l = 0; l < L; ++l)
        {
            scalar pa = 0.0, pb = 0.0;
            for (int m = -l; m <= l; ++m)
            {
                const label idx = 2*L*l + m + L;
                pa += sqr(a_lm_Re[idx]) + sqr(a_lm_Im[idx]);
                pb += sqr(b_lm_Re[idx]) + sqr(b_lm_Im[idx]);
            }
            sumA += pa;
            sumB += pb;
            Info<< "[Dbg]   l=" << l
                << "  |a_l|=" << Foam::sqrt(pa)
                << "  |b_l|=" << Foam::sqrt(pb) << nl;
        }
        {
            Info<< "[Dbg]   -- beta check: measured A.T vs asserted b/R --" << nl;
            scalar sm = 0.0, sa = 0.0, sd = 0.0;
            for (int l = 1; l < L; ++l)
            {
                scalar pm = 0.0, pa = 0.0, pd = 0.0;
                for (int m = -l; m <= l; ++m)
                {
                    const label idx = 2*L*l + m + L;
                    const scalar mRe = c_lm_Re[idx];
                    const scalar mIm = c_lm_Im[idx];
                    const scalar aRe = b_lm_Re[idx]/RSPHERE;
                    const scalar aIm = b_lm_Im[idx]/RSPHERE;
                    pm += sqr(mRe) + sqr(mIm);
                    pa += sqr(aRe) + sqr(aIm);
                    pd += sqr(mRe - aRe) + sqr(mIm - aIm);
                }
                sm += pm; sa += pa; sd += pd;
                Info<< "[Dbg]   l=" << l
                    << "  |beta_meas|=" << Foam::sqrt(pm)
                    << "  |beta_asserted|=" << Foam::sqrt(pa)
                    << "  |mismatch|=" << Foam::sqrt(pd) << nl;
            }
            Info<< "[Dbg]   TOTAL |beta_meas|=" << Foam::sqrt(sm)
                << "  |beta_asserted|=" << Foam::sqrt(sa)
                << "  |mismatch|=" << Foam::sqrt(sd)
                << "  mismatch/measured="
                << Foam::sqrt(sd)/(Foam::sqrt(sm) + SMALL) << nl;
        }
        Info<< "[Dbg]   TOTAL |a|=" << Foam::sqrt(sumA)
            << "  |b|=" << Foam::sqrt(sumB)
            << "  |b|/|a|=" << Foam::sqrt(sumB)/(Foam::sqrt(sumA) + SMALL)
            << nl;
    }
}

// [Dbg] VSH_mismatchEvery N > 0: every N-th time step, report how far the imposed (relaxed)
// gradient is from the freshly reconstructed DtN derivative, area-weighted L2, relative.
const label vshMmEvery = runTime.controlDict().getOrDefault<label>("VSH_mismatchEvery", 0);
const bool vshDoMm = (vshMmEvery > 0) && (runTime.timeIndex() % vshMmEvery == 0);
scalar vshMmPre = 0.0, vshMmPost = 0.0, vshMmRef = 0.0;
forAll(mesh.boundary(), patch) {
	if (patch != mesh.boundary().findPatchID("vacuumExterior")) { continue; }
	fixedGradientFvPatchVectorField& Av =
		refCast<fixedGradientFvPatchVectorField>
		(
			A.boundaryFieldRef()[patch]
		);
	forAll(mesh.boundary()[patch], face) {
		scalar dAr = 0.0, dAt = 0.0, dAp = 0.0;

		const vector& Cf = mesh.boundary()[patch].Cf()[face];
		const scalar xf = Cf.x();
		const scalar yf = Cf.y();
		const scalar zf = Cf.z();

		const scalar R = Foam::sqrt(xf*xf + yf*yf + zf*zf);
		const scalar phi = Foam::atan2(yf, xf);
		const scalar mu = Foam::max(-1.0, Foam::min(1.0, zf/R));
		const scalar theta = Foam::acos(mu);

		// Face unit normal
		const vector Sf = mesh.Sf().boundaryField()[patch][face];
		const vector nHat = Sf/(mag(Sf) + SMALL);

		// Spherical basis at face center
		const scalar st = Foam::sin(theta);
		const scalar ct = Foam::cos(theta);
		const scalar sp = Foam::sin(phi);
		const scalar cp = Foam::cos(phi);

		const vector e_r(st*cp, st*sp, ct);
		const vector e_t(ct*cp, ct*sp, -st);
		const vector e_p(-sp, cp, 0.0);

		// Components of face normal in spherical basis
		const scalar nr = (nHat & e_r);
		const scalar nt = (nHat & e_t);
		const scalar np = (nHat & e_p);

		// Helper: evaluate A on the sphere at (thetaEval, phiEval), returned in Cartesian components
		auto evalA_cart = [&](const scalar thetaEval, const scalar phiEval) -> vector
		{
			scalar Ar = 0.0, At = 0.0, Ap = 0.0;

			const scalar stE = Foam::sin(thetaEval);
			const scalar ctE = Foam::cos(thetaEval);
			const scalar spE = Foam::sin(phiEval);
			const scalar cpE = Foam::cos(phiEval);

			for (int l = 0; l < L; ++l)
			{
				for (int m = -l; m <= l; ++m)
				{
					const label idx = 2*L*l + m + L;

					const scalar aRe = a_lm_Re[idx];
					const scalar aIm = a_lm_Im[idx];
					const scalar bRe = b_lm_Re[idx];
					const scalar bIm = b_lm_Im[idx];

					const scalar Y_Re   = EvalSHSlow_Re(l, m, phiEval, thetaEval);
					const scalar Y_Im   = EvalSHSlow_Im(l, m, phiEval, thetaEval);
					const scalar dYt_Re = EvalSHSlow_dTheta_Re(l, m, phiEval, thetaEval);
					const scalar dYt_Im = EvalSHSlow_dTheta_Im(l, m, phiEval, thetaEval);

					const scalar invSin = 1.0/(Foam::max(Foam::mag(Foam::sin(thetaEval)), 1e-12));

					// T = grad_s Y, P = e_r x T  (same conventions as your code)
					const scalar T_t_Re = dYt_Re;
					const scalar T_t_Im = dYt_Im;

					const scalar T_p_Re = -m*invSin*Y_Im;
					const scalar T_p_Im =  m*invSin*Y_Re;

					const scalar P_t_Re =  m*invSin*Y_Im;
					const scalar P_t_Im = -m*invSin*Y_Re;

					const scalar P_p_Re = T_t_Re;
					const scalar P_p_Im = T_t_Im;

					// A on r = R:
					// Ar = (b/R)*[-(l+1)Y]
					// At = a*P_t + (b/R)*T_t
					// Ap = a*P_p + (b/R)*T_p
					const scalar coeff_b_val = 1.0/R;
					const scalar gRe_v = gaugeDecay ? c_lm_Re[idx] : coeff_b_val*bRe;
					const scalar gIm_v = gaugeDecay ? c_lm_Im[idx] : coeff_b_val*bIm;

					Ar += coeff_b_val*bRe*(-(l + 1.0)*Y_Re);
					Ar -= coeff_b_val*bIm*(-(l + 1.0)*Y_Im);

					At += aRe*P_t_Re;
					At -= aIm*P_t_Im;
					At += gRe_v*T_t_Re;
					At -= gIm_v*T_t_Im;

					Ap += aRe*P_p_Re;
					Ap -= aIm*P_p_Im;
					Ap += gRe_v*T_p_Re;
					Ap -= gIm_v*T_p_Im;
				}
			}

			// Convert spherical components to Cartesian
			const scalar Ax = Ar*stE*cpE + At*ctE*cpE - Ap*spE;
			const scalar Ay = Ar*stE*spE + At*ctE*spE + Ap*cpE;
			const scalar Az = Ar*ctE      - At*stE;

			return vector(Ax, Ay, Az);
		};

		// [Dbg] VSH_cartDtN: evaluate the trace from the componentwise expansion instead
		auto evalA_sel = [&](const scalar thetaEval, const scalar phiEval, const label pt) -> vector
		{
			if (!cartDtN) { return evalA_cart(thetaEval, phiEval); }
			vector As(Zero);
			const label oP = (face*5 + pt)*2*nKc;
			for (int l = 0; l < Lc; ++l)
			{
				for (int m = -l; m <= l; ++m)
				{
					const label kc = l*l + l + m;
					const scalar yRe = vshTab ? (*vshTab)[oP + kc] : EvalSHSlow_Re(l, m, phiEval, thetaEval);
					const scalar yIm = vshTab ? (*vshTab)[oP + nKc + kc] : EvalSHSlow_Im(l, m, phiEval, thetaEval);
					As += s_lm_Re[kc]*yRe
					    - s_lm_Im[kc]*yIm;
				}
			}
			return As;
		};

		// ----- analytic radial derivative from DtN map (two-family; overwritten when cartDtN) -----
		for (int l = 1; l < (cartDtN ? 1 : label(L)); ++l)
		{
			scalar coeff_a = 0.0;
			if (l > 0)
			{
				coeff_a = -(l + 1.0)/R;
			}
			const scalar coeff_b = -(l + 2.0)/(R*R);

			for (int m = -l; m <= l; ++m)
			{
				const label idx = 2*L*l + m + L;

				const scalar Y_lm_Re = EvalSHSlow_Re(l, m, phi, theta);
				const scalar Y_lm_Im = EvalSHSlow_Im(l, m, phi, theta);
				const scalar T_lm_t_Re = EvalSHSlow_dTheta_Re(l, m, phi, theta);
				const scalar T_lm_t_Im = EvalSHSlow_dTheta_Im(l, m, phi, theta);

				const scalar invSin = 1.0/(Foam::max(Foam::mag(Foam::sin(theta)), 1e-12));

				const scalar T_lm_p_Re = -m*invSin*Y_lm_Im;
				const scalar T_lm_p_Im =  m*invSin*Y_lm_Re;
				const scalar P_lm_t_Re =  m*invSin*Y_lm_Im;
				const scalar P_lm_t_Im = -m*invSin*Y_lm_Re;
				const scalar P_lm_p_Re = T_lm_t_Re;
				const scalar P_lm_p_Im = T_lm_t_Im;

				const scalar gRe_d = gaugeDecay ? -(l + 2.0)/R*c_lm_Re[idx] : coeff_b*b_lm_Re[idx];
				const scalar gIm_d = gaugeDecay ? -(l + 2.0)/R*c_lm_Im[idx] : coeff_b*b_lm_Im[idx];
				dAr += coeff_b*b_lm_Re[idx]* (-(l + 1.0)*Y_lm_Re);
				dAr -= coeff_b*b_lm_Im[idx]* (-(l + 1.0)*Y_lm_Im);

				dAt += coeff_a*a_lm_Re[idx]*P_lm_t_Re;
				dAt -= coeff_a*a_lm_Im[idx]*P_lm_t_Im;
				dAt += gRe_d*T_lm_t_Re;
				dAt -= gIm_d*T_lm_t_Im;

				dAp += coeff_a*a_lm_Re[idx]*P_lm_p_Re;
				dAp -= coeff_a*a_lm_Im[idx]*P_lm_p_Im;
				dAp += gRe_d*T_lm_p_Re;
				dAp -= gIm_d*T_lm_p_Im;
			}
		}

		// Convert analytic radial derivative to Cartesian
		vector dA_dr_cart
		(
			dAr*st*cp + dAt*ct*cp - dAp*sp,
			dAr*st*sp + dAt*ct*sp + dAp*cp,
			dAr*ct    - dAt*st
		);

		if (cartDtN)
		{
			dA_dr_cart = Zero;
			for (int l = 0; l < Lc; ++l)
			{
				for (int m = -l; m <= l; ++m)
				{
					const label kc = l*l + l + m;
					const label o0 = face*5*2*nKc;
					const scalar yRe = vshTab ? (*vshTab)[o0 + kc] : EvalSHSlow_Re(l, m, phi, theta);
					const scalar yIm = vshTab ? (*vshTab)[o0 + nKc + kc] : EvalSHSlow_Im(l, m, phi, theta);
					dA_dr_cart += (-(l + 1.0)/R)*
					(
						s_lm_Re[kc]*yRe
					  - s_lm_Im[kc]*yIm
					);
				}
			}
		}

		if (VSHmms >= 1)
{
			const vector Aex = mmsTrace(R, theta, phi);
			const vector exact = mmsK/R*Aex;   // dA/dr = k/r A
			const vector e = dA_dr_cart - exact;
			const scalar w = mesh.boundary()[patch].magSf()[face];
			mmsErr2 += w*magSqr(e);
			mmsRef2 += w*magSqr(exact);
			mmsErrInf = Foam::max(mmsErrInf, mag(e));
			mmsRefInf = Foam::max(mmsRefInf, mag(exact));
		}

		// ----- angular derivatives by finite differences on the sphere -----
		const scalar hTheta = 1e-6;
		const scalar hPhi   = 1e-6;

		// Avoid stepping exactly onto the poles
		const scalar thetaP = Foam::min(constant::mathematical::pi - 1e-8, theta + hTheta);
		const scalar thetaM = Foam::max(1e-8, theta - hTheta);
		const scalar phiP   = phi + hPhi;
		const scalar phiM   = phi - hPhi;

		const vector A_thetaP = evalA_sel(thetaP, phi, 1);
		const vector A_thetaM = evalA_sel(thetaM, phi, 2);
		const vector dA_dtheta_cart = (A_thetaP - A_thetaM)/(thetaP - thetaM);

		const vector A_phiP = evalA_sel(theta, phiP, 3);
		const vector A_phiM = evalA_sel(theta, phiM, 4);
		const vector dA_dphi_cart = (A_phiP - A_phiM)/(phiP - phiM);

		// ----- true normal derivative -----
		vector dA_dn = nr*dA_dr_cart;

		// add tangential corrections if normal is not exactly radial
		dA_dn += (nt/R)*dA_dtheta_cart;

		if (Foam::mag(st) > 1e-10)
		{
			dA_dn += (np/(R*st))*dA_dphi_cart;
		}

		// This is the actual patch-normal derivative n·grad(A)
		const vector vshGold = Av.gradient()[face];
		Av.gradient()[face] = (1.0 - VSHalpha)*Av.gradient()[face] + VSHalpha*dA_dn;
		if (vshDoMm)
		{
			const scalar w = mesh.boundary()[patch].magSf()[face];
			vshMmPre  += w*magSqr(vshGold - dA_dn);
			vshMmPost += w*magSqr(Av.gradient()[face] - dA_dn);
			vshMmRef  += w*magSqr(dA_dn);
		}
	}
	if (VSHmms >= 1)
{
		reduce(mmsErr2, sumOp<scalar>());  reduce(mmsRef2, sumOp<scalar>());
		reduce(mmsErrInf, maxOp<scalar>()); reduce(mmsRefInf, maxOp<scalar>());
		Info<< "[Dbg] MMS " << (VSHmmsFam == 1 ? "gradient" : VSHmmsFam == 2 ? "third" : "toroidal")
			<< " l=" << VSHmms << " m=" << VSHmmsM << " on " << mesh.name()
			<< "  (exact dA/dr = " << mmsK << "/R A)" << nl
			<< "[Dbg]   L2  rel error = "
			<< Foam::sqrt(mmsErr2)/(Foam::sqrt(mmsRef2) + SMALL) << nl
			<< "[Dbg]   Linf rel error = "
			<< mmsErrInf/(mmsRefInf + SMALL) << nl;
		scalar aOn = 0.0, aOff = 0.0, bAll = 0.0;
		for (int l = 1; l < L; ++l)
		{
			for (int m = -l; m <= l; ++m)
			{
				const label idx = 2*L*l + m + L;
				const scalar pa = sqr(a_lm_Re[idx]) + sqr(a_lm_Im[idx]);
				const scalar pb = sqr(b_lm_Re[idx]) + sqr(b_lm_Im[idx]);
				bAll += pb;
				if (l == VSHmms && m == 0) aOn += pa; else aOff += pa;
			}
		}
		Info<< "[Dbg]   recovered |a_on|  = " << Foam::sqrt(aOn)
			<< "   (exact = " << Foam::pow(RSPHERE, -(VSHmms + 1.0)) << ")" << nl
			<< "[Dbg]   leaked    |a_off| = " << Foam::sqrt(aOff)
			<< "   |b_all| = " << Foam::sqrt(bAll) << nl;
	}
	// [Dbg] PROBE 1: is the imposed gradient large enough to matter?
	if (runTime.controlDict().getOrDefault<bool>("VSH_bcDiag", false))
	{
		const scalarField magSfP(mesh.boundary()[patch].magSf());
		const scalarField dP(mesh.boundary()[patch].deltaCoeffs());
		scalar aw = 0.0, gw = 0.0, sw = 0.0, nw = 0.0;
		forAll(mesh.boundary()[patch], f)
		{
			const scalar w = magSfP[f];
			aw += w*magSqr(Av[f]);
			gw += w*magSqr(Av.gradient()[f]);
			nw += w*magSqr(Av[f])*sqr(dP[f]);
			sw += w;
		}
		reduce(aw, sumOp<scalar>()); reduce(gw, sumOp<scalar>());
		reduce(nw, sumOp<scalar>()); reduce(sw, sumOp<scalar>());
		const scalar rmsA = Foam::sqrt(aw/max(sw, SMALL));
		const scalar rmsG = Foam::sqrt(gw/max(sw, SMALL));
		const scalar natG = Foam::sqrt(nw/max(sw, SMALL));
		Info<< "[Dbg] PROBE1 vacuumExterior: rms|A|=" << rmsA
			<< "  rms|imposed dA/dn|=" << rmsG
			<< "  natural scale rms|A|*delta=" << natG
			<< "  ratio imposed/natural=" << rmsG/(natG + SMALL) << nl;
	}
	Av.updateCoeffs();
}
A.correctBoundaryConditions();
if (vshDoMm)
{
    reduce(vshMmPre, sumOp<scalar>());
    reduce(vshMmPost, sumOp<scalar>());
    reduce(vshMmRef, sumOp<scalar>());
    Info<< "[Dbg] DtN mismatch on " << mesh.name() << " t=" << runTime.value()
        << "  |g_old - DtN|/|DtN| = " << Foam::sqrt(vshMmPre/(vshMmRef + VSMALL))
        << "  |g_imposed - DtN|/|DtN| = " << Foam::sqrt(vshMmPost/(vshMmRef + VSMALL)) << nl;
}
vshCpuSum += runTime.elapsedCpuTime() - vshCpu0;
++vshCalls;
if (runTime.writeTime())
{
    const scalar cpuMax = returnReduce(vshCpuSum, maxOp<scalar>());
    Info<< "[Dbg] VSH operator CPU (max over ranks) = " << cpuMax << " s over " << vshCalls
        << " calls (cacheY " << (cacheY ? "on" : "off") << ")" << nl;
}
