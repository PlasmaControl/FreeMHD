{
	label nGaugeIter
	(
		runTime.controlDict().get<label>("nGaugeIter")
	);

	for (label gIter = 0; gIter < nGaugeIter; ++gIter)
	{
		{
			forAll(fluidRegions, i)
			{
				volScalarField& psiF = fluidPsi[i];
				volVectorField& AF   = fluidA[i];
				const fvMesh& meshF  = fluidRegions[i];

				surfaceScalarField phiA = fvc::interpolate(AF) & meshF.Sf();
				fvScalarMatrix PsiEqn(fvm::laplacian(psiF) == fvc::div(phiA));
				PsiEqn.setReference(potERefCellFluid[i], 0.0);
				const solverPerformance psiPerf = PsiEqn.solve();
				{
					// [Dbg] is the psi solve returning a sane field?
					const scalarField& Vf = meshF.V();
					const scalar rmsPsi =
						Foam::sqrt(gSum(Vf*sqr(psiF.primitiveField()))/gSum(Vf));
					const scalar rmsGradPsi =
						Foam::sqrt(gSum(Vf*magSqr(fvc::grad(psiF)().primitiveField()))/gSum(Vf));
					const scalar rmsA =
						Foam::sqrt(gSum(Vf*magSqr(AF.primitiveField()))/gSum(Vf));
					const scalar rmsDivA =
						Foam::sqrt(gSum(Vf*sqr(fvc::div(AF)().primitiveField()))/gSum(Vf));
					Info<< "[Dbg] psi solve: init " << psiPerf.initialResidual()
						<< " final " << psiPerf.finalResidual()
						<< " iters " << psiPerf.nIterations()
						<< " | rms psi " << rmsPsi
						<< " rms |grad psi| " << rmsGradPsi
						<< " rms |A| " << rmsA
						<< " rms divA " << rmsDivA
						<< " | ratio |grad psi|/|A| " << rmsGradPsi/(rmsA + SMALL) << nl;
				}
			}

			forAll(solidRegions, i)
			{
				volScalarField& psiS = solidPsi[i];
				volVectorField& AS   = solidA[i];
				const fvMesh& meshS  = solidRegions[i];

				surfaceScalarField phiS = fvc::interpolate(AS) & meshS.Sf();
				fvScalarMatrix PsiEqnS(fvm::laplacian(psiS) == fvc::div(phiS));
				//PsiEqnS.setReference(potERefCellSolid[i], 0.0);
				PsiEqnS.solve();
			}

			forAll(vacuumRegions, i)
			{
				volScalarField& psiV = vacuumPsi[i];
				volVectorField& AV   = vacuumA[i];
				const fvMesh& meshV  = vacuumRegions[i];

				surfaceScalarField phiV = fvc::interpolate(AV) & meshV.Sf();
				fvScalarMatrix PsiEqnV(fvm::laplacian(psiV) == fvc::div(phiV));
				//PsiEqnV.setReference(potERefCellVacuum[i], 0.0);
				PsiEqnV.solve();
			}
		}

		// [Dbg] Gauge cleaning subtracts a discrete gradient from A.  In the
		// continuum curl(grad psi) == 0, so B must be untouched.  Measure it.
		scalar B2pre = 0.0, B2post = 0.0;
		forAll(fluidRegions, i)
		{
			B2pre += gSum
			(
				magSqr(fvc::curl(fluidA[i]))().primitiveField()
			  * fluidRegions[i].V().field()
			);
		}
		forAll(solidRegions, i)
		{
			B2pre += gSum
			(
				magSqr(fvc::curl(solidA[i]))().primitiveField()
			  * solidRegions[i].V().field()
			);
		}
		forAll(vacuumRegions, i)
		{
			B2pre += gSum
			(
				magSqr(fvc::curl(vacuumA[i]))().primitiveField()
			  * vacuumRegions[i].V().field()
			);
		}

		forAll(fluidRegions, i)
		{
			volScalarField& psiF   = fluidPsi[i];
			volVectorField& AF     = fluidA[i];
			volVectorField& curlAF = fluidCurlA[i];
			volScalarField& divAF = fluidDivA[i];
			volVectorField& BF     = fluidB[i];
			volVectorField& B0F    = fluidB0[i];

			const fvMesh& meshF  = fluidRegions[i];

			// [Dbg] VSH_gaugeGradForm: 0 (default) keeps the implemented
			// reconstruct(snGrad(psi)*magSf); 1 uses fvc::grad(psi), whose
			// curl defect with a leastSquares gradient is ~1e-3 relative.
			if (runTime.controlDict().getOrDefault<label>("VSH_gaugeGradForm", 0) == 1)
			{
				AF -= fvc::grad(psiF);
			}
			else
			{
				AF -= fvc::reconstruct(fvc::snGrad(psiF) * meshF.magSf());
			}
			AF.correctBoundaryConditions();

			const scalar BtStartTime
			(
				runTime.controlDict().get<scalar>("BtStartTime")
			);
			const scalar BtDuration
			(
				max
				(
					runTime.controlDict().get<scalar>("BtDuration"),
					scalar(1e-6)
				)
			);
			const scalar Bscale = max
			(
				min((runTime.value() - BtStartTime)/BtDuration, scalar(1)),
				scalar(0)
			);

			divAF  = fvc::div(AF);
			divAF.correctBoundaryConditions();
			curlAF = fvc::curl(AF);
			curlAF.correctBoundaryConditions();
			BF = B0F * Bscale + curlAF;
			BF.correctBoundaryConditions();
		}

		forAll(solidRegions, i)
		{
			volScalarField& psiS   = solidPsi[i];
			volVectorField& AS     = solidA[i];
			volVectorField& curlAS = solidCurlA[i];
			volScalarField& divAS = solidDivA[i];
			const fvMesh& meshS  = solidRegions[i];

			AS -= fvc::reconstruct(fvc::snGrad(psiS) * meshS.magSf());
			AS.correctBoundaryConditions();

			divAS = fvc::div(AS);
			divAS.correctBoundaryConditions();
			curlAS = fvc::curl(AS);
			curlAS.correctBoundaryConditions();
		}

		forAll(vacuumRegions, i)
		{
			volScalarField& psiV = vacuumPsi[i];
			volVectorField& AV   = vacuumA[i];
			volVectorField& BV   = vacuumB[i];
			const fvMesh& meshV  = vacuumRegions[i];

			AV -= fvc::reconstruct(fvc::snGrad(psiV) * meshV.magSf());
			AV.correctBoundaryConditions();

			BV = fvc::curl(AV);
			BV.correctBoundaryConditions();
		}

		forAll(fluidRegions, i)
		{
			B2post += gSum
			(
				magSqr(fvc::curl(fluidA[i]))().primitiveField()
			  * fluidRegions[i].V().field()
			);
		}
		forAll(solidRegions, i)
		{
			B2post += gSum
			(
				magSqr(fvc::curl(solidA[i]))().primitiveField()
			  * solidRegions[i].V().field()
			);
		}
		forAll(vacuumRegions, i)
		{
			B2post += gSum
			(
				magSqr(fvc::curl(vacuumA[i]))().primitiveField()
			  * vacuumRegions[i].V().field()
			);
		}
		Info<< "[Dbg] gauge curl check: int|curlA|^2 pre = " << B2pre
			<< "  post = " << B2post
			<< "  rel change = " << (B2post - B2pre)/(B2pre + VSMALL) << nl;
	}
}
