/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2018 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    chtMultiRegionTwoPhaseEulerFoam

Group
    grpHeatTransferSolvers

Description
    Transient solver for buoyant, turbulent fluid flow and solid heat
    conduction with conjugate heat transfer between solid and fluid regions.

    It solves a two-phase Euler approach on the fluid region.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

#include "compressibleInterPhaseTransportModel.H"
#include "compressibleCourantNo.H"
#include "pimpleControl.H"
#include "fixedGradientFvPatchFields.H"
#include "regionProperties.H"
#include "solidRegionDiffNo.H"
#include "solidThermo.H"
#include "radiationModel.H"
#include "fvOptions.H"
#include "fvcSmooth.H"
#include "coordinateSystem.H"
#include "loopControl.H"

#include "turbulentFluidThermoModel.H"
#include "pressureControl.H"

#include "CMULES.H"
#include "EulerDdtScheme.H"
#include "localEulerDdtScheme.H"
#include "CrankNicolsonDdtScheme.H"
#include "subCycle.H"

#include "dynamicFvMesh.H"


//#include "spherical_harmonics.H"
const int kCacheSize = 16;

double Factorial(int x) {
  static const double factorial_cache[kCacheSize] = {1, 1, 2, 6, 24, 120, 720, 5040,
                                              40320, 362880, 3628800, 39916800,
                                              479001600, 6227020800,
                                              87178291200, 1307674368000};

  if (x < kCacheSize) {
    return factorial_cache[x];
  } else {
    double s = factorial_cache[kCacheSize - 1];
    for (int n = kCacheSize; n <= x; n++) {
      s *= n;
    }
    return s;
  }
}
double DoubleFactorial(int x) {
  static const double dbl_factorial_cache[kCacheSize] = {1, 1, 2, 3, 8, 15, 48, 105,
                                                  384, 945, 3840, 10395, 46080,
                                                  135135, 645120, 2027025};

  if (x < kCacheSize) {
    return dbl_factorial_cache[x];
  } else {
    double s = dbl_factorial_cache[kCacheSize - (x % 2 == 0 ? 2 : 1)];
    double n = x;
    while (n >= kCacheSize) {
      s *= n;
      n -= 2.0;
    }
    return s;
  }
}
double EvalLegendrePolynomial(int l, int m, double x) {
  // Compute Pmm(x) = (-1)^m(2m - 1)!!(1 - x^2)^(m/2), where !! is the double
  // factorial.
  double pmm = 1.0;
  // P00 is defined as 1.0, do don't evaluate Pmm unless we know m > 0
  if (m > 0) {
    double sign = (m % 2 == 0 ? 1 : -1);
    pmm = sign * DoubleFactorial(2 * m - 1) * Foam::pow(1 - x * x, m / 2.0);
  }

  if (l == m) {
    // Pml is the same as Pmm so there's no lifting to higher bands needed
    return pmm;
  }

  // Compute Pmm+1(x) = x(2m + 1)Pmm(x)
  double pmm1 = x * (2 * m + 1) * pmm;
  if (l == m + 1) {
    // Pml is the same as Pmm+1 so we are done as well
    return pmm1;
  }

  // Use the last two computed bands to lift up to the next band until l is
  // reached, using the recurrence relationship:
  // Pml(x) = (x(2l - 1)Pml-1 - (l + m - 1)Pml-2) / (l - m)
  for (int n = m + 2; n <= l; n++) {
    double pmn = (x * (2 * n - 1) * pmm1 - (n + m - 1) * pmm) / (n - m);
    pmm = pmm1;
    pmm1 = pmn;
  }
  // Pmm1 at the end of the above loop is equal to Pml
  return pmm1;
}
scalar EvalSHSlow_Re(int l, int m, scalar phi, scalar theta) {
 //CHECK(l >= 0, "l must be at least 0.");
 //CHECK(-l <= m && m <= l, "m must be between -l and l.");

  double kml = Foam::sqrt((2.0 * l + 1) * Factorial(l - abs(m)) /
                    (4.0 * M_PI * Factorial(l + abs(m))));
  if (m > 0) {
    return kml * Foam::cos(m * phi) *
        EvalLegendrePolynomial(l, m, Foam::cos(theta));
  } else if (m < 0) {
    return kml * Foam::cos(-m * phi) *
        EvalLegendrePolynomial(l, -m, Foam::cos(theta));
  } else {
    return kml * EvalLegendrePolynomial(l, 0, Foam::cos(theta));
  }
}
  scalar EvalSHSlow_Im(int l, int m, scalar phi, scalar theta) {
  //CHECK(l >= 0, "l must be at least 0.");
  //CHECK(-l <= m && m <= l, "m must be between -l and l.");

    double kml = Foam::sqrt((2.0 * l + 1) * Factorial(l - abs(m)) /
                      (4.0 * M_PI * Factorial(l + abs(m))));
    if (m > 0) {
      return kml * Foam::sin(m * phi) *
          EvalLegendrePolynomial(l, m, Foam::cos(theta));
    } else if (m < 0) {
      return kml * Foam::sin(-m * phi) *
          EvalLegendrePolynomial(l, -m, Foam::cos(theta));
    } else {
      return kml * EvalLegendrePolynomial(l, 0, Foam::cos(theta));
    }
}
const scalar L = 10;
scalarField a_lm_x_Re(2*L*L, 0.0);
scalarField a_lm_x_Im(2*L*L, 0.0);
scalarField a_lm_y_Re(2*L*L, 0.0);
scalarField a_lm_y_Im(2*L*L, 0.0);
scalarField a_lm_z_Re(2*L*L, 0.0);
scalarField a_lm_z_Im(2*L*L, 0.0);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "Transient solver for two compressible, non-isothermal immiscible fluids"
        "using VOF phase-fraction based interface capturing, with"
        "solid heat conduction with conjugate heat transfer "
        "between solid and fluid regions."
    );

    #define NO_CONTROL
    #define CREATE_MESH createMeshesPostProcess.H
    #include "postProcess.H"

    // #include "setRootCase.H"
    #include "setRootCaseLists.H"
    #include "createTime.H"

    #include "createMeshes.H"
    #include "createVacuumMeshes.H"
    #include "createFields.H"
    
    #include "initContinuityErrs.H"
    #include "createTimeControls.H"
    #include "readSolidTimeControls.H"

    const bool LTS = false; 

    if (!LTS) 
    {
        #include "compressibleMultiRegionCourantNo.H"
        #include "solidRegionDiffusionNo.H"
        #include "setInitialMultiRegionDeltaT.H"
    }

    Info<< "\nStarting time loop\n" << endl;

    #include "createMhdFields.H"
    
    while (runTime.run())
    {

        #include "readTimeControls.H"
        #include "readSolidTimeControls.H"
        #include "readPIMPLEControls.H"

        if (LTS)
        {
            // #include "setMultiRegionRDeltaT.H" 
        }
        else
        {
            #include "alphaMultiRegionCourantNo.H"
            #include "compressibleMultiRegionCourantNo.H"
            #include "solidRegionDiffusionNo.H"
            #include "setMultiRegionDeltaT.H"
        }

        ++runTime;

        Info<< "Time = " << runTime.timeName() << nl << endl;

        if (nOuterCorr != 1)
        {
            forAll(fluidRegions, i)
            {
                #include "storeOldFluidFields.H"
            }
        }

        // --- PIMPLE loop
        for (int oCorr=0; oCorr<nOuterCorr; ++oCorr)
        {
            const bool finalIter = (oCorr == nOuterCorr-1);

            forAll(vacuumRegions, i)
            {
                Info<<"\nSolving for vacuum region "
                    << vacuumRegions[i].name() << endl;
                #include "setRegionMhdVacuumFields.H"
                #include "solveMhdVacuum.H"
            }

            forAll(fluidRegions, i)
            {
                Info<< "\nSolving for fluid region "
                    << fluidRegions[i].name() << endl;
                #include "setRegionFluidFields.H" 
                    
                #include "setRegionMhdFluidFields.H"
                #include "readFluidMultiRegionPIMPLEControls.H" 
                #include "solveMhdFluid.H"
            }

            forAll(solidRegions, i)
            {
                Info<< "\nSolving for solid region "
                    << solidRegions[i].name() << endl;
                #include "setRegionSolidFields.H"
                #include "setRegionMhdSolidFields.H"
                #include "readSolidMultiRegionPIMPLEControls.H"
                #include "solveMhdSolid.H"
            }

            // Additional loops for energy solution only
            if (!oCorr && nOuterCorr > 1)
            {
                loopControl looping(runTime, pimple, "energyCoupling");

                while (looping.loop())
                {
                    Info<< nl << looping << nl;

                    forAll(fluidRegions, i)
                    {
                        Info<< "\nSolving for fluid region "
                            << fluidRegions[i].name() << endl;
                       #include "setRegionFluidFields.H"
                       #include "setRegionMhdFluidFields.H"
                       #include "readFluidMultiRegionPIMPLEControls.H"
                       frozenFlow = true;
                       #include "solveMhdFluid.H"
                    }

                    forAll(solidRegions, i)
                    {
                        Info<< "\nSolving for solid region "
                            << solidRegions[i].name() << endl;
                        #include "setRegionSolidFields.H"
                        #include "setRegionMhdSolidFields.H"
                        #include "readSolidMultiRegionPIMPLEControls.H"
                        #include "solveMhdSolid.H"
                    }
                }
            }
        }

        runTime.write();

        runTime.printExecutionTime(Info);
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
