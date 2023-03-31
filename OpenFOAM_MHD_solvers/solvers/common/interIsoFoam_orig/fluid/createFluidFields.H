PtrList<twoPhaseMixtureThermo> mixtureFluid(fluidRegions.size());

PtrList<volScalarField> ghFluid(fluidRegions.size());
PtrList<surfaceScalarField> ghfFluid(fluidRegions.size());
PtrList<uniformDimensionedScalarField> hRefFluid(fluidRegions.size());

PtrList<volScalarField> p_rghFluid(fluidRegions.size());
PtrList<volVectorField> UFluid(fluidRegions.size());
PtrList<surfaceScalarField> phiFluid(fluidRegions.size());
PtrList<surfaceScalarField> rhoPhiFluid(fluidRegions.size());
PtrList<volScalarField> rhoFluid(fluidRegions.size());


PtrList<const bool> alphaRestartFluid(fluidRegions.size());
PtrList<volScalarField> dgdtFluid(fluidRegions.size());
PtrList<surfaceScalarField> alphaPhi10Fluid(fluidRegions.size());

PtrList<autoPtr<advectionSchemes>> advectorFluid(fluidRegions.size());

PtrList<volScalarField> KFluid(fluidRegions.size());

PtrList<multivariateSurfaceInterpolationScheme<scalar>::fieldTable>
    fieldsFluid(fluidRegions.size());

List<scalar> initialMassFluid(fluidRegions.size());
List<bool> frozenFlowFluid(fluidRegions.size(), false);

PtrList<dimensionedScalar> pMinFluid(fluidRegions.size());

PtrList<compressibleInterPhaseTransportModel> turbulenceFluid(fluidRegions.size());

PtrList<fv::options>  fvOptionsFluid(fluidRegions.size());


PtrList<IOMRFZoneList> MRFfluid(fluidRegions.size());

// Same for all fluid zones!
Info<< "\nReading g" << endl;
const meshObjects::gravity& g = meshObjects::gravity::New(runTime);



forAll(fluidRegions, i)
{

    Info<< "*** Reading fluid mesh thermophysical properties for region "
        << fluidRegions[i].name() << nl << endl;


    Info<< "    Adding p_rghFluid\n" << endl;
    p_rghFluid.set
    (
        i,
        new volScalarField
        (
            IOobject
            (
                "p_rgh",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[i]
        )
    );

    Info<< "    Adding UFluid\n" << endl;
    UFluid.set
    (
        i,
        new volVectorField
        (
            IOobject
            (
                "U",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[i]
        )
    );
    
    Info<< "    Adding face flux field phiFluid\n" << endl;
    phiFluid.set
    (
        i,
        new surfaceScalarField
        (
        IOobject
        (
            "phi",
            runTime.timeName(),
            fluidRegions[i],
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fvc::flux(UFluid[i])
        )
    );

    Info<< "    Adding contructed twoPhaseMixtureThermo\n" << endl;
    mixtureFluid.set
    (
        i,
        new twoPhaseMixtureThermo
        (
            UFluid[i],
            phiFluid[i]
        )
    );

    volScalarField& alpha1(mixtureFluid[i].alpha1());
    volScalarField& alpha2(mixtureFluid[i].alpha2());

    Info<< "    Reading thermophysical properties\n" << endl;

    const volScalarField& rho1 = mixtureFluid[i].thermo1().rho();
    const volScalarField& rho2 = mixtureFluid[i].thermo2().rho();

     Info<< "   Adding rhoFluid\n" << endl;

    rhoFluid.set
    (   i,
        new volScalarField
        (
            IOobject
            (
                "rho",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::READ_IF_PRESENT,
                IOobject::AUTO_WRITE
            ),
            alpha1*rho1 + alpha2*rho2
        )
    );

    
    pMinFluid.set
    (
        i,
        new dimensionedScalar
        (
            "pMin",
            dimPressure,
            mixtureFluid[i]
        )
    );

    fluidRegions[i].setFluxRequired(p_rghFluid[i].name());
  
    Info<< "    Correcting initialMassFluid\n" << endl;
    initialMassFluid[i] = fvc::domainIntegrate(rhoFluid[i]).value();

    const dictionary& pimpleDict =
        fluidRegions[i].solutionDict().subDict("PIMPLE");

    pimpleDict.readIfPresent("frozenFlow", frozenFlowFluid[i]);

    fluidRegions[i].setFluxRequired(alpha1.name());


    Info<< "    Adding hRefFluid\n" << endl;
    hRefFluid.set
    (
        i,
        new uniformDimensionedScalarField
        (
            IOobject
            (
                "hRef",
                runTime.constant(),
                fluidRegions[i],
                IOobject::READ_IF_PRESENT,
                IOobject::NO_WRITE
            ),
            dimensionedScalar("hRef", dimLength, Zero)
        )
    );


    Info<< "    Calculating ghRef\n" << endl;
    dimensionedScalar ghRef
    (
        mag(g.value()) > SMALL
      ? g & (cmptMag(g.value())/mag(g.value()))*hRefFluid[i]
      : dimensionedScalar("ghRef", g.dimensions()*dimLength, 0)
    );

    Info<< "    Adding ghFluid\n" << endl;
    ghFluid.set
    (
        i,
        new volScalarField
        (
            "gh",
            (g & fluidRegions[i].C()) - ghRef
        )
    );

    Info<< "    Adding ghfFluid\n" << endl;
    ghfFluid.set
    (
        i,
        new surfaceScalarField
        (
            "ghf",
            (g & fluidRegions[i].Cf()) - ghRef
        )
    );


    
    Info<< "    Correcting p_rghFluid\n" << endl;

    // Force p_rgh to be consistent with p
     p_rghFluid[i] = mixtureFluid[i].p() - rhoFluid[i]*ghFluid[i]; //???

    // Mass flux
    // Initialisation does not matter because rhoPhi is reset after the
    // alpha1 solution before it is used in the U equation.
    rhoPhiFluid.set
    (
        i,
        new surfaceScalarField
        (
            IOobject
            (
                "rhoPhi",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            fvc::interpolate(rhoFluid[i])*phiFluid[i]
        )
    );

    dgdtFluid.set
    (
        i,
        new  volScalarField
        (
            alpha1*fvc::div(phiFluid[i])
        )
    );

    IOobject alphaPhi10Header
    (
        IOobject::groupName("alphaPhi0", alpha1.group()),
        runTime.timeName(),
        fluidRegions[i],
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    );

    alphaRestartFluid.set
    (
        i,
        new const bool 
        (
            alphaPhi10Header.typeHeaderOk<surfaceScalarField>(true)
        )
    );

    if (alphaRestartFluid[i])
    {
        Info << "   Restarting alpha!" << endl;
    }

    // MULES flux from previous time-step
    alphaPhi10Fluid.set
    (
        i, 
        new surfaceScalarField
        (
            alphaPhi10Header,
            phiFluid[i]*fvc::interpolate(alpha1)
        )
    );

   advectorFluid.set
   (
       i, 
        new autoPtr<advectionSchemes> 
        (
            advectionSchemes::New(alpha1,phiFluid[i],UFluid[i])
        )
   );

    // -----------------

    // Construct compressible turbulence model
    turbulenceFluid.set
    (
        i,
        new compressibleInterPhaseTransportModel
        (
            rhoFluid[i],
            UFluid[i],
            phiFluid[i],
            rhoPhiFluid[i],
            alphaPhi10Fluid[i],
            mixtureFluid[i]
        )
    );


    Info<< "Creating field kinetic energy K\n" << endl;

    KFluid.set
    (
        i, 
        new volScalarField
        (
            "K", 0.5*magSqr(UFluid[i])
        )
    );


    Info<< "    Adding MRF\n" << endl;
    MRFfluid.set
    (
        i,
        new IOMRFZoneList(fluidRegions[i])
    );

    Info<< "    Adding fvOptions\n" << endl;
    fvOptionsFluid.set
    (
        i,
        new fv::options(fluidRegions[i])
    );

    if (!fvOptionsFluid[i].optionList::size())
    {
        Info << "No finite volume options present" << endl;
    }

}
