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

Application
    Prepares the case for a parallel mesh generation run

Description
    Creates processor* directories which contain data for processors

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "Time.H"
#include "decompositionModel.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "(cfmesh)\n"
        "Create processor directories in preparation for a parallel run"
    );

    argList::noParallel();
    argList::addOption
    (
        "decomposeParDict",
        "file",
        "read decomposePar dictionary from specified location"
    );

    #include "setRootCase.H"
    #include "createTime.H"

    // Allow override of decomposeParDict location
    fileName decompDictFile;
    args.readIfPresent("decomposeParDict", decompDictFile);

    IOdictionary meshDict
    (
        IOobject
        (
            "meshDict",
            runTime.time().system(),
            runTime,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    // Get requested numberOfSubdomains directly from the dictionary.
    // Note: have no mesh yet so cannot use decompositionModel::New
    const label nDomains = decompositionMethod::nDomains
    (
        IOdictionary
        (
            IOobject::selectIO
            (
                IOobject
                (
                    decompositionModel::canonicalName,
                    runTime.time().system(),
                    runTime,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE,
                    false       // do not register
                ),
                decompDictFile
            )
        )
    );

    // Give file handler a chance to determine the output directory
    const_cast<fileOperation&>(fileHandler()).setNProcs(nDomains);

    for (label proci = 0; proci < nDomains; ++proci)
    {
        fileName dir("processor" + Foam::name(proci));
        Info<< "Creating " << dir << endl;

        // create a directory for processor data
        mkDir(runTime.path()/dir);

        // copy the contents of the const directory into processor*
        cp(runTime.path()/"constant", runTime.path()/dir);

        // generate 0 directories for
        mkDir(runTime.path()/dir/"0");
    }

    Info<< "\nEnd\n" << endl;
    return 0;
}


// ************************************************************************* //
