#pragma once

#include <projectexplorer/abstractprocessstep.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/buildsteplist.h>

namespace NimPlugin {

/// The nim compiler clean step
class NimCompilerCleanStep : public ProjectExplorer::BuildStep
{
    Q_OBJECT

public:
    /// Constructor
    NimCompilerCleanStep(ProjectExplorer::BuildStepList* parentList);

    /// Return the configuration widget
    ProjectExplorer::BuildStepConfigWidget *createConfigWidget() Q_DECL_OVERRIDE;

    /// Initialize
    bool init() Q_DECL_OVERRIDE;

    /// Run the clean step
    void run(QFutureInterface<bool> &fi) Q_DECL_OVERRIDE;

private:
    Utils::FileName m_buildDir;
};

}
