#pragma once

#include <projectexplorer/abstractprocessstep.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/buildsteplist.h>

namespace NimPlugin {

/// The nim compiler clean step
class NimCompilerCleanStep : public ProjectExplorer::AbstractProcessStep
{
    Q_OBJECT

public:
    /// Constructor
    NimCompilerCleanStep(ProjectExplorer::BuildStepList* parentList);

    /// Return the configuration widget
    ProjectExplorer::BuildStepConfigWidget *createConfigWidget();

private Q_SLOTS:
    /// Updates the nim compiler process flags.
    /// It's called when something change in the build configuration
    void updateProcessParameters();

private:
    /// Connect the build configuration change signals
    void connectBuildConfigurationSignals();
};

}
