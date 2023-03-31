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

#include "workflowControls.H"
#include "polyMeshGen.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

const std::map<Foam::word, Foam::label>
Foam::Module::workflowControls::workflowSteps_ =
    Foam::Module::workflowControls::populateWorkflowSteps();


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

bool Foam::Module::workflowControls::restartRequested() const
{
    const dictionary& meshDict =
        mesh_.returnTime().lookupObject<dictionary>("meshDict");

    bool restart = false;

    if
    (
        meshDict.found("workflowControls")
     && meshDict.isDict("workflowControls")
    )
    {
        const dictionary& controls =
            meshDict.subDict("workflowControls");

        controls.readIfPresent("restartFromLatestStep", restart);
    }

    return restart;
}


void Foam::Module::workflowControls::setStepCompleted() const
{
    if (mesh_.metaData().found("lastStep"))
    {
        mesh_.metaData().set("lastStep", currentStep_);
    }
    else
    {
        mesh_.metaData().add("lastStep", currentStep_);
    }

    DynList<word> completedSteps;
    if (mesh_.metaData().found("completedSteps"))
        completedSteps = wordList(mesh_.metaData().lookup("completedSteps"));

    completedSteps.append(currentStep_);

    if (mesh_.metaData().found("completedSteps"))
    {
        mesh_.metaData().set("completedSteps", completedSteps);
    }
    else
    {
        mesh_.metaData().add("completedSteps", completedSteps);
    }
}


bool Foam::Module::workflowControls::isStepCompleted() const
{
    const word latestStep = lastCompletedStep();

    if (latestStep.empty())
        return false;

    const label currVal = workflowSteps_.find(currentStep_)->second;
    const label latestVal = workflowSteps_.find(latestStep)->second;

    if (latestVal == currVal)
        return true;

    return false;
}


bool Foam::Module::workflowControls::exitAfterCurrentStep() const
{
    const dictionary& meshDict =
        mesh_.returnTime().lookupObject<dictionary>("meshDict");

    if (meshDict.isDict("workflowControls"))
    {
        const dictionary& controls =
            meshDict.subDict("workflowControls");

        word exitStep;

        if
        (
            controls.readIfPresent("stopAfter", exitStep)
         && exitStep == currentStep_
        )
        {
            return true;
        }
    }

    return false;
}


Foam::word Foam::Module::workflowControls::lastCompletedStep() const
{
    if (mesh_.metaData().found("lastStep"))
    {
        const word latestStep(mesh_.metaData().lookup("lastStep"));

        return latestStep;
    }

    return word();
}


Foam::Module::DynList<Foam::word> 
Foam::Module::workflowControls::completedSteps() const
{
    DynList<word> completedSteps;

    if (mesh_.metaData().found("completedSteps"))
    {
        completedSteps = wordList(mesh_.metaData().lookup("completedSteps"));
    }

    return completedSteps;
}


void Foam::Module::workflowControls::clearCompletedSteps()
{
    mesh_.metaData().remove("completedSteps");
    mesh_.metaData().remove("lastStep");
}


bool Foam::Module::workflowControls::stopAfterCurrentStep() const
{
    setStepCompleted();

    if (exitAfterCurrentStep())
    {
        bool writeSuccess(true);

        try
        {
            Info<< "Saving mesh generated after step " << currentStep_ << endl;
            mesh_.write();
        }
        catch (...)
        {
            writeSuccess = false;
        }

        returnReduce(writeSuccess, minOp<bool>());

        if (!writeSuccess)
            FatalErrorInFunction
                << "Mesh was not written on disk" << exit(FatalError);


        std::string message("Stopping after step ");
        message += currentStep_;

        throw message;

        return true;
    }

    return false;
}


bool Foam::Module::workflowControls::runAfterCurrentStep() const
{
    if (currentStep_ == restartAfterStep_)
    {
        try
        {
            Info<< "Reading mesh generated after step "
                << currentStep_ << endl;

            mesh_.read();

            isRestarted_ = true;

            return true;
        }
        catch (...)
        {
            FatalErrorInFunction
                << "Mesh cannot be loaded. Exitting..." << exit(FatalError);
        }
    }

    return false;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

std::map<Foam::word, Foam::label>
Foam::Module::workflowControls::populateWorkflowSteps()
{
    // Note: can likely use initializer_list here
    std::map<word, label> workflowSteps;
    workflowSteps.insert(std::make_pair(word("start"), 0));
    workflowSteps.insert(std::make_pair(word("templateGeneration"), 1));
    workflowSteps.insert(std::make_pair(word("surfaceTopology"), 2));
    workflowSteps.insert(std::make_pair(word("surfaceProjection"), 4));
    workflowSteps.insert(std::make_pair(word("patchAssignment"), 8));
    workflowSteps.insert(std::make_pair(word("edgeExtraction"), 16));
    workflowSteps.insert(std::make_pair(word("meshOptimisation"), 32));
    workflowSteps.insert(std::make_pair(word("boundaryLayerGeneration"), 64));
    workflowSteps.insert(std::make_pair(word("boundaryLayerRefinement"), 128));

    return workflowSteps;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::Module::workflowControls::workflowControls(polyMeshGen& mesh)
:
    mesh_(mesh),
    currentStep_("start"),
    restartAfterStep_(),
    completedStepsBeforeRestart_(),
    isRestarted_(false)
{
    if (restartRequested())
    {
        restartAfterStep_ = lastCompletedStep();
        completedStepsBeforeRestart_ = completedSteps();
    }
    else
    {
        clearCompletedSteps();

    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

bool Foam::Module::workflowControls::runCurrentStep(const word& stepName)
{
    if
    (
        completedStepsBeforeRestart_.size()
     && completedStepsBeforeRestart_.found(currentStep_)
     && restartRequested()
     && !isRestarted_
    )
    {
        Info<< "Step " << currentStep_ << " has already been executed" << endl;

        const bool retVal = runAfterCurrentStep();

        // this step has already been executed
        setStepCompleted();
        currentStep_ = stepName;

        return retVal;
    }
    else if (stopAfterCurrentStep())
    {
        // the process shall exit within the stopAfterCurrentStep function
        return false;
    }

    // check if the requested step exists in the database of steps
    if (workflowSteps_.find(stepName) == workflowSteps_.end())
    {
        DynList<word> toc;
        forAllConstIters(workflowSteps_, it)
        {
            toc.append(it->first);
        }

        FatalErrorInFunction
            << "Step " << stepName << " is not a valid name."
            << " Valid step names are " << toc << exit(FatalError);
    }

    setStepCompleted();
    currentStep_ = stepName;

    return true;
}


void Foam::Module::workflowControls::workflowCompleted()
{
    if (mesh_.metaData().found("lastStep"))
        mesh_.metaData().remove("lastStep");

    if (mesh_.metaData().found("completedSteps"))
        mesh_.metaData().remove("completedSteps");
}


// ************************************************************************* //
