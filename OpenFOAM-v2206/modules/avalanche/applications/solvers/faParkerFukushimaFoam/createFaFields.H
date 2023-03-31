    Info << "Reading field h" << endl;
    areaScalarField h
    (
        IOobject
        (
            "h",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        aMesh
    );


    Info << "Reading field Us" << endl;
    areaVectorField Us
    (
        IOobject
        (
            "Us",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        aMesh
    );

    Info << "Reading field c" << endl;
    areaScalarField c
    (
        IOobject
        (
            "c",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        aMesh
    );

    Info << "Reading field hentrain" << endl;
    areaScalarField hentrain
    (
        IOobject
        (
            "hentrain",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        aMesh,
        dimensionedScalar("hentrain0", dimLength, GREAT)
    );

    edgeScalarField phis
    (
        IOobject
        (
            "phis",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fac::interpolate(Us) & aMesh.Le()
    );

    edgeScalarField phi2s
    (
        IOobject
        (
            "phi2s",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fac::interpolate(h*Us) & aMesh.Le()
    );


    areaVectorField n = aMesh.faceAreaNormals();

    areaScalarField gn
    (
        IOobject
        (
            "gn",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        g & n
    );

    areaVectorField gs
    (
        IOobject
        (
            "gs",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        g - gn*n
    );

    areaScalarField ew
    (
        IOobject
        (
            "ew",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        aMesh,
        dimensionedScalar(dimVelocity)
    );

    areaScalarField Sd
    (
        IOobject
        (
            "Sd",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        aMesh,
        dimensionedScalar(dimVelocity)
    );


    areaVectorField tau
    (
        IOobject
        (
            "tau",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        aMesh,
        dimensionedVector(dimPressure/dimDensity)
    );

    areaScalarField boundaryCell
    (
        IOobject
        (
            "dist",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        aMesh,
        dimensionedScalar("one", dimless, 1)
    );

    const faPatchList& patches = aMesh.boundary();

    const labelList& owner = aMesh.edgeOwner();

    forAll(Us.boundaryField(), patchi)
    {
        if (Us.boundaryField()[patchi].type() != "processor" &&
            Us.boundaryField()[patchi].fixesValue()){

            for(label faceI = patches[patchi].start(); faceI < patches[patchi].start()+patches[patchi].size(); faceI++)
            {
                boundaryCell[owner[faceI]] = 0;
            }
        }
    }
