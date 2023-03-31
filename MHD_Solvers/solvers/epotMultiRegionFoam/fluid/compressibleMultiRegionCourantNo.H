    scalar CoNum = -GREAT;

    forAll(fluidRegions, regioni)
    {
        CoNum = max
        (
            compressibleCourantNo
            (
                fluidRegions[regioni],
                runTime,
                rhoFluid[regioni],
                phiFluid[regioni]
            ),
            CoNum
        );
    }
