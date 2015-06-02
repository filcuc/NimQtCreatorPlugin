#pragma once

#include <projectexplorer/abstractprocessstep.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/buildsteplist.h>

namespace NimPlugin {

/// The Nim compiler build step
class NimCompilerBuildStep : public ProjectExplorer::AbstractProcessStep
{
    Q_OBJECT

public:
    /// Constructor
    NimCompilerBuildStep(ProjectExplorer::BuildStepList* parentList);

    /// Create the widget that is shown inside the build step of the configuration page
    ProjectExplorer::BuildStepConfigWidget *createConfigWidget() Q_DECL_OVERRIDE;

private Q_SLOTS:
    /// Updates the nim compiler process flags.
    /// It's called when something change in the build configuration
    void updateProcessParameters();

private:
    /// Connect the build configuration change signals
    void connectBuildConfigurationSignals();

    void updateCommand();
    void updateWorkingDirectory();
    void updateArguments();
    void updateEnvironment();
};

}

