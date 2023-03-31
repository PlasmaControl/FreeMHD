IOdictionary transportProperties
(
    IOobject
    (
        "transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    )
);


dimensionedScalar hmin("hmin", dimLength, transportProperties);

dimensionedScalar cmin("cmin", dimless, transportProperties);

dimensionedScalar h0("h0", dimLength, transportProperties);

dimensionedScalar u0("u0", dimVelocity, transportProperties);

dimensionedScalar R("R", dimless, transportProperties);

dimensionedScalar Ds("Ds", dimLength, transportProperties);

dimensionedScalar nu("nu", dimViscosity, transportProperties);

dimensionedScalar hwem("hwem", dimLength, transportProperties);

dimensionedScalar hentmin("hentmin", dimLength, transportProperties);

Switch waterSink(transportProperties.getOrDefault<Switch>("waterSink", false));

Switch bindHeight(transportProperties.getOrDefault<Switch>("bindHeight", true));


Info << "Running with" << endl
     << "    hwem " << hwem << endl
     << "    hmin " << hmin << endl
     << "    cmin " << cmin << endl
     << "    h0   " << h0 << endl
     << "    u0   " << u0 << endl
     << "    waterSink is " << waterSink << endl
     << "    bindHeight is " << bindHeight << endl << endl;

