
    scalar nCorr =
            aMesh.solutionDict().getOrDefault<int>("nOuterCorrectors", 50);

    scalar minCorr =
            aMesh.solutionDict().getOrDefault<int>("minCorrectors", 3);

    scalar hResidualMax =
        aMesh.solutionDict().getOrDefault<scalar>("hResidualMax", 0.);

    scalar UsResidualMax =
        aMesh.solutionDict().getOrDefault<scalar>("UsResidualMax", 1);

