{   

  // #include "interfaceProperties.H"

//   interfaceProperties interfaceProps(alpha1, U, dict);

//   // Access nHatfv via interfaceProps object
//   const surfaceVectorField& nHatfv = interfaceProps.faceUnitInterfaceNormal();
// // Assuming alpha1, U, and dict are defined elsewhere in your code:
    // const IOdictionary& transportPropertiesDict = IOdictionary(IOobject
    // (
    //     "transportProperties",
    //     mesh.time().constant(),
    //     mesh,
    //     IOobject::MUST_READ_IF_MODIFIED,
    //     IOobject::NO_WRITE
    // ));

    // Foam::interfaceProperties interfaceProps(alpha1, U, transportPropertiesDict);

    // // Access nHatfv using the interfaceProps object:
    // const surfaceVectorField& nHatfv = interfaceProps.faceUnitInterfaceNormal();


    fvScalarMatrix TEqn
    (
        fvm::ddt(rho, T) + fvm::div(rhoPhi, T) - fvm::Sp(contErr, T)
      - fvm::laplacian(turbulence.alphaEff(), T)
      + (
             divUp - contErr/rho*p
          +  (fvc::ddt(rho, K) + fvc::div(rhoPhi, K))() - contErr*K
        )
       *(
           alpha1()/mixture.thermo1().Cv()()
         + alpha2()/mixture.thermo2().Cv()()
        )
     ==
        fvOptions(rho, T)
        + Qhf*Qhf_loc*(max(dimensionedScalar("zeroValue", dimless/dimLength, 0.0), Qhf_dir & fvc::reconstruct( fvc::snGrad(alpha1f) * mesh.magSf())))
        // + Qhf*(max(dimensionedScalar("zeroValue", dimLength/dimLength/dimLength, 0.0), Qhf_dir & fvc::reconstruct( fvc::snGrad(alpha1f) * mesh.magSf())))
        // + Qhf*(max(0.0,Qhf_dir & fvc::reconstruct( fvc::snGrad(alpha1f) * mesh.magSf())))
        // + Qhf*(Qhf_dir & fvc::reconstruct( fvc::snGrad(alpha1f) * mesh.magSf()))
        // + Qhf*(Qhf_dir & fvc::interpolate(fvc::snGrad(alpha1f)))
        // + Qhf*(Qhf_dir & fvc::grad(alpha1f))
        // + dot(nHatfv , Qhf_dir)


        // +mixture.faceUnitInterfaceNormal()
        // + max(dimensionedScalar("zeroValue", dimMass/	dimLength/	dimLength/	dimLength/ dimTime * dimTemperature, 0.0),
        //       Qhf * (Qhf_dir & fvc::grad(alpha1f)))

                      // + Qhf * max(zeroValue, dotProduct)


        // + max(dimensionedScalar("zeroValue", dimForce/dimLength/dimTemperature, 0.0),Qhf*(Qhf_dir & fvc::grad(alpha1f)))

        // + Qhf*(Qhf_dir & fvc::grad(alpha1f))
        // /(mag(Qhf_dir)*mag(fvc::grad(alpha1f)))


        //+Qhf
        //*(1)/(mag(Qhf_dir)*mag(fvc::grad(alpha1f)))*scalar(2)*rho/(rho1+rho2)
        // + Qhf_dir & fvc::grad(alpha1f)
    );

    TEqn.relax();

    fvOptions.constrain(TEqn);

    TEqn.solve();

    fvOptions.correct(T);

    mixture.correctThermo();
    mixture.correct();

    Info<< "Fluid Min/Max T:" << min(T).value() << ' '
    << max(T).value() << endl;

}
