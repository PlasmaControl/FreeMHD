PtrList<volVectorField> vacuumA(vacuumRegions.size()); 
PtrList<volVectorField> vacuumDADt(vacuumRegions.size()); 
PtrList<volVectorField> vacuumB(vacuumRegions.size()); 
PtrList<volScalarField> vacuumPsi(vacuumRegions.size());
PtrList<volScalarField> vacuumMagMu(vacuumRegions.size());
PtrList<dimensionedScalar> vacuumMagMuValue(vacuumRegions.size());

List<label> potERefCellVacuum(vacuumRegions.size());

forAll(vacuumRegions, i)
{
   Info<< "Initializing magnetic vector potential A for vacuum" << i << endl;
	vacuumA.set
	(
		i,
		new volVectorField
		(
			IOobject
			(
				"A",
				runTime.timeName(),
				vacuumRegions[i],
				IOobject::MUST_READ,
				IOobject::AUTO_WRITE
			),
			vacuumRegions[i]
		)
	);
	vacuumDADt.set
	(
		i,
		new volVectorField
		(
			IOobject
			(
				"dAdt",
				runTime.timeName(),
				vacuumRegions[i],
				IOobject::MUST_READ,
				IOobject::AUTO_WRITE
			),
			vacuumRegions[i]
		)
	);
	vacuumB.set
	(
		i,
		new volVectorField
		(
			IOobject
			(
				"B",
				runTime.timeName(),
				vacuumRegions[i],
				IOobject::MUST_READ,
				IOobject::AUTO_WRITE
			),
			vacuumRegions[i]
		)
	);
	vacuumPsi.set
	(
		i,
		new volScalarField
		(
			IOobject
			(
				"psi",
				runTime.timeName(),
				vacuumRegions[i],
				IOobject::MUST_READ,
				IOobject::AUTO_WRITE
			),
			vacuumRegions[i]
		)
	);
	vacuumMagMu.set
	(
		i,
		new volScalarField
		(
			IOobject
			(
				"magMu",
				runTime.timeName(),
				vacuumRegions[i],
				IOobject::NO_READ,
				IOobject::NO_WRITE
			),
			vacuumRegions[i],
			dimensionedScalar("zero", dimensionSet(1, 1, -2, 0, 0, -2, 0), 0)
		)
	);
	vacuumMagMuValue.set
	(
		i,
		new dimensionedScalar
		(
			"magMu",
			dimensionSet(1, 1, -2, 0, 0, -2, 0),
			vacuumThermos[i]
		)
	);

	vacuumMagMu[i] = vacuumMagMuValue[i];
	
	potERefCellVacuum[i] = 0;
	
	Info << "Init vacuum " << i << " finished!" << endl;
}
