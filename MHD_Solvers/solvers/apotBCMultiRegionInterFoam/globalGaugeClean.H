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
				PsiEqn.solve();
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

		forAll(fluidRegions, i)
		{
			volScalarField& psiF   = fluidPsi[i];
			volVectorField& AF     = fluidA[i];
			volVectorField& curlAF = fluidCurlA[i];
			volScalarField& divAF = fluidDivA[i];
			volVectorField& BF     = fluidB[i];
			volVectorField& B0F    = fluidB0[i];

			const fvMesh& meshF  = fluidRegions[i];

			AF -= fvc::reconstruct(fvc::snGrad(psiF) * meshF.magSf());
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
	}
}
