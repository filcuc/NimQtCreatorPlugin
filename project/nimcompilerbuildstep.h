#pragma once

#include <projectexplorer/abstractprocessstep.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/buildsteplist.h>

namespace NimPlugin {

class NimCompilerBuildStep : public ProjectExplorer::AbstractProcessStep
{
    Q_OBJECT

public:
    NimCompilerBuildStep(ProjectExplorer::BuildStepList* parentList);

    ProjectExplorer::BuildStepConfigWidget *createConfigWidget() Q_DECL_OVERRIDE;

private Q_SLOTS:
    void updateProcessParameters();

private:
    void connectBuildConfigurationSignals();

    void updateCommand();
    void updateWorkingDirectory();
    void updateArguments();
    void updateEnvironment();

    Utils::FileName m_target;
    QString m_mandatoryArguments;
    QString m_additionalArguments;
};

}

