#include "nimcompilercleanstep.h"
#include "nimbuildconfiguration.h"
#include "nimcompilercleanstepconfigwidget.h"

namespace NimPlugin {

NimCompilerCleanStep::NimCompilerCleanStep(ProjectExplorer::BuildStepList *parentList)
        : ProjectExplorer::AbstractProcessStep(parentList, Core::Id("Nim Clean Step"))
{
    setDefaultDisplayName(QStringLiteral("Nim Clean Step"));
    setDisplayName(QStringLiteral("Nim Clean Step"));
    connectBuildConfigurationSignals();
    updateProcessParameters();
}

ProjectExplorer::BuildStepConfigWidget *NimCompilerCleanStep::createConfigWidget()
{
    using namespace ProjectExplorer;
    return new NimCompilerCleanStepConfigWidget(this);
}

void NimCompilerCleanStep::updateProcessParameters()
{
    auto bc = dynamic_cast<NimBuildConfiguration*>(buildConfiguration());
    Q_ASSERT(bc);

    auto pp = processParameters();
    pp->setCommand(QStringLiteral("rm"));
    pp->setArguments(QStringLiteral("*"));
    pp->setWorkingDirectory(bc->buildDirectory().toString());
    pp->setEnvironment(bc->environment());
}

void NimCompilerCleanStep::connectBuildConfigurationSignals()
{
    auto bc = dynamic_cast<NimBuildConfiguration*>(buildConfiguration());
    connect(bc, SIGNAL(buildDirectoryChanged()), this, SLOT(updateProcessParameters()));
    connect(bc, SIGNAL(targetNimFileChanged(Utils::FileName)), this, SLOT(updateProcessParameters()));
    connect(bc, SIGNAL(userCompilerOptionsChanged(QStringList)), this, SLOT(updateProcessParameters()));
}

}

