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
    epotMultiRegionInterFoam

Group
    grpHeatTransferSolvers

Description
    Transient solver for two-phase buoyant, turbulent fluid flow and solid heat
    conduction with conjugate heat transfer and electric potential between solid and fluid regions.

    It solves a two-phase MULES approach on the fluid region.

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


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "Transient solver for two compressible, non-isothermal immiscible fluids"
        "using the MULES VOF phase-fraction based interface capturing, with"
        "solid heat conduction with conjugate heat transfer and electric potential"
        "between solid and fluid regions."
    );

    #define NO_CONTROL
    #define CREATE_MESH createMeshesPostProcess.H
    
    #include "postProcess.H"
    #include "setRootCaseLists.H"
    #include "createTime.H"

    #include "createMeshes.H"
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
	
   #include "createMhdFields.H"
	  
    Info<< "\nStarting time loop\n" << endl;
   
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
