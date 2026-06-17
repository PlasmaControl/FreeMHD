// Detect whether any psi patch on any fluid or solid region has
// useImplicit() == true, which means the global fvMatrix assembly
// path is available for the gauge-cleaning Poisson solve.

bool useGlobalGaugeAssembly = false;

forAll(fluidRegions, i)
{
	const volScalarField& psiF = fluidPsi[i];
	forAll(psiF.boundaryField(), patchi)
	{
		if (psiF.boundaryField()[patchi].useImplicit())
		{
			useGlobalGaugeAssembly = true;
			break;
		}
	}
	if (useGlobalGaugeAssembly) break;
}

if (!useGlobalGaugeAssembly)
{
	forAll(solidRegions, i)
	{
		const volScalarField& psiS = solidPsi[i];
		forAll(psiS.boundaryField(), patchi)
		{
			if (psiS.boundaryField()[patchi].useImplicit())
			{
				useGlobalGaugeAssembly = true;
				break;
			}
		}
		if (useGlobalGaugeAssembly) break;
	}
}

Info<< "Global gauge assembly: "
	<< (useGlobalGaugeAssembly ? "enabled" : "disabled (per-region fallback)")
	<< nl;
