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
#include "VRWGraphSMPModifier.H"

# ifdef USE_OMP
#include <omp.h>
# endif

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::Module::polyMeshGenAddressing::calcPointPoints() const
{
    if (ppPtr_)
    {
        FatalErrorInFunction
            << "pointPoints already calculated"
            << abort(FatalError);
    }
    else
    {
        ppPtr_ = new VRWGraph();
        VRWGraph& pp = *ppPtr_;

        const faceListPMG& faces = mesh_.faces();
        const VRWGraph& pointFaces = this->pointFaces();

        labelList nPoints(pointFaces.size());

        # ifdef USE_OMP
        const label nThreads = 3*omp_get_num_procs();
        # pragma omp parallel num_threads(nThreads) if (nPoints.size() > 10000)
        # endif
        {
            # ifdef USE_OMP
            # pragma omp for schedule(static)
            # endif
            forAll(nPoints, i)
                nPoints[i] = 0;

            # ifdef USE_OMP
            # pragma omp for schedule(static)
            # endif
            forAll(pointFaces, pointI)
            {
                DynList<label, 32> helper;

                forAllRow(pointFaces, pointI, pfI)
                {
                    const face& f = faces[pointFaces(pointI, pfI)];

                    const label pos = f.which(pointI);
                    helper.appendUniq(f.prevLabel(pos));
                    helper.appendUniq(f.nextLabel(pos));
                }

                nPoints[pointI] = helper.size();
            }

            # ifdef USE_OMP
            # pragma omp barrier

            # pragma omp master
            # endif
            VRWGraphSMPModifier(pp).setSizeAndRowSize(nPoints);

            # ifdef USE_OMP
            # pragma omp barrier

            # pragma omp for schedule(static)
            # endif
            forAll(pointFaces, pointI)
            {
                DynList<label, 32> helper;

                forAllRow(pointFaces, pointI, pfI)
                {
                    const face& f = faces[pointFaces(pointI, pfI)];

                    const label pos = f.which(pointI);
                    const label pLabel = f.prevLabel(pos);
                    const label nLabel = f.nextLabel(pos);

                    helper.appendUniq(nLabel);
                    helper.appendUniq(pLabel);
                }

                pp.setRow(pointI, helper);
            }
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::Module::VRWGraph&
Foam::Module::polyMeshGenAddressing::pointPoints() const
{
    if (!ppPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
        {
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        }
        # endif

        calcPointPoints();
    }

    return *ppPtr_;
}


// ************************************************************************* //
