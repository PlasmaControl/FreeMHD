/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | cfMesh: A library for mesh generation
   \\    /   O peration     |
    \\  /    A nd           | www.cfmesh.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2014-2017 Creative Fields, Ltd.
-------------------------------------------------------------------------------
Author
     Franjo Juretic (franjo.juretic@c-fields.com)

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

\*---------------------------------------------------------------------------*/

#include "polyMeshGenAddressing.H"
#include "DynList.H"
#include "VRWGraphSMPModifier.H"

# ifdef USE_OMP
#include <omp.h>
# endif

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::Module::polyMeshGenAddressing::calcCellEdges() const
{
    if (cePtr_)
    {
        FatalErrorInFunction
            << "cellEdges already calculated"
            << abort(FatalError);
    }
    else
    {
        const cellListPMG& cells = mesh_.cells();
        const VRWGraph& fe = faceEdges();

        cePtr_ = new VRWGraph();
        VRWGraph& cellEdgeAddr = *cePtr_;

        labelList nEdges(cells.size());

        # ifdef USE_OMP
        const label nThreads = 3*omp_get_num_procs();
        # endif

        # ifdef USE_OMP
        # pragma omp parallel num_threads(nThreads) if (cells.size() > 10000)
        # endif
        {
            # ifdef USE_OMP
            # pragma omp for schedule(static)
            # endif
            forAll(nEdges, i)
            {
                nEdges[i] = 0;
            }

            # ifdef USE_OMP
            # pragma omp for schedule(static)
            # endif
            forAll(cells, cellI)
            {
                const cell& c = cells[cellI];

                DynList<label, 32> cEdges;

                forAll(c, fI)
                {
                    const label faceI = c[fI];

                    forAllRow(fe, faceI, eI)
                    {
                        cEdges.appendUniq(fe(faceI, eI));
                    }
                }

                nEdges[cellI] = cEdges.size();
            }

            # ifdef USE_OMP
            # pragma omp barrier

            # pragma omp master
            # endif
            VRWGraphSMPModifier(cellEdgeAddr).setSizeAndRowSize(nEdges);

            # ifdef USE_OMP
            # pragma omp barrier

            # pragma omp for schedule(static)
            # endif
            forAll(cells, cellI)
            {
                const cell& c = cells[cellI];

                DynList<label, 32> cEdges;

                forAll(c, fI)
                {
                    const label faceI = c[fI];

                    forAllRow(fe, faceI, eI)
                    {
                        cEdges.appendUniq(fe(faceI, eI));
                    }
                }

                cellEdgeAddr.setRow(cellI, cEdges);
            }
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::Module::VRWGraph&
Foam::Module::polyMeshGenAddressing::cellEdges() const
{
    if (!cePtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
        {
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        }
        # endif

        calcCellEdges();
    }

    return *cePtr_;
}


// ************************************************************************* //
