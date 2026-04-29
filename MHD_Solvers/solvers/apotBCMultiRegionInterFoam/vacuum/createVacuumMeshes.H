    const wordList vacuumNames(rp["vacuum"]);

    PtrList<fvMesh> vacuumRegions(vacuumNames.size());

    forAll(vacuumNames, i)
    {
        Info<< "Create vacuum mesh for region " << vacuumNames[i]
            << " for time = " << runTime.timeName() << nl << endl;

        vacuumRegions.set
        (
            i,
            new fvMesh
            (
                IOobject
                (
                    vacuumNames[i],
                    runTime.timeName(),
                    runTime,
                    IOobject::MUST_READ
                )
            )
        );
    }
