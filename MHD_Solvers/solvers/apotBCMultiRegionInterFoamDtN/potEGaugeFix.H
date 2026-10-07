// Global zero-mean gauge for potE across all conducting regions.
// Replaces the setReference pin; prevents bulk drift in the
// compound near-Neumann system.
{
	scalar sumPhiV = 0.0;
	scalar sumV    = 0.0;

	// Fluid region(s)
	forAll(fluidRegions, i)
	{
		const fvMesh& m = fluidRegions[i];
		const volScalarField& phi =
			m.lookupObject<volScalarField>("potE");
		sumPhiV += gSum(phi.primitiveField() * m.V().field());
		sumV    += gSum(m.V().field());
	}

	// Solid (wall) regions
	forAll(solidRegions, i)
	{
		const fvMesh& m = solidRegions[i];
		const volScalarField& phi =
			m.lookupObject<volScalarField>("potE");
		sumPhiV += gSum(phi.primitiveField() * m.V().field());
		sumV    += gSum(m.V().field());
	}

	const scalar phiMean = sumPhiV / max(sumV, SMALL);

	Info<< "Global potE mean (removed): " << phiMean << endl;

	forAll(fluidRegions, i)
	{
		volScalarField& phi =
			fluidRegions[i].lookupObjectRef<volScalarField>("potE");
		phi -= dimensionedScalar("phiMean", phi.dimensions(), phiMean);
		phi.correctBoundaryConditions();
	}
	forAll(solidRegions, i)
	{
		volScalarField& phi =
			solidRegions[i].lookupObjectRef<volScalarField>("potE");
		phi -= dimensionedScalar("phiMean", phi.dimensions(), phiMean);
		phi.correctBoundaryConditions();
	}
}
