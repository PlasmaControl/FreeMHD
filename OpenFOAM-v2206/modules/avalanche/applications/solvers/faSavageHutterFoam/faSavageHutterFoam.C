/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | avalanche module
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017 Matthias Rauter
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
    faSavageHutterFoam

Description
    A depth-integrated solver for shallow granular flows.
    The solver is based on the Finite Area Method.
    Model derivation and description:
    Rauter and Tukovic, "A finite area scheme for shallow granular flows on
    three-dimensional surfaces", Computers and Fluids:166, 184–199 (2018),
    dx.doi.org/10.1016/j.compfluid.2018.02.017

Author
    Matthias Rauter matthias@rauter.it

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "faCFD.H"
#include "frictionModel.H"
#include "entrainmentModel.H"
#include "depositionModel.H"
#include "SolverPerformance.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "(avalanche)\n"
        "A depth-integrated solver for shallow granular flows using"
        " Finite Area Methods."
    );

    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "createFaMesh.H"
    #include "readGravitationalAcceleration.H"
    #include "createFaFields.H"
    #include "readTransportProperties.H"
    #include "createTimeControls.H"

    Info<< "\nStarting time loop\n" << endl;

    #include "readSolutionControls.H"

    Info<< nl
        << "Numerical settings" << nl
        << "    max number of iterations " << nCorr << nl
        << "    min number of iterations " << minCorr << nl
        << "    TOL h " << hResidualMax << nl
        << "    TOL Us " << UsResidualMax << nl << endl;

    const bool initDeltaT = runTime.controlDict().get<bool>("initDeltaT");

    if (initDeltaT)
    {
        Info<< "Initializing Delta T" << endl;
        #include "readTimeControls.H"
        #include "surfaceCourantNo.H"
        runTime.setDeltaT
        (
            min(maxCo/(CoNum + SMALL)*runTime.deltaT().value(), maxDeltaT)
        );
    }

    bool final = false;

    while (runTime.run())
    {
        #include "readSolutionControls.H"
        #include "readTimeControls.H"
        #include "surfaceCourantNo.H"
        #include "setDeltaT.H"

        runTime++;

        Info<< "Time = " << runTime.timeName() << nl << endl;
        final = false;

        for (int iCorr = 0; ; iCorr++)
        {
            #include "calcBasalstress.H"

            const areaVectorField & tauSc = friction->tauSc();
            const areaScalarField & tauSp = friction->tauSp();

            faVectorMatrix UsEqn
            (
                fam::ddt(h, Us)
              + xi*fam::div(phi2s, Us)
              + tauSc
              + fam::Sp(tauSp, Us)
             ==
                gs*h
              - fac::grad(pb*h/(2.*friction->rho()))
            );

            if(explicitDryAreas)
            {
                scalarField &diag = UsEqn.diag();
                vectorField &source = UsEqn.source();
                scalarField &upper = UsEqn.upper();
                scalarField &lower = UsEqn.lower();

                vector forcedValue(0,0,0);
                label dryCellCount = 0;

                forAll(diag, i)
                {

                    const label startFaceOwn = aMesh.lduAddr().ownerStartAddr()[i];
                    const label endFaceOwn = aMesh.lduAddr().ownerStartAddr()[i + 1];

                    const label startFaceNbr = aMesh.lduAddr().losortStartAddr()[i];
                    const label endFaceNbr = aMesh.lduAddr().losortStartAddr()[i + 1];

                    const labelUList& owner = aMesh.lduAddr().lowerAddr();
                    const labelUList& neighbour = aMesh.lduAddr().upperAddr();
                    const labelUList& losort = aMesh.lduAddr().losortAddr();


                    if (diag[i] < 1e-8)
                    {

                        diag[i] = 1;
                        source[i] = forcedValue;

                        label faceIndex = startFaceOwn;
                        while (faceIndex < endFaceOwn)
                        {
                            source[neighbour[faceIndex]] -= lower[faceIndex]*forcedValue;
                            lower[faceIndex] = 0.0;
                            faceIndex++;
                        }

                        faceIndex = startFaceNbr;
                        while (faceIndex < endFaceNbr)
                        {
                            source[owner[losort[faceIndex]]] -= upper[losort[faceIndex]]*forcedValue;
                            upper[losort[faceIndex]] = 0.0;
                            faceIndex++;
                        }

                        dryCellCount++;
                    }
                }
                if (dryCellCount  > 0)
                {
                    Info<< "Number of dry cells = " << dryCellCount << endl;
                }
            }


            if (!final)
                UsEqn.relax();

            SolverPerformance<vector> UsResidual = solve(UsEqn);

            tau = (tauSc + tauSp*Us)*friction->rho();
            phis = (fac::interpolate(Us) & aMesh.Le());

            const areaScalarField & Sm = entrainment->Sm();
            const areaScalarField & Sd = deposition->Sd();


            h.storePrevIter();

            faScalarMatrix hEqn
            (
                fam::ddt(h)
              + fam::div(phis, h)
             ==
                Sm
              - fam::Sp
                (
                    Sd/(h + dimensionedScalar("small", dimLength, SMALL)),
                    h
                )
            );

            solverPerformance hResidual = hEqn.solve();

            phi2s = hEqn.flux();

            if (!final)
                h.relax();


            // Bind h
            if (bindHeight)
            {
                h = max
                (
                    h,
                    hmin
                );
            }

            faScalarMatrix hentrainEqn
            (
                fam::ddt(hentrain)
              ==
                Sd
                - fam::Sp
                (
                    Sm/(hentrain + dimensionedScalar("small", dimLength, SMALL)),
                    hentrain
                )
            );

            hentrainEqn.solve();


            h.correctBoundaryConditions();
            Us.correctBoundaryConditions();

            if (final)
            {
                if (hResidual.initialResidual() < hResidualMax && mag(UsResidual.initialResidual()) < UsResidualMax)
                {
                    Info<< "reached residual in h = "
                        << hResidual.initialResidual()
                        << " < " << hResidualMax
                        << " and in Us = "
                        << UsResidual.initialResidual()
                        << " < " << UsResidualMax
                        << ", stopping loop!" << endl;
                }
                else
                {
                    Info<< "Reached maximum numbers of iterations, "
                        << "stopping loop!" << endl;
                }
                break;
            }

            if
            (
                (
                    hResidual.initialResidual() < hResidualMax
                 && mag(UsResidual.initialResidual()) < UsResidualMax
                 && iCorr >= minCorr
                )
             || iCorr >= nCorr
            )
            {
                final = true;
            }
        }

        if (runTime.outputTime())
        {
            runTime.write();
        }

        runTime.printExecutionTime(Info);
    }


    Info<< nl << "End" << endl;

    return 0;
}


// ************************************************************************* //
