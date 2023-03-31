    scalar CoNum = -GREAT;

    forAll(fluidRegions, regioni)
    {
        CoNum = max
        (
            Foam::compressibleCourantNo
            (
                fluidRegions[regioni],
                runTime,
                rhoFluid[regioni],
                phiFluid[regioni]
            ),
            CoNum
        );
    }

