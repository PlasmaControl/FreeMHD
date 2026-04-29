//const scalar L = 20;
const scalar L = runTime.controlDict().getOrDefault("VSH_L", 1);
scalarField a_lm_Re(2*L*L, 0.0); scalarField a_lm_Im(2*L*L, 0.0);
scalarField b_lm_Re(2*L*L, 0.0); scalarField b_lm_Im(2*L*L, 0.0);

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
		fvPatchField<vector> &Av = A.boundaryFieldRef()[patch];
		scalar Ax = Av[face].x(); scalar Ay = Av[face].y(); scalar Az = Av[face].z();
		scalar Ar = Ax * Foam::sin(theta) * Foam::cos(phi) + Ay * Foam::sin(theta) * Foam::sin(phi) + Az * Foam::cos(theta);
		scalar At = Ax * Foam::cos(theta) * Foam::cos(phi) + Ay * Foam::cos(theta) * Foam::sin(phi) - Az * Foam::sin(theta);
		scalar Ap = -Ax * Foam::sin(phi) + Ay * Foam::cos(phi);
		for(int l = 0; l < L; l++) {
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
			}
		}
	}
}
reduce(a_lm_Re, sumOp<scalarField>()); reduce(a_lm_Im, sumOp<scalarField>());
reduce(b_lm_Re, sumOp<scalarField>()); reduce(b_lm_Im, sumOp<scalarField>());
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

                    Ar += coeff_b_val*bRe*(-(l + 1.0)*Y_Re);
                    Ar -= coeff_b_val*bIm*(-(l + 1.0)*Y_Im);

                    At += aRe*P_t_Re;
                    At -= aIm*P_t_Im;
                    At += coeff_b_val*bRe*T_t_Re;
                    At -= coeff_b_val*bIm*T_t_Im;

                    Ap += aRe*P_p_Re;
                    Ap -= aIm*P_p_Im;
                    Ap += coeff_b_val*bRe*T_p_Re;
                    Ap -= coeff_b_val*bIm*T_p_Im;
                }
            }

            // Convert spherical components to Cartesian
            const scalar Ax = Ar*stE*cpE + At*ctE*cpE - Ap*spE;
            const scalar Ay = Ar*stE*spE + At*ctE*spE + Ap*cpE;
            const scalar Az = Ar*ctE      - At*stE;

            return vector(Ax, Ay, Az);
        };

        // ----- analytic radial derivative from DtN map -----
        for (int l = 1; l < L; ++l)
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

                dAr += coeff_b*b_lm_Re[idx]* (-(l + 1.0)*Y_lm_Re);
                dAr -= coeff_b*b_lm_Im[idx]* (-(l + 1.0)*Y_lm_Im);

                dAt += coeff_a*a_lm_Re[idx]*P_lm_t_Re;
                dAt -= coeff_a*a_lm_Im[idx]*P_lm_t_Im;
                dAt += coeff_b*b_lm_Re[idx]*T_lm_t_Re;
                dAt -= coeff_b*b_lm_Im[idx]*T_lm_t_Im;

                dAp += coeff_a*a_lm_Re[idx]*P_lm_p_Re;
                dAp -= coeff_a*a_lm_Im[idx]*P_lm_p_Im;
                dAp += coeff_b*b_lm_Re[idx]*T_lm_p_Re;
                dAp -= coeff_b*b_lm_Im[idx]*T_lm_p_Im;
            }
        }

        // Convert analytic radial derivative to Cartesian
        const vector dA_dr_cart
        (
            dAr*st*cp + dAt*ct*cp - dAp*sp,
            dAr*st*sp + dAt*ct*sp + dAp*cp,
            dAr*ct    - dAt*st
        );

        // ----- angular derivatives by finite differences on the sphere -----
        const scalar hTheta = 1e-6;
        const scalar hPhi   = 1e-6;

        // Avoid stepping exactly onto the poles
        const scalar thetaP = Foam::min(constant::mathematical::pi - 1e-8, theta + hTheta);
        const scalar thetaM = Foam::max(1e-8, theta - hTheta);
        const scalar phiP   = phi + hPhi;
        const scalar phiM   = phi - hPhi;

        const vector A_thetaP = evalA_cart(thetaP, phi);
        const vector A_thetaM = evalA_cart(thetaM, phi);
        const vector dA_dtheta_cart = (A_thetaP - A_thetaM)/(thetaP - thetaM);

        const vector A_phiP = evalA_cart(theta, phiP);
        const vector A_phiM = evalA_cart(theta, phiM);
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
        Av.gradient()[face] = Av.gradient()[face] * 0.9 + dA_dn * 0.1;
	}
	Av.updateCoeffs();
}
A.correctBoundaryConditions();
