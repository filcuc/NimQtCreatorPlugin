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

    Utils::FileName target() const;
    void setTarget(const Utils::FileName &target);

    QString additionalArguments() const;
    void setAdditionalArguments(const QString& additionalArguments);

signals:
    void additionalArgumentsChanged(const QString& args);
    void targetChanged(const Utils::FileName &target);

private:
    void updateProcessParameters();
    void updateCommand();
    void updateWorkingDirectory();
    void updateArguments();
    void updateEnvironment();

    Utils::FileName m_target;
    QString m_additionalArguments;
};

}

