    // Initialise vacuum field pointer lists
    PtrList<solidThermo> vacuumThermos(vacuumRegions.size());
    PtrList<radiation::radiationModel> vacuumRadiations(vacuumRegions.size());

    // Populate vacuum field pointer lists
    forAll(vacuumRegions, i)
    {
        Info<< "*** Reading vacuum mesh thermophysical properties for region "
            << vacuumRegions[i].name() << nl << endl;

        Info<< "    Adding to vacuumThermos\n" << endl;
        vacuumThermos.set(i, solidThermo::New(vacuumRegions[i]));

        Info<< "    Adding to vacuumRadiations\n" << endl;
        vacuumRadiations.set
        (
            i,
            radiation::radiationModel::New(vacuumThermos[i].T())
        );
    }
