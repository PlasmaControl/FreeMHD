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

#include "meshSurfaceEngine.H"
#include "demandDrivenData.H"

# ifdef USE_OMP
#include <omp.h>
# endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline const Foam::Module::polyMeshGen&
Foam::Module::meshSurfaceEngine::mesh() const
{
    return mesh_;
}


inline const Foam::Module::pointFieldPMG&
Foam::Module::meshSurfaceEngine::points() const
{
    return mesh_.points();
}


inline const Foam::Module::faceListPMG&
Foam::Module::meshSurfaceEngine::faces() const
{
    return mesh_.faces();
}


inline const Foam::Module::cellListPMG&
Foam::Module::meshSurfaceEngine::cells() const
{
    return mesh_.cells();
}


inline const Foam::labelList&
Foam::Module::meshSurfaceEngine::bp() const
{
    if (!bppPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateBoundaryFaces();
        calculateBoundaryNodes();
    }

    return *bppPtr_;
}


inline const Foam::labelList&
Foam::Module::meshSurfaceEngine::boundaryPoints() const
{
    if (!boundaryPointsPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateBoundaryNodes();
    }

    return *boundaryPointsPtr_;
}


inline const Foam::faceList::subList&
Foam::Module::meshSurfaceEngine::boundaryFaces() const
{
    if (!boundaryFacesPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateBoundaryFaces();
    }

    return *boundaryFacesPtr_;
}


inline const Foam::labelList&
Foam::Module::meshSurfaceEngine::boundaryFacePatches() const
{
    if (!boundaryFacePatchPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateBoundaryFacePatches();
    }

    return *boundaryFacePatchPtr_;
}


inline const Foam::labelList&
Foam::Module::meshSurfaceEngine::faceOwners() const
{
    if (!boundaryFaceOwnersPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateBoundaryOwners();
    }

    return *boundaryFaceOwnersPtr_;
}


inline const Foam::Module::VRWGraph&
Foam::Module::meshSurfaceEngine::pointFaces() const
{
    if (!pointFacesPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculatePointFaces();
    }

    return *pointFacesPtr_;
}


inline const Foam::Module::VRWGraph&
Foam::Module::meshSurfaceEngine::pointInFaces() const
{
    if (!pointInFacePtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculatePointFaces();
    }

    return *pointInFacePtr_;
}


/*
inline const Foam::Module::VRWGraph&
Foam::Module::meshSurfaceEngine::pointPatches() const
{
    if(!pointPatchesPtr_)
    {
        # ifdef USE_OMP
        if(omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculatePointPatches();
    }

    return *pointPatchesPtr_;
}
*/

inline const Foam::Module::VRWGraph&
Foam::Module::meshSurfaceEngine::pointPoints() const
{
    if (!pointPointsPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculatePointPoints();
    }

    return *pointPointsPtr_;
}


inline const Foam::vectorField&
Foam::Module::meshSurfaceEngine::pointNormals() const
{
    if (!pointNormalsPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculatePointNormals();
    }

    return *pointNormalsPtr_;
}


inline const Foam::vectorField&
Foam::Module::meshSurfaceEngine::faceNormals() const
{
    if (!faceNormalsPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateFaceNormals();
    }

    return *faceNormalsPtr_;
}


inline const Foam::vectorField&
Foam::Module::meshSurfaceEngine::faceCentres() const
{
    if (!faceCentresPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateFaceCentres();
    }

    return *faceCentresPtr_;
}


inline const Foam::edgeList&
Foam::Module::meshSurfaceEngine::edges() const
{
    if (!edgesPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateEdgesAndAddressing();
    }

    return *edgesPtr_;
}


inline const Foam::Module::VRWGraph&
Foam::Module::meshSurfaceEngine::boundaryPointEdges() const
{
    if (!bpEdgesPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateEdgesAndAddressing();
    }

    return *bpEdgesPtr_;
}


inline const Foam::Module::VRWGraph&
Foam::Module::meshSurfaceEngine::edgeFaces() const
{
    if (!edgeFacesPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateEdgeFacesAddressing();
    }

    return *edgeFacesPtr_;
}


inline const Foam::Module::VRWGraph&
Foam::Module::meshSurfaceEngine::faceEdges() const
{
    if (!faceEdgesPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateFaceEdgesAddressing();
    }

    return *faceEdgesPtr_;
}


inline const Foam::Module::VRWGraph&
Foam::Module::meshSurfaceEngine::edgePatches() const
{
    if (!edgePatchesPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateEdgePatchesAddressing();
    }

    return *edgePatchesPtr_;
}


inline const Foam::Module::VRWGraph&
Foam::Module::meshSurfaceEngine::faceFaces() const
{
    if (!faceFacesPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateFaceFacesAddressing();
    }

    return *faceFacesPtr_;
}


inline const Foam::labelList&
Foam::Module::meshSurfaceEngine::globalBoundaryPointLabel() const
{
    if (!globalBoundaryPointLabelPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryPointLabels();
    }

    return *globalBoundaryPointLabelPtr_;
}


inline const Foam::Map<Foam::label>&
Foam::Module::meshSurfaceEngine::globalToLocalBndPointAddressing() const
{
    if (!globalBoundaryPointToLocalPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryPointLabels();
    }

    return *globalBoundaryPointToLocalPtr_;
}


inline const Foam::Module::VRWGraph&
Foam::Module::meshSurfaceEngine::bpAtProcs() const
{
    if (!globalBoundaryPointLabelPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryPointLabels();
    }

    return *bpProcsPtr_;
}


inline const Foam::Module::DynList<Foam::label>&
Foam::Module::meshSurfaceEngine::bpNeiProcs() const
{
    if (!bpNeiProcsPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryPointLabels();
    }

    return *bpNeiProcsPtr_;
}


inline const Foam::labelList&
Foam::Module::meshSurfaceEngine::globalBoundaryEdgeLabel() const
{
    if (!globalBoundaryEdgeLabelPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryEdgeLabels();
    }

    return *globalBoundaryEdgeLabelPtr_;
}


inline const Foam::Map<Foam::label>&
Foam::Module::meshSurfaceEngine::globalToLocalBndEdgeAddressing() const
{
    if (!globalBoundaryEdgeToLocalPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryEdgeLabels();
    }

    return *globalBoundaryEdgeToLocalPtr_;
}


inline const Foam::Module::VRWGraph&
Foam::Module::meshSurfaceEngine::beAtProcs() const
{
    if (!globalBoundaryEdgeLabelPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryEdgeLabels();
    }

    return *beProcsPtr_;
}


inline const Foam::Module::DynList<Foam::label>&
Foam::Module::meshSurfaceEngine::beNeiProcs() const
{
    if (!beNeiProcsPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryEdgeLabels();
    }

    return *beNeiProcsPtr_;
}


inline const Foam::Map<Foam::label>&
Foam::Module::meshSurfaceEngine::otherEdgeFaceAtProc() const
{
    if (!otherEdgeFaceAtProcPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcAddressingForProcEdges();
    }

    return *otherEdgeFaceAtProcPtr_;
}


inline const Foam::Map<Foam::label>&
Foam::Module::meshSurfaceEngine::otherEdgeFacePatch() const
{
    if (!otherEdgeFacePatchPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcAddressingForProcEdges();
    }

    return *otherEdgeFacePatchPtr_;
}


inline const Foam::labelList&
Foam::Module::meshSurfaceEngine::globalBoundaryFaceLabel() const
{
    if (!globalBoundaryFaceLabelPtr_)
    {
        # ifdef USE_OMP
        if (omp_in_parallel())
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryFaceLabels();
    }

    return *globalBoundaryFaceLabelPtr_;
}


// ************************************************************************* //
