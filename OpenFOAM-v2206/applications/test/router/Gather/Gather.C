/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2020 OpenCFD Ltd.
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

\*---------------------------------------------------------------------------*/

#include "Gather.H"
#include "IPstream.H"
#include "OPstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from component
template<class T0>
Gather<T0>::Gather(const T0& localData, const bool redistribute)
:
    List<T0>(0),
    nProcs_(max(1, Pstream::nProcs()))
{
    this->setSize(nProcs_);

    //
    // Collect sizes on all processor
    //

    if (Pstream::parRun())
    {
        if (Pstream::master())
        {
            auto outIter = this->begin();
            *outIter = localData;

            // Receive data
            for (const int proci : Pstream::subProcs())
            {
                IPstream fromSlave(Pstream::commsTypes::scheduled, proci);
                fromSlave >> *(++outIter);
            }

            // Send data
            for (const int proci : Pstream::subProcs())
            {
                OPstream toSlave(Pstream::commsTypes::scheduled, proci);

                if (redistribute)
                {
                    toSlave << *this;
                }
                else
                {
                    // Dummy send just to balance sends/receives
                    toSlave << 0;
                }
            }
        }
        else
        {
            // Slave: send my local data to master
            {
                OPstream toMaster
                (
                    Pstream::commsTypes::scheduled,
                    Pstream::masterNo()
                );
                toMaster << localData;
            }

            // Receive data from master
            {
                IPstream fromMaster
                (
                    Pstream::commsTypes::scheduled,
                    Pstream::masterNo()
                );
                if (redistribute)
                {
                    fromMaster >> *this;
                }
                else
                {
                    label dummy;
                    fromMaster >> dummy;
                }
            }
        }
    }
    else
    {
        this->operator[](0) = localData;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
